#pragma once
#ifdef QT_VERSION5X
#include <QtWidgets/qwidget.h>
#include <QtWidgets/QBoxLayout>
#include <QObject>

#else
#include <QtGui/qwidget.h>
#include <QtGui/QBoxLayout>
#include <QObject>
#endif
#include <QString>
#include "widgets/ElementWidgets/BigButtonsSpinbox.h"
#include "qsharedpointer.h"

/*
	This file provides interfaces and common features to control widgets. Main idea is to 
	incapsulate control type behind universal interfaces based on strings.
*/

class abs_control : public QObject
	//control interfaces
{
	Q_OBJECT
public:
	enum controlType { None, Int, Float }; // Types of controls by their inner types.

protected:
	controlType type;	//	default None
	QWidget* parent;
	QString label;

	virtual QString prepareAndReturnValue() const = 0;	//	inner function, used to return normalized value for using it into web queries
	virtual bool parseAndSetValue(QString) = 0;			//	uses normalized value to set up control
	virtual void clear() = 0;							//	drops state to default. Does not hides control
	virtual bool valueAvailable() const = 0;			//	true if value differs from default
	virtual QWidget* getInnerWidget() const = 0;
public:
	QString name;
	explicit abs_control(QWidget* parent, controlType Type = None, QString cname = QString());
	QString getValue() const;		//	returns normalized value
	QWidget* myWidget() const;
	controlType myType();				//	return type of this control
	bool setValue(QString);				//	sets value from provided string
	void reset();						//	sets state to default
	virtual ~abs_control() = 0;
public slots:
	void subcontrolEditingFinished();
	virtual void setFocus() const  = 0;	
	virtual void show() = 0;			//	shows wrapped widget
	virtual void hide() = 0;			//	hides wrapped widget
signals:
	void editingFinished();
	void valueReceived(QString);
};
typedef QSharedPointer<abs_control> Control;

