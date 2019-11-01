#pragma once
#ifdef QT_VERSION5X
#include <QtWidgets/qwidget.h>
#include <QtWidgets/QBoxLayout>
#else
#include <QtGui/qwidget.h>
#include <QtGui/QBoxLayout>
#endif


/*
	This file provides interfaces and common features to control widgets. Main idea is to incapsulate control behind string values.


*/


enum controlType {None, Quantity}; // Types of controls by their inner types. 

class abs_control : public QWidget
	//control interfaces
{
	Q_OBJECT
protected:
	QHBoxLayout* mainLayout; // requires additional layout

	controlType type;	//	default None
	bool isAwaiting;	//	tech value, is raised when control awaits value

	virtual QString prepareAndReturnValue() const =0;	//	inner function, used to return normalized value for using it into web queries
	virtual bool parseAndSetValue(QString) = 0;			//	uses normalized value to set up control
	virtual void clear() = 0;							//	drops state to default. Does not hides control
	virtual bool valueAvailable() const = 0;			//	true if value differs from default

public:
	explicit abs_control(controlType Type = None, QWidget* parent = Q_NULLPTR);
	QString getValue() const;		//	returns normalized value
	bool canGiveValue() const;			//	true if value can be extracted
	controlType myType();				//	return type of this control
	bool setValue(QString);				//	sets value from provided string
	bool isAwaitingValue();				//	true if value is awaited
	void setAwaiting();					//	shows control and sets it to default value
	void reset();						//	sets state to default
	virtual bool hasFocus() const = 0;	//	returns true if inner input field has focus
signals:
	void valueChanged(QString);			//	is emitted when value is changed

};
