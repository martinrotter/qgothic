#include "gboard.h"
#include "referee.h"

#include <QDebug>
#include <QStyleOption>
#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>


GBoard::GBoard(QWidget *parent) : QWidget(parent),
    m_game(NULL), m_offset(25), m_is_piece_selected(false) {
    setBackgroundRole(QPalette::Light);
    setAutoFillBackground(true);
}

void GBoard::setGame(Game *game) {
    m_game = game;
}

void GBoard::mouseMoveEvent(QMouseEvent *e) {
    QWidget::mouseMoveEvent(e);
}

Location GBoard::locationFromPoint(const QPoint &point) const {
    int size = m_game->getBoard()->getSize();
    int block_size = (width()-2*m_offset)/size;

    // User clicked outside of actual board, he clicked on border.
    if (point.x() < m_offset || point.x() > size*block_size+m_offset ||
	    point.y() < m_offset || point.y() > size*block_size+m_offset) {
	return Location(-1, -1);
    }
    int x = point.x();
    x -= m_offset;
    x /= block_size;
    int y = point.y();
    y -= m_offset;
    y /= block_size;
    y = size-y-1;
    return Location(x, y);
}

void GBoard::clearSelections() {
    m_is_piece_selected = false;
    m_available_pieces.clear();
    m_available_moves.clear();
    repaint();
}

void GBoard::mousePressEvent(QMouseEvent *e) {
    QWidget::mousePressEvent(e);

    // No human player has his turn, so he/she can't make board selections or moves.
    if (m_game->getCurrentPlayer().getState() != Player::HUMAN ||
	    m_game->getBoard()->getState() != Board::ORDINARY ||
	    m_game->getState() != Game::RUNNING) {
	return;
    }

    // No piece is selected, we select piece and display all posibble moves.
    Location clicked_location = locationFromPoint(e->pos());
    if (m_is_piece_selected == false) {
	// User clicked outside of the actual board.
	if (clicked_location.isInvalid()) {
	    return;
	}

	m_selected_piece = clicked_location;

	// je třeba zjistit kterymy figurkami jde tahnout
	// na vykreslení tahnutelných figurek
	QList<Move> available_moves = Referee::getMoves(Figure::getColorByType((*m_game->getBoard())(m_selected_piece)),
							*m_game->getBoard());
	m_available_pieces.clear();
	foreach (Move move, available_moves) {
	    m_available_pieces.append(move.getFrom());
	}

	//QList<Move> available_moves = Referee::getBestMoves(Referee::getAllMoves(m_selected_piece, *m_game->getBoard()));


	// do m_available_moves dej tahy ktere jdou udělat z vybranou figurkou
	foreach (Move move, available_moves) {
	    if (move.getFrom() != clicked_location) {
		available_moves.removeOne(move);
	    }
	}

	m_available_moves.clear();
	m_available_moves.append(available_moves);
	m_is_piece_selected = true;
    }
    // We have piece selected already, potential moves are displayed.
    else {
	if (clicked_location.isInvalid()) {
	    return;
	}

	// klikli jsme tam kde už je vybraný kámen
	if (clicked_location == m_selected_piece) {
	    bool ends_in_same_place = false;
	    // pokud nějaký tah končí na stejné pozici kde je kamen
	    // tak nahodíme ends_in_same_place na true
	    foreach (Move move, m_available_moves) {
		if (clicked_location == move.getTo()) {
		    ends_in_same_place = true;
		}
	    }
	    // pokud nekončí tah na stejné pozici, tak odznačíme aktuání kámen
	    if (ends_in_same_place == false) {
		m_is_piece_selected = false;
		m_available_moves.clear();
		repaint();
		return;
	    }
	}
/*
	QList<Move> available_moves = Referee::getBestMoves(Referee::getAllMoves(m_selected_piece, *m_game->getBoard()));
	foreach (Move move, available_moves) {
	    if (m_available_pieces.contains(move.getFrom()) == false) {
		available_moves.removeOne(move);
	    }
	}
*/


	m_available_pieces.clear();
	m_is_piece_selected = false;


	//qDebug() << "pocet moznych tahu: " << available_moves.size();
	foreach (Move move, m_available_moves) {	    // našli jsme vhodný tah
	    // ošetřovat, že když je na tahu člověk, tak může táhnout jen svoji barvou
	    // navíc je zde více možných rovnocenných tahů
	    if (clicked_location == move.getTo()) {
		m_game->makeMove(move);
		m_available_moves.clear();
		repaint();
		return;
	    }
	}

	m_available_moves.clear();
	mousePressEvent(e);
    }
    repaint();
}

