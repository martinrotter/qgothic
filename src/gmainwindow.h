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

class GMainWindow : public QMainWindow {
	Q_OBJECT
	
    public:
	explicit GMainWindow(QWidget *parent = 0);
	~GMainWindow();

    protected:
	void closeEvent(QCloseEvent *e);
	void keyPressEvent(QKeyEvent *e);

	void initialiseMembers();
	void initialiseStatusBar();
	void initialiseHistoryView();
	void createConnections();
	void setupWindow();

	void switchControlBox(Qt::Alignment alignment);

    protected slots:
	void referenceDocumentation();
	void quit();
	void about();
	void save();
	void load();
	void newGame();
	void configuration();
	void updateTable(bool just_turning = true);
	void controlGame(bool running = false);
	void continueGame();
	void pauseGame();
	void moveStart();
	void moveEnd();
	void adviseMove();
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
