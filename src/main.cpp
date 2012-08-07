#include "referee.h"
#include "gmainwindow.h"
#include "definitions.h"
#include "gsettings.h"

#include <QApplication>
#include <QTextCodec>
#include <QSettings>
#include <QThread>

QList<Location> *Referee::s_pawnDirections;
QList<Location> *Referee::s_queenDirections;

QSettings *GSettings::s_settings;
/*
class Sleeper : public QThread
{
public:
    static void usleep(unsigned long usecs){QThread::usleep(usecs);}
    static void msleep(unsigned long msecs){QThread::msleep(msecs);}
    static void sleep(unsigned long secs){QThread::sleep(secs);}
};
*/

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

    // Setup settings file.
    GSettings::setupSettings();
/*
    for (int i = 0; i < 10; i++) {
	Sleeper::sleep(2);
	qDebug() << "sleep";
    }
*/
    // Setup information about application.
    a.setApplicationName(APP_NAME);
    a.setOrganizationName(APP_COPYRIGHT);
    a.setOrganizationDomain(APP_URL);
    a.setApplicationVersion(APP_VERSION);
    a.setWindowIcon(QIcon(":/graphics/qgothic.png"));

    GMainWindow w;

    // Setup information for main window.
    w.setWindowIcon(QIcon(":/graphics/qgothic.png"));
    w.setWindowTitle(QObject::tr("%1 %2").arg(APP_NAME,
					      APP_VERSION));

    w.show();
    return a.exec();
}
