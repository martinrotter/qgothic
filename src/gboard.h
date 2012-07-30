#ifndef GBOARD_H
#define GBOARD_H

#include "game.h"
#include "move.h"

#include <QWidget>


class GBoard : public QWidget {
	Q_OBJECT

    public:
	explicit GBoard(QWidget *parent = 0);
	void setGame(Game *game);


    protected:
	void mouseMoveEvent(QMouseEvent *e);
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e);
	void mousePressEvent(QMouseEvent *e);

	Location locationFromPoint(const QPoint &point) const;

    private:
	Game *m_game;
	int m_offset;
	bool m_is_piece_selected;
	Location m_selected_piece;
	QList<Location> m_available_pieces;
	QList<Move> m_available_moves;

};

#endif // GBOARD_H
