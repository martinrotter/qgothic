#ifndef DEBUG_H
#define DEBUG_H

#include <QtGlobal>


/*!
 * \brief Debug output provider.
 * \ingroup Core
 */
class Debug {

    public:
	static void debugHandler(QtMsgType type, const char *msg);

    private:
	Debug();

};

#endif // DEBUG_H
