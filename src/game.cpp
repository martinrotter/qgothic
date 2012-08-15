#include "game.h"
#include "historyitem.h"
#include "gsettings.h"
#include "referee.h"
#include "definitions.h"
#include "simplecrypt.h"

#include <QDebug>
#include <QObject>
#include <QMetaType>
#include <QDateTime>
#include <QCryptographicHash>
#include <QXmlStreamWriter>
#include <QDomDocument>

typedef QPair<Move, int> Item;

Game::Game(QObject *parent) :
    QObject(parent), m_board(new Board()), m_history(new History(this)),
    m_generator(new Generator()), m_state(Game::PAUSED), m_isSaved(false) {

    qRegisterMetaType<Move>("Move");
    connect(m_generator, SIGNAL(moveGenerated(Move)), this, SLOT(makeMove(Move)));
    //connect(m_generator, SIGNAL(terminated()), this, SLOT(informAboutHistory()));
    //connect(m_generator, SIGNAL(terminated()), this, SIGNAL(moveSearchFinished()));
    connect(m_history, SIGNAL(changed(int)), this, SLOT(informAboutHistory()));

    int current_player = GSettings::value(SET_GAME, "starting_player", 0).toInt();
    m_currentPlayer = current_player >= 0 && current_player <= 1 ?
			  current_player :
			  0;

    setStartingPlayer(m_currentPlayer);

    int white_dif = GSettings::value(SET_GAME, "white_player_dif", 0).toInt();
    int black_dif = GSettings::value(SET_GAME, "black_player_dif", 2).toInt();
    m_players << Player(white_dif <= Player::HARD && white_dif >= Player::HUMAN ?
			    static_cast<Player::State>(white_dif) :
			    Player::HUMAN,
			Figure::WHITE);
    m_players << Player(black_dif <= Player::HARD && black_dif >= Player::HUMAN ?
			    static_cast<Player::State>(black_dif) :
			    Player::MEDIUM,
			Figure::BLACK);
}

Game::~Game() {
    delete m_board;
    delete m_generator;
}

bool Game::saveGame(const QString &file_name) {
    // vytvoří xml strukturu s historií a potřebnými daty
    // na konec qbytearray přidá MD5 checksum
    // zašifruje QByteArray
    // uloži do souboru

    QFile file(file_name);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text) == true) {
	QString array;
	QXmlStreamWriter writer(&array);

	// Initialize xml structure.
	writer.setAutoFormatting(true);
	writer.setAutoFormattingIndent(2);
	writer.setCodec("UTF-8");
	writer.writeStartDocument();

	// Write general data about application.
	writer.writeStartElement("saved-game");
	writer.writeAttribute("version", APP_VERSION);
	writer.writeAttribute("date", QDateTime::currentDateTime().toString(Qt::ISODate));

	// ještě přidat nastavení hráčů
	// Write information about game (eg. players, current player).
	writer.writeStartElement("game");
	writer.writeTextElement("max-moves-without-jump", QString::number(m_board->getMaxMovesNoJump()));
	writer.writeTextElement("starting-player", QString::number(m_startingPlayer));
	writer.writeTextElement("current-player", QString::number(m_currentPlayer));
	writer.writeTextElement("white-player", QString::number(getPlayer(Figure::WHITE).getState()));
	writer.writeTextElement("black-player", QString::number(getPlayer(Figure::BLACK).getState()));
	writer.writeEndElement();

	// Write history to xml structure.
	writer.writeStartElement("history");
	writer.writeAttribute("index", QString::number(m_history->getIndex()));
	// Write all moves from history.
	for (int i = 1; i < m_history->count(); i++) {
	    writer.writeStartElement("item");
	    writer.writeAttribute("ordinal-number",
				  QString::number(m_history->at(i)->getOrdinalNumber()));
	    writer.writeAttribute("moves-without-jumps",
				  QString::number(m_history->at(i)->getMovesWithoutJump()));
	    // Write move
	    writer.writeStartElement("move");
	    writer.writeAttribute("from", m_history->at(i)->getMove()->getFrom().toString());
	    writer.writeAttribute("to", m_history->at(i)->getMove()->getTo().toString());
	    writer.writeAttribute("type-of-figure", QString::number(m_history->at(i)->getMove()->getFigureType()));
	    writer.writeAttribute("promoted", QString::number(m_history->at(i)->getMove()->getPromoted()));
	    // Write jumped figures.
	    writer.writeStartElement("jumped-figures");
	    foreach (JumpedFigure figure, m_history->at(i)->getMove()->getJumpedFigures()) {
		writer.writeStartElement("figure");
		writer.writeAttribute("location", figure.first.toString());
		writer.writeAttribute("type-of-figure", QString::number(figure.second));
		writer.writeEndElement();
	    }
	    writer.writeEndElement();
	    writer.writeEndElement();
	    writer.writeEndElement();
	}
	writer.writeEndElement();

	// Finalize xml document.
	writer.writeEndElement();
	writer.writeEndDocument();

	// Append MD5 checksum as the last line of the xml structure.
	//array.append(QCryptographicHash::hash(QByteArray().append(array), QCryptographicHash::Md5).toHex());

	// Write encrypted snapshot of actual game into file.
	QTextStream out(&file);
	// Hash for encrypting game files.
	// 0x3f5e300c8d2e95d4
	SimpleCrypt crypto(Q_UINT64_C(0x3f5e300c8d2e95d4));
	crypto.setCompressionMode(SimpleCrypt::CompressionAlways);
	crypto.setIntegrityProtectionMode(SimpleCrypt::ProtectionHash);

	out << crypto.encryptToString(array);
	//out << array;
	out.flush();
	file.close();
	//qDebug() << array;
	m_isSaved = true;
	return true;
    }
    else {
	return false;
    }
    /*
    QByteArray result = crypto.encryptToByteArray(array);
    QByteArray decrypted = crypto.decryptToByteArray(result);
    qDebug() << result;
    qDebug() << decrypted;
    */
}

