#include "game.h"
#include "historyitem.h"
#include "gsettings.h"
#include "definitions.h"

#include <QDebug>
#include <QObject>
#include <QMetaType>


Game::Game(QObject *parent) :
    QObject(parent), m_board(new Board()), m_history(new History(this)),
    m_generator(new Generator()), m_state(Game::PAUSED),
    m_current_player(GSettings::value(SET_GAME, "starting_player", 0).toInt()) {

    qRegisterMetaType<Move>("Move");
    connect(m_generator, SIGNAL(moveGenerated(Move)), this, SLOT(makeMove(Move)));
    connect(m_history, SIGNAL(changed(int)), this, SLOT(informAboutHistory()));

    m_players << Player(static_cast<Player::State>(GSettings::value(SET_GAME, "white_player_dif", 0).toInt()),
			Figure::WHITE);
    m_players << Player(static_cast<Player::State>(GSettings::value(SET_GAME, "black_player_dif", 2).toInt()),
			Figure::BLACK);
}

Game::~Game() {
    delete m_board;
    delete m_generator;
}

void Game::saveGame(const QDataStream &out) {

}

void Game::loadGame(const QDataStream &in) {

}

void Game::newGame() {
    m_board->setInitialPositions();
    m_board->setActualMovesNoJump(0);
    m_board->setState(Board::ORDINARY);
    m_history->clear();

    //informAboutHistory();
    setState(Game::PAUSED);
    m_current_player = GSettings::value(SET_GAME, "starting_player", 0).toInt();
    emit boardChanged();
    emit playersSwapped();
}

void Game::computerMove() {
    if (m_board->getState() == Board::ORDINARY) {
	emit moveSearchStarted();
	m_generator->searchMove(getCurrentPlayer(), m_board);
    }
    // kontroluj stav hry
    // proveď pc tah zavoláním generátoru, poté generátor vrátí signál
    // a na ten signál se provede makeMove
}

void Game::informAboutHistory() {
    emit canUndo(m_history->canUndo());
    emit canRedo(m_history->canRedo());
}

void Game::makeInverseMove(const Move &move) {
    m_board->makeInverseMove(move);
}

void Game::makeMove(const Move &move) {
    // zkontroluj stav hry
    emit moveSearchFinished();

    // pokud chce lidský hráč táhnout figurkou opačné barvy, tak to nejde
    // u počítače tato situace nenastane
    if (getCurrentPlayer().getState() == Player::HUMAN &&
	    Figure::getColorByType((*m_board)(move.getFrom())) != getCurrentPlayer().getColor()) {
	qDebug() << "you are not available to make this move";
	return;
    }

    // tady to running, táhnutí lidského hráče při pauznutí hry
    // We can make move only if game is running and it is not ended.
    if ((m_state == Game::RUNNING && getCurrentPlayer().getState() == Player::HUMAN) || m_board->getState() == Board::ORDINARY) {
	m_board->makeMove(move);
	m_history->addMove(move, m_board->getActualMovesNoJump());
    }
    //qDebug() << "stav desky " << m_board->getState();
    qDebug() << "pocet tahu beze skoku " << m_board->getActualMovesNoJump();
/*
    if (getCurrentPlayer().getState() == Player::HUMAN && m_state == Game::PAUSED) {
	emit humanTurnsWhenPaused();
    }
*/
    // aktualizuj historii
    // aktualizuj stav hry
    // prohoď hráče na tahu a volej computerMove když je na tahu PC
    emit boardChanged();
    swapPlayer();
}

Board *Game::getBoard() const {
    return m_board;
}

Generator *Game::getGenerator() const {
    return m_generator;
}

void Game::setState(Game::State state) {
    m_state = state;
    emit stateChanged(m_state == Game::RUNNING);
}

Player Game::getCurrentPlayer() const {
    return m_players[m_current_player];
}

void Game::undo() {
    if (m_history->canUndo()) {

	// nastavit pocet tahu beze skoku
	// vratit o jeden tah
	// to znamena vratit posledni tah v historii
	// pak dekrementovat historii
	HistoryItem *item_to_undo = m_history->at(m_history->getIndex());

	//qDebug() << "undo item: pocet skoku " << item_to_undo->getMovesWithoutJump();

	// tady jsem prohodil radky dva a dal -1 k preskocenym
	// tahum, protoze vychazelo vyssi cislo
	m_board->makeInverseMove(*item_to_undo->getMove());
	m_board->setActualMovesNoJump(m_history->at(m_history->getIndex()-1)->getMovesWithoutJump());
	m_history->decrement();
	emit boardChanged();

	m_current_player = m_current_player == 0 ? 1 : 0;
	qDebug() << "current player: " << m_current_player;
	qDebug() << "tahy beze skoku: " << m_board->getActualMovesNoJump();
	emit playersSwapped();
	informAboutHistory();
    }
    else {
	qDebug() << "cannot undo";
    }
}

void Game::redo() {
    if (m_history->canRedo()) {

	// nastavit pocet tahu beze skoku
	// vratit o jeden tah
	// to znamena vratit posledni tah v historii
	// pak dekrementovat historii
	m_history->increment();
	HistoryItem *item_to_undo = m_history->at(m_history->getIndex());
	m_board->makeMove(*item_to_undo->getMove());
	m_board->setActualMovesNoJump(item_to_undo->getMovesWithoutJump());

	emit boardChanged();

	m_current_player = m_current_player == 0 ? 1 : 0;
	qDebug() << "current player: " << m_current_player;
	qDebug() << "tahy beze skoku: " << m_board->getActualMovesNoJump();
	emit playersSwapped();
	informAboutHistory();
    }
    else {
	qDebug() << "cannot redo";
    }
}

History *Game::getHistory() const {
    return m_history;
}

Game::State Game::getState() const {
    return m_state;
}

void Game::swapPlayer() {
    m_current_player = m_current_player == 0 ? 1 : 0;
    qDebug() << "current player: " << m_current_player;
    emit playersSwapped();
    if (getCurrentPlayer().getState() != Player::HUMAN &&
	    m_state == Game::RUNNING) {
	computerMove();
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
