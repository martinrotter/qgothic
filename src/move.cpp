#include "move.h"

#include <QString>


Move::Move() :
    m_from(new Location()), m_to(new Location()), m_typeOfFigure(Figure::EMPTY),
    m_promoted(false), m_jumpedFigures(new QList<JumpedFigure>()) {
}

Move::Move(const Move &move) :
    m_from(new Location(move.getFrom())),
    m_to(new Location(move.getTo())),
    m_typeOfFigure(move.getFigureType()),
    m_promoted(move.getPromoted()), m_jumpedFigures(new QList<JumpedFigure>()) {
    addJumpedFigures(move.getJumpedFigures());
}

Move::~Move() {
    delete m_from;
    delete m_to;
    delete m_jumpedFigures;
}

QList<JumpedFigure> &Move::getJumpedFigures() const {
    return *m_jumpedFigures;
}

void Move::setFrom(const Location &location) {
    *m_from = location;
}

Location &Move::getFrom() const {
    return *m_from;
}

void Move::setTo(const Location &location) {
    *m_to = location;
}

Location &Move::getTo() const {
    return *m_to;
}

void Move::setPromoted(bool promoted) {
    m_promoted = promoted;
}

bool Move::getPromoted() const {
    return m_promoted;
}

void Move::addJumpedFigure(int x, int y, Figure::Type type_of_figure) {
    m_jumpedFigures->append(JumpedFigure(Location(x, y),
					 type_of_figure));
}

void Move::addJumpedFigure(const Location &location, Figure::Type type_of_figure) {
    m_jumpedFigures->append(JumpedFigure(Location(location),
					 type_of_figure));
}

void Move::addJumpedFigures(const QList<JumpedFigure> &figures) {
    if (figures.size() > 0) {
	m_jumpedFigures->append(figures);
    }
}

int Move::price() const {
    return m_jumpedFigures->size();
}

bool Move::isInvalid() const {
    return m_from->getX() < 0;
}

Move Move::getInvalidMove() {
    Move move;
    Location invalid_from(-1, -1);
    move.setFrom(invalid_from);
    return move;
}

bool Move::operator==(const Move &rhs) const {
    if (*m_from != rhs.getFrom() || *m_to != rhs.getTo()) {
	return false;
    }
    if (getJumpedFigures().size() != rhs.getJumpedFigures().size()) {
	return false;
    }
    for (int i = 0; i < getJumpedFigures().size(); i++) {
	if (getJumpedFigures()[i] != rhs.getJumpedFigures()[i]) {
	    return false;
	}
    }
    return true;
}

void Move::setFigureType(Figure::Type type_of_figure) {
    m_typeOfFigure = type_of_figure;
}

Figure::Type Move::getFigureType() const {
    return m_typeOfFigure;
}

bool Move::operator!=(const Move &rhs) const {
    return !(*this == rhs);
}

QString Move::toString() const {
    QString to = m_to->toString();
    QString from = m_from->toString();
    return QString("%1-%2").arg(from, to);
}

QString Move::toFiguresString() const {
    if (m_jumpedFigures->size() == 0) {
	return QObject::tr("no jumped figures in this move");
    }
    else {
	QString jumped_figures("jumped figures in this move:");
	int counter = 1;
	foreach (JumpedFigure fig, *m_jumpedFigures) {
	    jumped_figures.append(QObject::tr("\n %1. %2 at location %3").arg(QString::number(counter++),
									      Figure::toString(fig.second),
									      fig.first.toString()));
	}
	return jumped_figures;
    }
}
