#ifndef GINTERFACE_H
#define GINTERFACE_H


class QWidget;

/*!
 * \brief Various methods for working with GUI.
 */
class GInterface {

    public:
	/*!
	 * \brief Centers selected widget both vertically and horizontally on the screen.
	 * \param widget Widget to center.
	 */
	static void centerWidgetOnScreen(QWidget *widget);

    private:
	GInterface();

};

#endif // GINTERFACE_H
