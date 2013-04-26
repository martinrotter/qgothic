#include "gmainwindow.h"
#include "gsettingsdialog.h"
#include "gsettings.h"
#include "gdocdialog.h"
#include "ginterface.h"
#include "gaboutdialog.h"
#include "move.h"
#include "intelligence.h"
#include "definitions.h"
#include "historymodel.h"

#include <cmath>

#include <QDebug>
#include <QMessageBox>
#include <QProgressBar>
#include <QProgressDialog>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMimeData>


GMainWindow::GMainWindow(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::GMainWindow) {
    m_ui->setupUi(this);

    initialiseStatusBar();

    initialiseMembers();
    initialiseHistoryView();
    createConnections();
    setupWindow();

    updateTable(false);
    m_game->newGame();
}

GMainWindow::~GMainWindow() {
    delete m_ui;

    delete m_game;
    delete m_historyModel;

    delete m_labelStatusTurn;
    delete m_labelStatusState;
    delete m_barStatusTurn;
}

void GMainWindow::initialiseHistoryView() {
    m_historyModel = new HistoryModel(m_game->getHistory(),
				      m_ui->m_historyView);
    m_ui->m_historyView->setCurrentModel(m_historyModel);
}

void GMainWindow::initialiseStatusBar() {
    m_labelStatusState = new QLabel(this);
    m_labelStatusTurn = new QLabel(this);
    m_barStatusTurn = new QProgressBar(this);

    m_labelStatusState->setStyleSheet("QLabel {margin-right: 10px;}");
    m_labelStatusState->setAlignment(Qt::AlignRight);

    m_barStatusTurn->setMinimum(0);
    m_barStatusTurn->setMaximum(0);
    m_barStatusTurn->setValue(0);
    m_barStatusTurn->setVisible(false);
    m_barStatusTurn->setTextVisible(false);

    m_ui->m_statusBar->addWidget(m_barStatusTurn);
    m_ui->m_statusBar->addWidget(m_labelStatusTurn);
    m_ui->m_statusBar->addWidget(m_labelStatusState,1);
}

void GMainWindow::keyPressEvent(QKeyEvent *e) {
    QMainWindow::keyPressEvent(e);

    if (e->modifiers() == Qt::NoModifier) {
	switch (e->key()) {
	    case Qt::Key_Left:
		pauseGame();
		m_game->undo();
		break;
	    case Qt::Key_Right:
		pauseGame();
		m_game->redo();
	    default:
		break;
	}
    }

}

void GMainWindow::dragEnterEvent(QDragEnterEvent *e) {
    e->acceptProposedAction();
}

void GMainWindow::dropEvent(QDropEvent *e) {
    if (e->mimeData()->hasUrls() == true) {
	e->acceptProposedAction();
	loadFromFile(e->mimeData()->urls().at(0).toString().split("///").at(1));
    }
}

void GMainWindow::closeEvent(QCloseEvent *e) {
    QMainWindow::closeEvent(e);
    pauseGame();

    if (checkIfSaved() == Game::CANCELLED) {
	e->ignore();
    }
    else {
	m_game->getGenerator()->exit(EXIT_SUCCESS);
	GSettings::checkSettings();
	qDebug() << "QGothic is exiting.";
	qDebug() << "Leaving thread with"
		 << Q_FUNC_INFO << "and code"
		 << QThread::currentThreadId() << ".";

	e->accept();
    }
}

void GMainWindow::initialiseMembers() {
    //setFixedSize(width(), height());
    GInterface::centerWidgetOnScreen(this);

    m_game = new Game(this);

    m_ui->m_gboard->setGame(m_game);
    m_ui->m_gboard->repaint();
}

void GMainWindow::switchControlBox(Qt::Alignment alignment) {
    switch (alignment) {
	case Qt::AlignRight:
	    m_ui->horizontalLayout_2->removeWidget(m_ui->m_gboard);
	    m_ui->horizontalLayout_2->removeWidget(m_ui->widget);

	    m_ui->horizontalLayout_2->addWidget(m_ui->m_gboard);
	    m_ui->horizontalLayout_2->addWidget(m_ui->widget);
	    break;
	case Qt::AlignLeft:
	    m_ui->horizontalLayout_2->removeWidget(m_ui->m_gboard);
	    m_ui->horizontalLayout_2->removeWidget(m_ui->widget);

	    m_ui->horizontalLayout_2->addWidget(m_ui->widget);
	    m_ui->horizontalLayout_2->addWidget(m_ui->m_gboard);
	    break;
	default:
	    break;
    }
}

