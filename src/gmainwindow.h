#ifndef GWINDOW_H
#define GWINDOW_H

#include "ui_gmainwindow.h"
#include "game.h"

#include <QMainWindow>


namespace Ui {
    class GWindow;
}

class QProgressBar;
class HistoryModel;

/*!
 * \brief Main QGothic application window.
 *
 * \ingroup Gui
 */
class GMainWindow : public QMainWindow {
	Q_OBJECT
	
    public:
	explicit GMainWindow(QWidget *parent = 0);
	~GMainWindow();

	// We need this to be public because of "opening" *.qgo files from OS.
	void loadFromFile(const QString &file_name);

    protected:
	void dragEnterEvent(QDragEnterEvent *e);
	void dropEvent(QDropEvent *e);
	void closeEvent(QCloseEvent *e);
	void keyPressEvent(QKeyEvent *e);

	void initialiseMembers();
	void initialiseStatusBar();
	void initialiseHistoryView();
	void createConnections();
	void setupWindow();

	void switchControlBox(Qt::Alignment alignment);
	Game::SaveState checkIfSaved();

    protected slots:
	void guideDocumentation();
	void about();
	bool save();
	void load();
	void newGame();
	void configuration();
	void noticeAboutFinish(Board::State state);
	void updateTable(bool just_turning = true);
	void controlGame(bool running = false);
	void continueGame();
	void pauseGame();
	void moveStart();
	void moveEnd();
	void adviseMove();
	void adviseMoveResult(Move move);
	void moveInGame(int new_index, int previous_index);
	
    private:
	Ui::GMainWindow *m_ui;
	Game *m_game;
	HistoryModel *m_historyModel;

	QLabel *m_labelStatusTurn;
	QLabel *m_labelStatusState;
	QProgressBar *m_barStatusTurn;

};

#endif // GWINDOW_H
