#pragma once
#ifdef QT_VERSION5X
#include <QtWidgets/QWidget>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qdatetimeedit.h>
#else
#include <QtGui/QWidget>
#include <QtGui/QSpinBox>
#include <QtGui/qpushbutton.h>
#include <QtGui/qboxlayout.h>
#include <QtGui/qdatetimeedit.h>
#endif
#include "widgets/utils/EventsAndFilters.h"

/*
	This class is a wrapper for any normal spinBox, providing it with big and suitable
	buttons for steps. For specifying which spinbox is added into it->check spintype.
	It uses RTTI for connecting to corresponding slots, so you must alter this class to add
	new spinbox type in it. Now supported: SpinBox, DatetimeEdit and all their inheritors.

	Update:
		Now Keyfilter is filtering input to get back presses. Signal backRequired is emitted
		on <- key.
*/

class BigButtonsSpinbox : public QWidget
	//	Spinbox with two GIANT buttons which are replacing it's original
{
	Q_OBJECT
public:
	enum spintype { intspin, timespin };	//	Used to determine which type of spinbox is wrapped

private:
	QHBoxLayout* mainLayout;			//	View
	QPushButton* buttonUp;
	QPushButton* buttonDown;
	QAbstractSpinBox* coreSpinbox;	//	Abstract spinbox allows to place here any spinbox
	filters::CaptureBackFilter* keyFilter;		//	Captures back press

	spintype sptype;				//	Stores type for correct usage of inner functions

public:
	BigButtonsSpinbox(spintype type, QWidget* parent, double adaptH = 0.125);
	//	methods of QSpinBox interface
	void setMinimum(int min);	// sets maximum
	void setMaximum(int max);	// sets minimum
	void setValue(int val);		//	sets current value
	int value() const;				//	returns current value
	//	methods of QTimeEdit interface
	void setTime(const QTime& tm);	//	sets current value
	QTime time();			//	returns current value
	void setDisplayFormat(const QString& tf);//	sets format
	bool hasFocus() const;

private slots:
	void intValueChanged(int);				//	These slots are wrapping slots of inner spinbox
	void timeValueChanged(const QTime& t);
	void editingDone();
	void backRequire();

public slots:
	void setFocus() const;			//	Sets focus to spinbox

signals:
	void ivalueChanged(int);			//	emitted only when SpinBox is wrapped
	void timeChanged(const QTime& t);	//	emitted only when TimeEdit is wrapped
	void valueChanged(QString);
	void returnPressed();				//	return was pressed
	void backRequired();				//	back was pressed
};
