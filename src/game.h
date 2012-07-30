#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "board.h"
#include "history.h"
#include "generator.h"

#include <QList>
#include <QDebug>


class Game : public QObject {
	Q_OBJECT

    public:
	enum State {
	    RUNNING,
	    PAUSED
	};

	Game(QObject *parent = 0);
	~Game();

	void setPlayer(Player player);
	Board *getBoard() const;
	Generator *getGenerator() const;
	Player getPlayer(Figure::Color color) const;
	void swapPlayer();
	void setState(Game::State state);
	Game::State getState() const;
	History *getHistory() const;

	void newGame();
	void saveGame(const QDataStream &out);
	void loadGame(const QDataStream &in);

	Player getCurrentPlayer() const;
	void computerMove();


    public slots:
	void undo();
	void redo();

	void makeMove(const Move &move);
	void makeInverseMove(const Move &move);

    protected slots:
	void informAboutHistory();

    private:
	QList<Player> m_players;
	Board *m_board;
	History *m_history;
	Generator *m_generator;
	Game::State m_state;
	int m_current_player;

    signals:
	//void humanTurnsWhenPaused();
	void boardChanged();
	void stateChanged(bool);
	void playersSwapped();
	void moveSearchStarted();
	void moveSearchFinished();
	void canRedo(bool);
	void canUndo(bool);

};

#endif // GAME_H
