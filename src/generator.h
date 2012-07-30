#ifndef GENERATOR_H
#define GENERATOR_H

#include "move.h"
#include "player.h"

#include <QThread>


class Board;

class Generator : public QThread {
	Q_OBJECT

    public:
	explicit Generator(QObject *parent = 0);
	void run();
	bool isActive() const;

    signals:
	void moveGenerated(Move move);
	
    public slots:
	void cancel();
	void searchMove(Player applicant, Board *board);

    private:
	bool m_active;
	Player m_applicant;
	Board *m_board;
	
};

#endif // GENERATOR_H
