#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "board.h"
#include "history.h"
#include "generator.h"

#include <QList>
#include <QDebug>


/*!
 * \brief QGothic Game top-level representation.
 */
class Game : public QObject {
	Q_OBJECT

    public:
	/*!
	 * \brief Holds all possible States of the Game.
	 */
	enum State {
	    RUNNING,
	    PAUSED
	};

	/*!
	 * \brief Holds all possible States of the saving Game procedure.
	 */
	enum SaveState {
	    SAVED,
	    SAVE_BUT_CANCELLED,
	    DONT_SAVED,
	    CANCELLED
	};

	/*!
	 * \brief Constructs new Game.
	 * \param parent Pointer to the super QObject, this is used for automatic memory-management.
	 */
	Game(QObject *parent = 0);
	~Game();

	/*!
	 * \brief Sets players for Game.
	 * \param player Player to set.
	 */
	void setPlayer(Player player);
	Board *getBoard() const;

	/*!
	 * \brief Gets Player according to color.
	 * \param color Color of Player.
	 * \return Method returns Player with given color.
	 */
	Player getPlayer(Figure::Color color) const;

	/*!
	 * \brief Swaps players on turn.
	 *
	 * Switches turn to next Player.
	 */
	void swapPlayer();
	void setState(Game::State state);
	Game::State getState() const;
	History *getHistory() const;

	void setStartingPlayer(int starting_player);
	int getStartingPlayer() const;

	/*!
	 * \brief Indicates if Game was saved.
	 *
	 * \note This gets restored if any player makes Move.
	 * \return Method returns true if Game was saved recently, otherwise false is returned.
	 */
	bool isSaved() const;

	/*!
	 * \brief Starts new Game.
	 *
	 * When new Game is started, starting Player is loaded from settings.
	 * Also levels of both Players are loaded, History gets cleard and Board re-initialized.
	 */
	void newGame();

	/*!
	 * \brief Saves game.
	 * \param file_name Name of file, which will hold saved game.
	 * \return Method returns true, if game was saved with success.
	 */
	bool saveGame(const QString &file_name);


	/*!
	 * \brief Loads game.
	 * \param file_name Name of file, which holds saved game.
	 * \return Method returns true, if game was loaded with success.
	 */
	bool loadGame(const QString &file_name);

	/*!
	 * \brief Gets active Player.
	 * \return Method returns turning Player.
	 */
	Player getCurrentPlayer() const;

	/*!
	 * \brief Forces computer Player to start finding his next Move.
	 */
	void computerMove();

	/*!
	 * \brief Fakes human Player Moves.
	 *
	 * This method is important when there are no Moves for human Player to make.
	 * "Empty" Move is made and opponents can continue in Game.
	 */
	void fakeHumanMove();


    public slots:
	/*!
	 * \brief Undoes the Game.
	 * \param repaint If true, then GBoard is repainted after this undo.
	 */
	void undo(bool repaint = true);

	/*!
	 * \brief Redoes the Game.
	 * \param repaint If true, then GBoard is repainted after this redo.
	 */
	void redo(bool repaint = true);

	/*!
	 * \brief Makes the move and proceeds the Game.
	 * \param move Move to make.
	 */
	void makeMove(const Move &move);

    protected slots:
	/*!
	 * \brief Informs connected Views and indicators about History changes.
	 */
	void informAboutHistory();

    private:
	QList<Player> m_players;
	Board *m_board;
	History *m_history;
	Generator *m_generator;
	Game::State m_state;
	int m_currentPlayer;
	int m_startingPlayer;
	bool m_isSaved;

    signals:
	/*!
	 * \brief Emitted when Game has ended "naturally". Not emitted when game "finished" by redo.
	 */
	void gameFinished(Board::State);

	/*!
	 * \brief Emitted, when Board changed and GBoard should reflect the changes.
	 */
	void boardChanged();
	void stateChanged(bool);
	void initialPlayerChanged(int);

	/*!
	 * \brief Emitted when players are swapped.
	 */
	void playersSwapped();
	void moveSearchStarted();
	void moveSearchFinished();
	void canRedo(bool);
	void canUndo(bool);

};

#endif // GAME_H