void GMainWindow::setupWindow() {
#ifdef Q_WS_MAC
    // Allow blending of toolbar into titlebar on Mac OS
    setUnifiedTitleAndToolBarOnMac(true);
#endif
    // Switch controlbox to left or right side.
    switchControlBox(static_cast<Qt::Alignment>(GSettings::value(SET_APPEAR ,"controlbox_alignment",
								 Qt::AlignLeft).toInt()));
}

void GMainWindow::createConnections() {
    connect(m_ui->m_actionNew, SIGNAL(triggered()), this, SLOT(newGame()));
    connect(m_ui->m_actionLoad, SIGNAL(triggered()), this, SLOT(load()));
    connect(m_ui->m_actionSave, SIGNAL(triggered()), this, SLOT(save()));
    connect(m_ui->m_actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    connect(m_ui->m_actionSettings, SIGNAL(triggered()), this, SLOT(configuration()));
    connect(m_ui->m_actionBestMove, SIGNAL(triggered()), this, SLOT(adviseMove()));

    connect(m_ui->m_actionBrowseHelp, SIGNAL(triggered()), this, SLOT(guideDocumentation()));
    connect(m_ui->m_actionAboutQGothic, SIGNAL(triggered()), this, SLOT(about()));

    connect(m_ui->m_buttonPlayPause, SIGNAL(toggled(bool)), this, SLOT(controlGame(bool)));
    connect(m_ui->m_buttonUndo, SIGNAL(clicked()), this, SLOT(pauseGame()));
    connect(m_ui->m_buttonUndo, SIGNAL(clicked()), m_game, SLOT(undo()));
    connect(m_ui->m_buttonRedo, SIGNAL(clicked()), this, SLOT(pauseGame()));
    connect(m_ui->m_buttonRedo, SIGNAL(clicked()), m_game, SLOT(redo()));

    connect(m_ui->m_historyView, SIGNAL(currentRowChanged(int,int)),
	    this, SLOT(moveInGame(int,int)));

    connect(m_game->getGenerator(), SIGNAL(countOfCalls(int)), m_barStatusTurn, SLOT(setMaximum(int)));
    connect(m_game->getGenerator(), SIGNAL(rankOfCall(int)), m_barStatusTurn, SLOT(setValue(int)));;

    connect(m_game, SIGNAL(gameFinished(Board::State)), this, SLOT(noticeAboutFinish(Board::State)));
    connect(m_game, SIGNAL(initialPlayerChanged(int)),
	    m_historyModel, SLOT(setInitialPlayerHeader(int)));
    connect(m_game->getHistory(), SIGNAL(changed(int)),
	    m_ui->m_historyView, SLOT(setCurrentRow(int)));
    connect(m_game, SIGNAL(playersSwapped()), this, SLOT(updateTable()));
    connect(m_game, SIGNAL(moveSearchStarted()), this, SLOT(moveStart()));
    connect(m_game, SIGNAL(moveSearchFinished()), this, SLOT(moveEnd()));
    connect(m_game, SIGNAL(stateChanged(bool)),
	    m_ui->m_buttonPlayPause, SLOT(setChecked(bool)));
    connect(m_game, SIGNAL(canUndo(bool)), m_ui->m_buttonUndo, SLOT(setEnabled(bool)));
    connect(m_game, SIGNAL(canRedo(bool)), m_ui->m_buttonRedo, SLOT(setEnabled(bool)));
    connect(m_game, SIGNAL(boardChanged()), m_ui->m_gboard, SLOT(repaint()));
}

void GMainWindow::configuration() {
    pauseGame();
    GSettingsDialog dialog(m_game, this);
    dialog.exec();
    setupWindow();
    updateTable(false);
}

Game::SaveState GMainWindow::checkIfSaved() {
    if (m_game->isSaved() == true) {
	return Game::SAVED;
    }

    Game::SaveState state;
    do {
	QMessageBox msg_box;
	msg_box.setWindowTitle(tr("Save Your Game"));
	msg_box.setIcon(QMessageBox::Information);
	msg_box.setText(tr("This game has not been saved."));
	msg_box.setInformativeText(tr("Do you want to save or discard your game?"));
	msg_box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	msg_box.setDefaultButton(QMessageBox::Save);
	int ret = msg_box.exec();

	switch (ret) {
	    case QMessageBox::Save:
		if (save() == false) {
		    state = Game::SAVE_BUT_CANCELLED;
		}
		else {
		    state = Game::SAVED;
		}
		break;
	    case QMessageBox::Discard:
		state = Game::DONT_SAVED;
		break;
	    default:
		state = Game::CANCELLED;
		break;
	}
    }
    while (state == Game::SAVE_BUT_CANCELLED);

    return state;
}

void GMainWindow::noticeAboutFinish(Board::State state) {
    switch (state) {
	case Board::DRAW:
	    QMessageBox::information(this, tr("Game Has Ended"),
				     tr("Game has ended in a draw."));
	    break;
	case Board::BLACK_WON:
	    QMessageBox::information(this, tr("Game Has Ended"),
				     tr("Black player has won this game."));
	    break;
	case Board::WHITE_WON:
	    QMessageBox::information(this, tr("Game Has Ended"),
				     tr("White player has won this game."));
	    break;
	default:
	    break;
    }
}

void GMainWindow::moveInGame(int new_index, int previous_index) {
    pauseGame();

    // redo
    int how_many_times = abs(new_index-previous_index);
    //m_game->blockSignals(true);
    if (new_index > previous_index) {
	for (int i = 0; i < how_many_times; i++) {
	    m_game->redo(false);
	}
    }
    // undo
    else {
	for (int i = 0; i < how_many_times; i++) {
	    m_game->undo(false);
	}
    }
    //m_game->blockSignals(false);
    updateTable();
    m_ui->m_gboard->repaint();
}

void GMainWindow::adviseMoveResult(Move move) {
    if (move.isInvalid() == true) {
	QMessageBox::warning(this, tr("No Move Found"),
			     tr("There were found no available move for you at this phase of game."));
    }
    else {
	QMessageBox::information(this, tr("Move found"),
				 tr("Best move was found.\n\n%1").arg(move.toString()));
    }
    disconnect(m_game->getGenerator(), SIGNAL(moveForHumanFound(Move)), this, SLOT(adviseMoveResult(Move)));

    controlGame(true);
}

void GMainWindow::adviseMove() {
    QProgressDialog dialog(tr("Looking for best available move..."), tr("Cancel"), 0, 100, this);

    controlGame(false);

    connect(m_game->getGenerator(), SIGNAL(countOfCalls(int)), &dialog, SLOT(setMaximum(int)));
    connect(m_game->getGenerator(), SIGNAL(rankOfCall(int)), &dialog, SLOT(setValue(int)));
    connect(&dialog, SIGNAL(canceled()), m_game->getGenerator(), SLOT(cancel()));
    connect(m_game->getGenerator(), SIGNAL(moveForHumanFound(Move)), &dialog, SLOT(close()));
    connect(m_game->getGenerator(), SIGNAL(moveForHumanFound(Move)), this, SLOT(adviseMoveResult(Move)));

    //Intelligence::cancel(false);
    m_game->getGenerator()->searchMove(m_game->getCurrentPlayer(), *m_game->getBoard());

    qApp->processEvents();

    dialog.setWindowTitle(tr("Best Move Search"));
    dialog.setWindowModality(Qt::WindowModal);
    dialog.exec();
}

void GMainWindow::moveStart() {
    switch (m_game->getCurrentPlayer().getColor()) {
	case Figure::WHITE:
	    m_labelStatusTurn->setText(tr("White player is seeking the move."));
	    m_labelStatusTurn->setToolTip(tr("White player is searching for move. If game pauses now, it may take few seconds to abort current move search."));
	    break;
	case Figure::BLACK:
	    m_labelStatusTurn->setText(tr("Black player is seeking the move."));
	    m_labelStatusTurn->setToolTip(tr("Black player is searching for move. If game pauses now, it may take few seconds to abort current move search."));
	    break;
	default:
	    break;
    }
    m_barStatusTurn->setVisible(true);
    m_labelStatusTurn->setVisible(true);

    m_ui->m_buttonRedo->setEnabled(false);
    m_ui->m_buttonUndo->setEnabled(false);
    m_ui->m_historyView->setEnabled(false);
    m_ui->m_menuBar->setEnabled(false);
}

void GMainWindow::moveEnd() {
    m_barStatusTurn->setVisible(false);
    m_labelStatusTurn->setVisible(false);

    m_ui->m_buttonPlayPause->setEnabled(true);
    m_ui->m_buttonRedo->setEnabled(true);
    m_ui->m_buttonUndo->setEnabled(true);
    m_ui->m_historyView->setEnabled(true);
    m_ui->m_menuBar->setEnabled(true);
}

void GMainWindow::continueGame() {
    controlGame(true);
}

void GMainWindow::pauseGame() {
    controlGame(false);
}

void GMainWindow::controlGame(bool running) {
    if (running) {
	Intelligence::cancel(false);

	m_game->setState(Game::RUNNING);
	m_game->getHistory()->consolidate();
	m_labelStatusState->setText(tr("Running"));
	m_labelStatusState->setToolTip(tr("Game is running."));
	if (m_game->getCurrentPlayer().getState() != Player::HUMAN) {
	    m_game->computerMove();
	}
	else {
	    m_game->fakeHumanMove();
	}
    }
    else {
	// AI player is now thinking about moving. Cancel this thinking.
	if (m_game->getCurrentPlayer().getState() != Player::HUMAN && m_game->getState() == Game::RUNNING) {
	    m_ui->m_buttonPlayPause->setEnabled(false);
	    Intelligence::cancel(true);
	}
	m_labelStatusState->setText(tr("Paused"));
	m_labelStatusState->setToolTip(tr("Game is paused."));
	m_game->setState(Game::PAUSED);
	m_ui->m_gboard->clearSelections();
    }
    repaint();
}

void GMainWindow::updateTable(bool just_turning) {
    Board::State board_state = m_game->getBoard()->getState();

    if (board_state == Board::ORDINARY) {
	switch (m_game->getCurrentPlayer().getColor()) {
	    case Figure::BLACK:
		m_ui->m_labelWhiteTurns->setText("");
		m_ui->m_labelWhiteTurns->setToolTip("");
		m_ui->m_labelBlackTurns->setToolTip(tr("Black player has his turn."));
		m_ui->m_labelBlackTurns->setText(QString(GAM_PLAY_STYLE).arg("32",
									     GAM_TURNS, ""));
		break;
	    case Figure::WHITE:
		m_ui->m_labelWhiteTurns->setToolTip(tr("White player has his turn."));
		m_ui->m_labelWhiteTurns->setText(QString(GAM_PLAY_STYLE).arg("32",
									     GAM_TURNS, ""));
		m_ui->m_labelBlackTurns->setText("");
		m_ui->m_labelBlackTurns->setToolTip("");
		break;
	    default:
		break;
	}

	m_ui->m_buttonPlayPause->setEnabled(true);
    }
    else {
	m_ui->m_labelWhiteTurns->setToolTip("");
	m_ui->m_labelWhiteTurns->clear();
	m_ui->m_labelBlackTurns->clear();
	m_ui->m_labelBlackTurns->setToolTip("");

	m_ui->m_buttonPlayPause->setChecked(false);
	m_ui->m_buttonPlayPause->setEnabled(false);
    }

    m_ui->m_actionBestMove->setEnabled(m_game->getCurrentPlayer().getState() == Player::HUMAN &&
				       board_state == Board::ORDINARY);

    switch (board_state) {
	case Board::ORDINARY:
	    m_ui->m_labelNoJumpMovesWhite->setText(GAM_PLAY_STYLE.arg("16",
								      GAM_ORDINARY,
								      tr("in action")));
	    m_ui->m_labelNoJumpMovesBlack->setText(GAM_PLAY_STYLE.arg("16",
								      GAM_ORDINARY,
								      tr("in action")));
	    break;
	case Board::DRAW:
	    m_ui->m_labelNoJumpMovesWhite->setText(GAM_PLAY_STYLE.arg("16",
								      GAM_DRAW,
								      tr("tiing")));
	    m_ui->m_labelNoJumpMovesBlack->setText(GAM_PLAY_STYLE.arg("16",
								      GAM_DRAW,
								      tr("tiing")));
	    break;
	case Board::WHITE_WON:
	    m_ui->m_labelNoJumpMovesWhite->setText(GAM_PLAY_STYLE.arg("16",
								      GAM_WON,
								      tr("winning")));
	    m_ui->m_labelNoJumpMovesBlack->setText(GAM_PLAY_STYLE.arg("16",
								      GAM_LOST,
								      tr("losing")));
	    break;
	case Board::BLACK_WON:
	    m_ui->m_labelNoJumpMovesWhite->setText(GAM_PLAY_STYLE.arg("16",
								      GAM_LOST,
								      tr("losing")));
	    m_ui->m_labelNoJumpMovesBlack->setText(GAM_PLAY_STYLE.arg("16",
								      GAM_WON,
								      tr("winning")));
	    break;
	default:
	    break;
    }

    QString how_many_string = tr(" %n/%1 move(s) without jump",
				 "",
				 m_game->getBoard()->getActualMovesNoJump());
    how_many_string = how_many_string.arg(QString::number(m_game->getBoard()->getMaxMovesNoJump()));


    m_ui->m_labelJumps->setText(GAM_PLAY_STYLE.arg("16",
						   GAM_JUMPS,
						   how_many_string));

    int white_count = m_game->getBoard()->getLocations(Figure::WHITE).size();
    int black_count = m_game->getBoard()->getLocations(Figure::BLACK).size();
    m_ui->m_labelWhiteNumber->setText(GAM_PLAY_STYLE.arg("16",
							 GAM_PAWNS,
							 tr("%n figure(s)", "", white_count)));
    m_ui->m_labelBlackNumber->setText(GAM_PLAY_STYLE.arg("16",
							 GAM_PAWNS,
							 tr("%n figure(s)", "", black_count)));
    if (just_turning) {
	return;
    }

    QString states_texts[] = {
	tr("human"), tr("easy"), tr("medium"), tr("hard")
    };
    QString states_images[] = {
	GAM_HUMAN_IMG, GAM_EASY_IMG, GAM_MEDIUM_IMG, GAM_HARD_IMG
    };
    Player::State white = m_game->getPlayer(Figure::WHITE).getState();
    Player::State black = m_game->getPlayer(Figure::BLACK).getState();
    m_ui->m_labelWhiteType->setText(QString(GAM_PLAY_STYLE).arg("16",
								states_images[white],
								states_texts[white]));
    m_ui->m_labelBlackType->setText(QString(GAM_PLAY_STYLE).arg("16",
								states_images[black],
								states_texts[black]));
}

bool GMainWindow::save() {
    pauseGame();

    QString file_name = QFileDialog::getSaveFileName(this, tr("Save Game"),
						     QDir::homePath()+QDir::separator()+tr("my-saved-game.qgo"),
						     tr("QGothic Game File (*.qgo);;All Files (*.*)"));
    if (file_name.size() > 0) {
	if (m_game->saveGame(file_name) == false) {
	    QMessageBox::warning(this, tr("Error"), tr("Game couldn't be saved because target file is not writable or some kind of other error occured."
						       "\n\nFile: %1").arg(file_name));
	}
	else {
	    return true;
	}
    }
    return false;
}

void GMainWindow::loadFromFile(const QString &file_name) {
    if (file_name.size() > 0) {
	if (m_game->loadGame(file_name) == false) {
	    QMessageBox::warning(this, "Error", tr("Game couldn't be loaded because this file is not in valid format or was saved under another version of this application."
						   "\n\nFile: %1").arg(file_name));
	} else {
	    //m_game->blockSignals(true);
	    while (m_game->getHistory()->canRedo()) {
		m_game->redo(false);
	    }
	    //m_game->blockSignals(false);
	    updateTable(false);
	    m_ui->m_gboard->repaint();
	}
    }
}

void GMainWindow::load() {
    pauseGame();

    if (checkIfSaved() == Game::CANCELLED) {
	return;
    }

    QString file_name = QFileDialog::getOpenFileName(this, tr("Load Game"),
						     QDir::homePath(),
						     tr("QGothic Game File (*.qgo);;All Files (*.*)"));
    loadFromFile(file_name);
}

void GMainWindow::newGame() {
    pauseGame();

    if (checkIfSaved() == Game::CANCELLED) {
	return;
    }

    m_game->newGame();
}

void GMainWindow::about() {
    pauseGame();
    GAboutDialog(this).exec();
}

void GMainWindow::guideDocumentation() {
    pauseGame();
    GDocDialog(tr("User Guide"),
	       "qrc:/doc/user-guide.html",
	       this).exec();
}
