#include "figure.h"

#include <QObject>


Figure::Figure() {
}

QString Figure::figureToString(Figure::Type figure) {
    switch (figure) {
	case Figure::WHITE_PAWN:
	    return QObject::tr("white pawn");
	case Figure::WHITE_QUEEN:
	    return QObject::tr("white queen");
	case Figure::BLACK_PAWN:
	    return QObject::tr("black pawn");
	case Figure::BLACK_QUEEN:
	    return QObject::tr("black queen");
	default:
	    return QObject::tr("unknown figure");
    }
}

QList<Figure::Type> Figure::getTypesByColor(const Color &color) {
    QList<Figure::Type> types;
    switch (color) {
	case Figure::BLACK:
	    types << Figure::BLACK_PAWN << Figure::BLACK_QUEEN;
	    break;
	case Figure::WHITE:
	    types << Figure::WHITE_PAWN << Figure::WHITE_QUEEN;
	    break;
	default:
	    break;
    }
    return types;
}

Figure::Color Figure::getColorByType(const Figure::Type &type) {
    switch (type) {
	case Figure::WHITE_PAWN:
	case Figure::WHITE_QUEEN:
	    return Figure::WHITE;
	case Figure::BLACK_PAWN:
	case Figure::BLACK_QUEEN:
	    return Figure::BLACK;
	default:
	    return Figure::UNKNOWN;
    }
}

Figure::Color Figure::negateColor(const Figure::Color &color) {
    switch (color) {
	case Figure::WHITE:
	    return Figure::BLACK;
	case Figure::BLACK:
	    return Figure::WHITE;
	default:
	    return Figure::UNKNOWN;
    }
}

Figure::Type Figure::promote(const Figure::Type &type) {
    switch (getColorByType(type)) {
	case Figure::WHITE:
	    return Figure::WHITE_QUEEN;
	case Figure::BLACK:
	    return Figure::BLACK_QUEEN;
	default:
	    return Figure::EMPTY;
    }
}

Figure::Type Figure::degrade(const Figure::Type &type) {
    switch (getColorByType(type)) {
	case Figure::WHITE:
	    return Figure::WHITE_PAWN;
	case Figure::BLACK:
	    return Figure::BLACK_PAWN;
	default:
	    return Figure::EMPTY;
    }
}
