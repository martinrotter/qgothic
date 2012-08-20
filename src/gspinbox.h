#ifndef GSPINBOX_H
#define GSPINBOX_H

#include <QSpinBox>


/*!
 * \brief Custom QSpinBox subclass.
 *
 * Subclass of QSpinBox, which allows user to set text part of control to be read-only.
 *
 * \ingroup Gui
 */
class GSpinBox : public QSpinBox{
	Q_OBJECT

    public:
	explicit GSpinBox(QWidget *parent = 0);
	void setLineEditReadOnly(bool read_only);
	bool isLineEditReadOnly() const;
	
};

#endif // GSPINBOX_H
