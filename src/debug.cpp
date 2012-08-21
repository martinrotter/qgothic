#include "debug.h"
#include "definitions.h"

#include <cstdio>
#include <cstdlib>

#include <QApplication>


Debug::Debug() {
}

void Debug::debugHandler(QtMsgType type, const char *msg) {
#ifdef ENABLE_DEBUG_OUTPUTS
    switch (type) {
	case QtDebugMsg:
	    fprintf(stderr, "%s (normal): %s\n", APP_DEBUG.toAscii().constData(), msg);
	    break;
	case QtWarningMsg:
	    fprintf(stderr, "%s (warning): %s\n", APP_DEBUG.toAscii().constData(), msg);
	    break;
	case QtCriticalMsg:
	    fprintf(stderr, "%s (critical): %s\n", APP_DEBUG.toAscii().constData(), msg);
	    break;
	case QtFatalMsg:
	    fprintf(stderr, "%s (fatal): %s\nApplication is halting now with error return code.",
		    APP_DEBUG.toAscii().constData(), msg);
	    qApp->exit(EXIT_FAILURE);
    }
#endif
}
