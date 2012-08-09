#ifndef GSPINBOX_H
#define GSPINBOX_H

#include <QSpinBox>


class GSpinBox : public QSpinBox{
	Q_OBJECT

    public:
	explicit GSpinBox(QWidget *parent = 0);
	void setLineEditReadOnly(bool read_only);
	bool isLineEditReadOnly() const;
	
};

#endif // GSPINBOX_H
