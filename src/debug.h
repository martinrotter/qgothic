#ifndef DEBUG_H
#define DEBUG_H

#include <QtGlobal>


/*!
 * \brief Debug output provider.
 * \ingroup Core
 */
class Debug {

    public:
    static void debugHandler(QtMsgType type, const QMessageLogContext &context, const QString &message);

    private:
	Debug();

};

#endif // DEBUG_H
