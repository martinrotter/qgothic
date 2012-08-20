#include "definitions.h"
#include "debug.h"
#include "referee.h"
#include "gmainwindow.h"
#include "gsettings.h"

#include <QApplication>
#include <QTextCodec>
#include <QSettings>
#include <QThread>
#include <QSplashScreen>
#include <QTime>
#include <QTranslator>
#include <QDebug>


QList<Location> *Referee::s_pawnDirections;
QList<Location> *Referee::s_queenDirections;
QSettings *GSettings::s_settings;
bool Intelligence::m_cancelling;

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

    // Display main thread id.
    qDebug() << "Entering thread with"
	     << Q_FUNC_INFO << "and code"
	     << QThread::currentThreadId() << ".";

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
	qDebug("Language \'%s\' wasn't loaded successfully.", lang.toAscii().constData());
    }
    // If file is loaded, then install it.
    else {
	a.installTranslator(&app_translator);
	qDebug("Language \'%s\' was loaded successfully.", lang.toAscii().constData());
    }

    // Setup translation for Qt itself.
    // Try to load selected language file.
    if (qt_translator.load(QString("qt_%1.qm").arg(locale_name), QString("%1/qt").arg(LANG_BASE)) == false) {
	qDebug("Language for Qt \'%s\' wasn't loaded successfully.", locale_name.toAscii().constData());
    }
    // If file is loaded, then install it.
    else {
	a.installTranslator(&qt_translator);
	qDebug("Language for Qt \'%s\' was loaded successfully.", locale_name.toAscii().constData());
    }

    // Set up locale.
    qDebug("Setting up '%s' locale.", locale_name.toAscii().constData());
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