void GBoard::paintEvent(QPaintEvent *e) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);
    p.setRenderHint(QPainter::HighQualityAntialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing, true);

    // Count all necessary variables.
    int size = m_game->getBoard()->getSize();
    int block_size = (width()-2*m_offset)/size;
    int x = m_offset + block_size*0.4, y = height() - m_offset*0.9;

    // Draw border.
    p.fillRect(0, 0, width(), height(), QPalette().window().color());

    p.setPen(QPen(Qt::black, 3.0));
    p.setBrush(QBrush(Qt::black));
    p.drawRect(m_offset, m_offset, size*block_size, size*block_size);

    // Draw labels.

    for (int i = 0; i < size; i++) {
	p.drawText(x, y, 10, 15, Qt::AlignCenter, QString(i+'A'));
	x += block_size;
    }

    x = block_size*0.2; y = height() - m_offset*2.3;
    for (int i = 0; i < size; i++) {
	p.drawText(x, y, 10, 15, Qt::AlignCenter, QString::number(i+1));
	y -= block_size;
    }

    x = m_offset; y = height() - m_offset - block_size;
    // Draw checkered cells.
    for (int i = 0; i < size; i++) {
	for (int j = 0; j < size; j++) {
	    if (!((i+j%2)%2)) {
		p.fillRect(x, y, block_size, block_size, QColor(150, 150, 150));
	    }
	    else {
		p.fillRect(x, y, block_size, block_size, Qt::white);
	    }
	    x += block_size;
	}
	y -= block_size;
	x = m_offset;
    }

    // Draw available moves including jumping sub-locations.
    foreach (Move move, m_available_moves) {
	int x = move.getTo().getX()*block_size + m_offset;
	int y = height()-m_offset-2-(move.getTo().getY()+1)*block_size;

	p.fillRect(x, y+2, block_size, block_size, QColor(0,255,0,150));

	foreach (JumpedFigure fig, move.getJumpedFigures()) {
	    int x = fig.first.getX()*block_size + m_offset;
	    int y = height()-m_offset-2-(fig.first.getY()+1)*block_size;

	    p.fillRect(x, y+2, block_size, block_size, Qt::red);
	}

    }

    // Draw available pieces.
    foreach (Location loc, m_available_pieces) {
	int x = loc.getX()*block_size + m_offset;
	int y = height()-m_offset-2-(loc.getY()+1)*block_size;

	p.fillRect(x, y+2, block_size, block_size, QColor(240, 140, 210, 150));
    }

    if (m_is_piece_selected) {
	int x = m_selected_piece.getX()*block_size + m_offset;
	int y = height()-m_offset-2-(m_selected_piece.getY()+1)*block_size;
	p.fillRect(x, y+2, block_size, block_size, QColor(0, 0, 0, 70));
    }

    x = m_offset; y = height() - m_offset - block_size;
    // Draw figures.
    for (int i = 0; i < size; i++) {
	for (int j = 0; j < size; j++) {
	    switch((*m_game->getBoard())(j, i)) {
		/*
		    p.drawImage(QRect(x, y, block_size, block_size),
				QImage(":/graphics/black-queen.png"));*/
		case Figure::BLACK_PAWN:
		    //p.set
		    p.setPen(QPen(Qt::black, 3.0));
		    p.setBrush(QBrush(Qt::black));
		    p.drawEllipse(x+block_size*0.1, y+block_size*0.1,
						      block_size*0.8, block_size*0.8);
		    break;
		case Figure::WHITE_PAWN:
		    //p.set
		    p.setPen(QPen(Qt::black, 3.0));
		    p.setBrush(QBrush(Qt::white));
		    p.drawEllipse(x+block_size*0.1, y+block_size*0.1,
				  block_size*0.8, block_size*0.8);
		    break;
		case Figure::BLACK_QUEEN:
		    //p.set

		    p.setPen(QPen(Qt::black, 3.0));
		    p.setBrush(QBrush(Qt::black));
		    p.drawEllipse(x+block_size*0.1, y+block_size*0.1,
				  block_size*0.8, block_size*0.8);
		    p.setPen(QPen(Qt::white, 3.0));
		    p.drawEllipse(x+block_size*0.3, y+block_size*0.3,
				  block_size*0.4, block_size*0.4);

		    /*
		    p.drawImage(QRect(x, y, block_size, block_size),
				QImage(":/graphics/black-queen.png"));*/
		    break;
		case Figure::WHITE_QUEEN:
		    //p.set

		    p.setPen(QPen(Qt::black, 3.0));
		    p.setBrush(QBrush(Qt::white));
		    p.drawEllipse(x+block_size*0.1, y+block_size*0.1,
				  block_size*0.8, block_size*0.8);
		    p.drawEllipse(x+block_size*0.3, y+block_size*0.3,
				  block_size*0.4, block_size*0.4);
				  /*
		    p.drawImage(QRect(x, y, block_size, block_size),
				QImage(":/graphics/white-queen.png"));*/

		    break;
		default:
		    break;
	    }

	    //qDebug() << x << " " << y;
	    x += block_size;
	}
	y -= block_size;
	x = m_offset;
    }
    /*
    if (m_game->getState() == Game::PAUSED) {
	// Make board darker.
	p.fillRect(0, 0, width(), height(), QColor(0, 0, 0, 130));

	// Draw "GAME PAUSED" text.
	QFont font;
	font.setPointSize(40);
	p.setFont(font);
	p.setPen(QPen(QColor(120, 215, 230), 15));
	p.setBrush(QBrush(Qt::white));
	p.drawText(QRect(0, 0, width(), height()),
		   Qt::AlignCenter, tr("GAME PAUSED"));
    }
    */

    QWidget::paintEvent(e);
}

void GBoard::resizeEvent(QResizeEvent *e) {
    //qDebug() << "resize";
    int new_size = qMin(e->size().width(), e->size().height());
    resize(new_size, new_size);
}
