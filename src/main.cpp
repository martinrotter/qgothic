#include "definitions.h"
#include "debug.h"
#include "referee.h"
#include "gmainwindow.h"
#include "gsettings.h"

#include <QApplication>
#include <QTextCodec>
#include <QSettings>
#include <QThread>
#include <QTranslator>

QList<Location> *Referee::s_pawnDirections;
QList<Location> *Referee::s_queenDirections;

QSettings *GSettings::s_settings;


int main(int argc, char *argv[]) {
    // QGothic works in plain UTF-8 encoding.
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

#ifdef Q_WS_MAC
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif

    // Instantiate application.
    QApplication a(argc, argv);

    // Setup debug output system.
    qInstallMsgHandler(Debug::debugHandler);

    // Setup settings file.
    GSettings::setupSettings();

    // Setup information about application.
    a.setApplicationName(APP_NAME);
    a.setOrganizationName(APP_COPYRIGHT);
    a.setOrganizationDomain(APP_URL);
    a.setApplicationVersion(APP_VERSION);
    a.setWindowIcon(QIcon(":/graphics/qgothic.png"));

    // Load language and setup locale.
    QRegExp sep("[(|)]+");
    QTranslator qt_translator, app_translator;
    QString lang = GSettings::value(SET_APPEAR, "language",
				    "English (en).qm").toString();
    QString locale_name = lang.section(sep, 1, 1);

    // Try to load selected language file.
    if (app_translator.load(lang, LANG_BASE) == false) {
	qDebug() << QString("Language \'%1\' wasn't loaded successfully.").arg(lang);
    }
    // If file is loaded, then install it.
    else {
	a.installTranslator(&app_translator);
	qDebug() << QString("Language \'%1\' was loaded successfully.").arg(lang);
    }

    // Setup translation for Qt itself.
    // Try to load selected language file.
    if (qt_translator.load(QString("qt_%1.qm").arg(locale_name), QString("%1/qt").arg(LANG_BASE)) == false) {
	qDebug() << QString("Language for Qt \'%1\' wasn't loaded successfully.").arg(locale_name);
    }
    // If file is loaded, then install it.
    else {
	a.installTranslator(&qt_translator);
	qDebug() << QString("Language for Qt \'%1\' was loaded successfully.").arg(locale_name);
    }

    // Set up locale.
    qDebug() << QString("Setting up '%1' locale.").arg(locale_name);
    QLocale locale(locale_name);
    QLocale::setDefault(locale);

    GMainWindow w;

    // Setup information for main window.
    w.setWindowIcon(QIcon(":/graphics/qgothic.png"));
    w.setWindowTitle(QObject::tr("%1 %2").arg(APP_NAME,
					      APP_VERSION));

    if (argc > 1) {
	w.loadFromFile(argv[1]);
    }

    w.show();
    return a.exec();
}