bool Game::isSaved() const {
    return m_isSaved;
}

bool Game::loadGame(const QString &file_name) {
    // Open file with saved game.
    QFile file(file_name);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text) == true) {
	// Decrypt contents of saved game and then close the file.
	SimpleCrypt crypto(Q_UINT64_C(0x3f5e300c8d2e95d4));
	crypto.setCompressionMode(SimpleCrypt::CompressionAlways);
	crypto.setIntegrityProtectionMode(SimpleCrypt::ProtectionHash);

	QString array = crypto.decryptToString(QString(file.readAll()));
	file.close();

	qDebug() << array;

	// If file is too small (it is corrupted or decryption failed, then
	// return from this function.
	if (array.size() < APP_SAVE_LIMIT) {
	    return false;
	}

	int tmp_starting_player, max_moves_without_jump;
	Player::State white_player, black_player;

	QList<QPair<Move, int> > tmp_his_items;

	QDomDocument reader;
	reader.setContent(array);

	// We do not allow usage of saved games from previous QGothic builds.
	QDomNodeList list_saved = reader.elementsByTagName("saved-game");
	if (list_saved.at(0).toElement().attribute("version") != APP_VERSION) {
	    return false;
	}

	QDomNodeList list_max = reader.elementsByTagName("max-moves-without-jump");
	max_moves_without_jump = list_max.at(0).toElement().text().toInt();

	QDomNodeList list_starting = reader.elementsByTagName("starting-player");
	tmp_starting_player = list_starting.at(0).toElement().text().toInt();

	QDomNodeList list_white_pl = reader.elementsByTagName("white-player");
	white_player = static_cast<Player::State>(list_white_pl.at(0).toElement().text().toInt());

	QDomNodeList list_black_pl = reader.elementsByTagName("black-player");
	black_player = static_cast<Player::State>(list_black_pl.at(0).toElement().text().toInt());

	// Add each item of history to temp.
	QDomNodeList list_items = reader.elementsByTagName("item");
	for (int i = 0; i < list_items.size(); i++) {
	    QPair<Move, int> new_move;
	    // Set moves without jumps for this move.
	    new_move.second = list_items.at(i).toElement().attribute("moves-without-jumps").toInt();

	    QDomElement move = list_items.at(i).childNodes().at(0).toElement();
	    new_move.first.setFrom(Location::fromString(move.attribute("from")));
	    new_move.first.setTo(Location::fromString(move.attribute("to")));
	    new_move.first.setFigureType(static_cast<Figure::Type>(move.attribute("type-of-figure").toInt()));
	    new_move.first.setPromoted(static_cast<bool>(move.attribute("promoted").toInt()));

	    QDomNodeList list_jumped_figures = move.elementsByTagName("figure");
	    for (int i = 0; i < list_jumped_figures.size(); i++) {
		QDomElement new_figure = list_jumped_figures.at(i).toElement();
		Location loc = Location::fromString(new_figure.attribute("location"));
		Figure::Type typ = static_cast<Figure::Type>(new_figure.attribute("type-of-figure").toInt());
		new_move.first.addJumpedFigure(loc, typ);
	    }
	    tmp_his_items.append(new_move);
	}

	// Set data into game and start new game.
	newGame();

	GSettings::setValue(SET_GAME, "white_player_dif", white_player);
	GSettings::setValue(SET_GAME, "black_player_dif", black_player);

	m_players.clear();
	m_players << Player(white_player, Figure::WHITE);
	m_players << Player(black_player, Figure::BLACK);

	foreach (Item move, tmp_his_items) {
	    m_history->addMove(move.first, move.second);
	}


	GSettings::setValue(SET_GAME, "starting_player", tmp_starting_player);
	GSettings::setValue(SET_GAME, "max_moves_without_jump", max_moves_without_jump);

	m_board->setMaxMovesNoJump(max_moves_without_jump);
	setStartingPlayer(tmp_starting_player);
	m_currentPlayer = tmp_starting_player;
	m_history->setIndex(0);
	return true;
    }
    else {
	return false;
    }
}

