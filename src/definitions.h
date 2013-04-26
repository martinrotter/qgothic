#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <QString>
#include <QDir>


/////////////////////
/// Paths and names.
/////////////////////
#define ENABLE_DEBUG_OUTPUTS	1
const QString CONFIG_BASE	= QDir::homePath() + QDir::separator() + ".qgothic";
const QString CONFIG_NAME	= "qgothic.ini";

const QString LANG_BASE		= ":/translations";

const QString APP_NAME		= "QGothic";
const QString APP_VERSION	= "0.3.8";
const QString APP_AUTHOR	= "Martin Rotter";
const QString APP_COPYRIGHT	= "Copyright (C) 2011-2012 Martin Rotter";
const QString APP_URL		= "http://code.google.com/p/qgothic/";
const QString APP_EMAIL		= "rotter.martinos@gmail.com";
const QString APP_DEBUG		= QString("[%1]").arg(APP_NAME);

///////////////////////
/// Settings sections.
///////////////////////
const QString SET_APPEAR	= "Appearance";
const QString SET_GAME		= "Game";

/////////////////////
/// Game defines.
/////////////////////
const QString GAM_TURNS		= ":/graphics/current-player.png";
const QString GAM_HUMAN_IMG	= ":/graphics/dif-human.png";
const QString GAM_HUMAN_IMG_B	= ":/graphics/dif-human-big.png";
const QString GAM_EASY_IMG	= ":/graphics/dif-easy.png";
const QString GAM_EASY_IMG_B	= ":/graphics/dif-easy-big.png";
const QString GAM_MEDIUM_IMG	= ":/graphics/dif-medium.png";
const QString GAM_MEDIUM_IMG_B	= ":/graphics/dif-medium-big.png";
const QString GAM_HARD_IMG	= ":/graphics/dif-hard.png";
const QString GAM_HARD_IMG_B	= ":/graphics/dif-hard-big.png";
const QString GAM_DRAW		= ":/graphics/status-equal.png";
const QString GAM_WON		= ":/graphics/status-winner.png";
const QString GAM_LOST		= ":/graphics/status-loser.png";
const QString GAM_ORDINARY	= ":/graphics/status-ordinary.png";
const QString GAM_JUMPS		= ":/graphics/game-jumps.png";
const QString GAM_PAWNS		= ":/graphics/game-pawns.png";

const QString GAM_PLAY_STYLE	= "<div style=\"margin-bottom: 5px;\"><img height='%1' width='%1' src='%2'></div>%3";

#endif // DEFINITIONS_H
