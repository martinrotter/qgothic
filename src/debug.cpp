#include "debug.h"
#include "definitions.h"

#include <cstdio>
#include <cstdlib>

#include <QApplication>


Debug::Debug() {
}

void Debug::debugHandler(QtMsgType type,
                         const QMessageLogContext &context,
                         const QString &message) {
  Q_UNUSED(context);

#ifdef ENABLE_DEBUG_OUTPUTS
  switch (type) {
    case QtDebugMsg:
      fprintf(stderr, "%s (normal): %s\n", qPrintable(APP_DEBUG), qPrintable(message));
      break;
    case QtWarningMsg:
      fprintf(stderr, "%s (warning): %s\n", qPrintable(APP_DEBUG), qPrintable(message));
      break;
    case QtCriticalMsg:
      fprintf(stderr, "%s (critical): %s\n", qPrintable(APP_DEBUG), qPrintable(message));
      break;
    case QtFatalMsg:
      fprintf(stderr, "%s (fatal): %s\nApplication is halting now with error return code.",
              qPrintable(APP_DEBUG), qPrintable(message));
      qApp->exit(EXIT_FAILURE);
  }
#endif
}