void Game::newGame() {
    m_board->setInitialPositions();
    m_board->setActualMovesNoJump(0);
    m_board->setMaxMovesNoJump(GSettings::value(SET_GAME, "max_moves_without_jump", 60).toInt());
    m_board->setState(Board::ORDINARY);
    m_isSaved = false;
    m_history->clear();

    setState(Game::PAUSED);
    int current_player = GSettings::value(SET_GAME, "starting_player", 0).toInt();
    m_currentPlayer = current_player == 0 || current_player == 1 ?
			  current_player :
			  0;

    setStartingPlayer(m_currentPlayer);

    emit boardChanged();
    emit playersSwapped();
}

void Game::fakeHumanMove() {
    if (Referee::getMoves(getCurrentPlayer().getColor(), *m_board).size() == 0) {
	makeMove(Move::getInvalidMove());
    }
}

void Game::computerMove() {
    if (m_board->getState() == Board::ORDINARY) {
	emit moveSearchStarted();
	m_generator->searchMove(getCurrentPlayer(), m_board);
    }
}

void Game::informAboutHistory() {
    m_isSaved = false;

    emit canUndo(m_history->canUndo());
    emit canRedo(m_history->canRedo());
}

void Game::makeMove(const Move &move) {
    emit moveSearchFinished();

    if ((m_state == Game::RUNNING && getCurrentPlayer().getState() == Player::HUMAN) || m_board->getState() == Board::ORDINARY) {
	m_isSaved = false;

	m_board->makeMove(move);
	m_history->addMove(move, m_board->getActualMovesNoJump());
    }

    emit boardChanged();
    swapPlayer();
}

Board *Game::getBoard() const {
    return m_board;
}

void Game::setState(Game::State state) {
    m_state = state;
    emit stateChanged(m_state == Game::RUNNING);
}

Player Game::getCurrentPlayer() const {
    return m_players[m_currentPlayer];
}

void Game::undo(bool repaint) {
    if (m_history->canUndo()) {
	HistoryItem *item_to_undo = m_history->at(m_history->getIndex());

	m_board->setActualMovesNoJump(m_history->at(m_history->getIndex()-1)->getMovesWithoutJump());
	m_board->makeInverseMove(*item_to_undo->getMove());
	m_history->decrement();

	m_currentPlayer = m_currentPlayer == 0 ? 1 : 0;
	if (repaint) {
	    emit boardChanged();
	    emit playersSwapped();
	}
	informAboutHistory();
    }
    qDebug() << "Is undo available?" << m_history->canUndo();
}

void Game::redo(bool repaint) {
    if (m_history->canRedo()) {
	m_history->increment();
	HistoryItem *item_to_undo = m_history->at(m_history->getIndex());
	m_board->makeMove(*item_to_undo->getMove());
	m_board->setActualMovesNoJump(item_to_undo->getMovesWithoutJump());

	m_currentPlayer = m_currentPlayer == 0 ? 1 : 0;
	if (repaint) {
	    emit boardChanged();
	    emit playersSwapped();
	}
	informAboutHistory();
    }
    qDebug() << "Is redo available?" << m_history->canRedo();
}

int Game::getStartingPlayer() const {
    return m_startingPlayer;
}

void Game::setStartingPlayer(int starting_player) {
    m_startingPlayer = starting_player;
    emit initialPlayerChanged(m_startingPlayer);
}

History *Game::getHistory() const {
    return m_history;
}

Generator *Game::getGenerator() const {
    return m_generator;
}

Game::State Game::getState() const {
    return m_state;
}

void Game::swapPlayer() {
    m_currentPlayer = m_currentPlayer == 0 ? 1 : 0;
    qDebug() << "Current player: " << m_currentPlayer;
    Board::State state =  m_board->getState();
    emit playersSwapped();
    if (state != Board::ORDINARY) {
	emit gameFinished(state);
    }
    if (m_state == Game::RUNNING) {
	if (getCurrentPlayer().getState() != Player::HUMAN) {
	    computerMove();
	}
	else {
	    fakeHumanMove();
	}
    }
}

Player Game::getPlayer(Figure::Color color) const {
    switch (color) {
	case Figure::WHITE:
	    return m_players[0];
	case Figure::BLACK:
	    return m_players[1];
	default:
	    return m_players[0];
    }
}

void Game::setPlayer(Player player) {
    switch (player.getColor()) {
	case Figure::WHITE:
	    m_players[0] = player;
	    break;
	case Figure::BLACK:
	    m_players[1] = player;
	    break;
	default:
	    break;
    }
}
