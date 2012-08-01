#include "gmainwindow.h"
#include "gsettingsdialog.h"
#include "gsettings.h"
#include "greferencedocdialog.h"
#include "ginterface.h"
#include "move.h"
#include "intelligence.h"
#include "definitions.h"

#include "historymodel.h"

#include <QDebug>
#include <QMessageBox>
#include <QProgressBar>
#include <QMouseEvent>
#include <QFileDialog>


GMainWindow::GMainWindow(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::GMainWindow) {
    m_ui->setupUi(this);

    initialiseStatusBar();

    initialiseMembers();
    initialiseHistoryView();
    createConnections();

    setupWindow();

    updateTable(false);
    newGame();
    pauseGame();
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
    m_barStatusTurn->setValue(-1);
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

void GMainWindow::closeEvent(QCloseEvent *e) {
    QMainWindow::closeEvent(e);
    quit();
}

// Initialises members of this class and prepares main window.
void GMainWindow::initialiseMembers() {
    setFixedSize(width(), height());
    GInterface::centerWidgetOnScreen(this);
    //setFixedSize(sizeHint().width(), sizeHint().height());

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
    connect(m_ui->m_actionQuit, SIGNAL(triggered()), this, SLOT(quit()));

    connect(m_ui->m_actionSettings, SIGNAL(triggered()), this, SLOT(configuration()));
    connect(m_ui->m_actionBestMove, SIGNAL(triggered()), this, SLOT(adviseMove()));

    connect(m_ui->m_actionReferenceDocs, SIGNAL(triggered()), this, SLOT(referenceDocumentation()));
    connect(m_ui->m_actionAboutQGothic, SIGNAL(triggered()), this, SLOT(about()));

    connect(m_ui->m_buttonPlayPause, SIGNAL(toggled(bool)), this, SLOT(controlGame(bool)));
    connect(m_ui->m_buttonUndo, SIGNAL(clicked()), this, SLOT(pauseGame()));
    connect(m_ui->m_buttonUndo, SIGNAL(clicked()), m_game, SLOT(undo()));
    connect(m_ui->m_buttonRedo, SIGNAL(clicked()), this, SLOT(pauseGame()));
    connect(m_ui->m_buttonRedo, SIGNAL(clicked()), m_game, SLOT(redo()));

    connect(m_ui->m_historyView, SIGNAL(currentRowChanged(int,int)),
	    this, SLOT(moveInGame(int,int)));

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

void GMainWindow::moveInGame(int new_index, int previous_index) {
    pauseGame();

    // redo
    int how_many_times = abs(new_index-previous_index);
    if (new_index > previous_index) {
	for (int i = 0; i < how_many_times; i++) {
	    m_game->redo();
	}
    }
    // undo
    else {
	for (int i = 0; i < how_many_times; i++) {
	    m_game->undo();
	}
    }
}

void GMainWindow::adviseMove() {
    if (m_game->getCurrentPlayer().getState() != Player::HUMAN) {
	QMessageBox::information(this, tr("Best move search"), tr("Non-human players cannot search for best move."));
    }
    else {
	Move move = Intelligence::computerMove(m_game->getCurrentPlayer(), *m_game->getBoard());
	QMessageBox::information(this, tr("Result"), tr("Your move was found successfully.\n"
							"%1-%2").arg(move.getFrom().toString(),
								     move.getTo().toString()));
    }
}

void GMainWindow::moveStart() {
    //qDebug() << "move start";
    switch (m_game->getCurrentPlayer().getColor()) {
	case Figure::WHITE:
	    m_labelStatusTurn->setText(tr("White player is seeking the move."));
	    m_labelStatusTurn->setToolTip(tr("White player is searching for move. If game pauses now, then this search is completed before."));
	    break;
	case Figure::BLACK:
	    m_labelStatusTurn->setText(tr("Black player is seeking the move."));
	    m_labelStatusTurn->setToolTip(tr("Black player is searching for move. If game pauses now, then this search is completed before."));
	    break;
	default:
	    break;
    }
    m_barStatusTurn->setVisible(true);
    m_labelStatusTurn->setVisible(true);

    m_ui->m_buttonRedo->setEnabled(false);
    m_ui->m_buttonUndo->setEnabled(false);
    m_ui->m_historyView->setEnabled(false);
    //m_bar->m_ui->m_labelPlaying->setText("Computer is looking for move.");
}

void GMainWindow::moveEnd() {
    //qDebug() << "move end";
    m_barStatusTurn->setVisible(false);
    m_labelStatusTurn->setVisible(false);

    m_ui->m_buttonRedo->setEnabled(true);
    m_ui->m_buttonUndo->setEnabled(true);
    m_ui->m_historyView->setEnabled(true);
}

void GMainWindow::continueGame() {
    controlGame(true);
}

void GMainWindow::pauseGame() {
    controlGame(false);
}

void GMainWindow::controlGame(bool running) {
    if (running) {
	m_game->setState(Game::RUNNING);
	m_game->getHistory()->consolidate();
	m_labelStatusState->setText(tr("Running"));
	m_labelStatusState->setToolTip(tr("Game is running."));
	if (m_game->getCurrentPlayer().getState() != Player::HUMAN) {
	    m_game->computerMove();
	}
    }
    else {
	m_labelStatusState->setText(tr("Paused"));
	m_labelStatusState->setToolTip(tr("Game is paused."));
	m_game->setState(Game::PAUSED);
	m_ui->m_gboard->clearSelections();
    }
    repaint();
}

void GMainWindow::updateTable(bool just_turning) {

    switch (m_game->getBoard()->getState()) {
	break;
	case Board::DRAW:
	    break;
	case Board::WHITE_WON:
	    break;
	case Board::BLACK_WON:
	    break;
	default:
	    break;
    }

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

    int white_count = m_game->getBoard()->getLocations(Figure::WHITE).size();
    int black_count = m_game->getBoard()->getLocations(Figure::BLACK).size();
    white_count == 1
	    ? m_ui->m_labelWhiteNumber->setText(tr("%1 figure").arg(QString::number(white_count)))
	    : m_ui->m_labelWhiteNumber->setText(tr("%1 figures").arg(QString::number(white_count)));
    black_count == 1
	    ? m_ui->m_labelBlackNumber->setText(tr("%1 figure").arg(QString::number(black_count)))
	    : m_ui->m_labelBlackNumber->setText(tr("%1 figures").arg(QString::number(black_count)));

    /*
    // Reset history view, so that all changes are reflected.
    m_ui->m_historyView->reset();
    m_ui->m_historyView->scrollToBottom();
    */

    if (just_turning) {
	return;
    }

    QString states_texts[] = {
	GAM_HUMAN, GAM_EASY, GAM_MEDIUM, GAM_HARD
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

void GMainWindow::save() {
    pauseGame();

    QString file_name = QFileDialog::getSaveFileName(this, tr("Save Game"),
						     QDir::homePath(),
						     APP_SAVE_FILTER);
    if (file_name.size() > 0) {
	if (m_game->saveGame(file_name) == false) {
	    QMessageBox::warning(this, WORD_ERROR, tr("Game couldn't be saved because target file is not writable or some kind of other error occured."
						      "\n\nFile: %1").arg(file_name));
	}
    }
}

void GMainWindow::load() {
    pauseGame();

    QString file_name = QFileDialog::getOpenFileName(this, tr("Load Game"),
						     QDir::homePath(),
						     APP_SAVE_FILTER);
    if (file_name.size() > 0) {
	if (m_game->loadGame(file_name) == false) {
	    QMessageBox::warning(this, WORD_ERROR, tr("Game couldn't be loaded because this file is not in valid format."
						      "\n\nFile: %1").arg(file_name));
	}
    }
}

void GMainWindow::newGame() {
    pauseGame();
    m_game->newGame();
}

void GMainWindow::about() {
    pauseGame();
    QMessageBox::about(this, APP_NAME,
		       APP_DESC + "\n\n"
		       + tr("Made by %1.").arg(APP_AUTHOR)
		       + "\n"
		       + tr("Version %1").arg(APP_VERSION));
}

void GMainWindow::quit() {
    pauseGame();
    GSettings::checkSettings();
    qDebug() << "exiting";
    QApplication::exit(EXIT_SUCCESS);
}

void GMainWindow::referenceDocumentation() {
    pauseGame();
    GReferenceDocDialog(this).exec();
}

