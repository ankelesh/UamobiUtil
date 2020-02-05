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
	This file provides interfaces and common features to control widgets. Main idea is to incapsulate control behind string values.

	Update:
		Now this widget is not QObject to avoid unused levels of inheritance and layouts. Now it fully incapsulates
		any widget behind it's interfaces.
		
		This widget is buffer-based. You can associate it with buffer and then just refresh instead of overriding value.
		Warning! do not associate it with temporary variables. 

		To construct control without meaning it's full type - use fabricateControl functions. They will use initstr value to define which
		control type must be created.

		Now this file contains refresher class. This class is wrapped qobject that allows capturing of valueChanged() signals and uses
		value from it to update buffer. This is usefull if user is allowed to change value of inner widget directly, avoiding capturer.

*/

class abs_control
	//control interfaces
{
public:
	enum controlType { None, Int, Float }; // Types of controls by their inner types.

protected:
	QString* associatedBuffer;
	controlType type;	//	default None
	bool isAwaiting;	//	tech value, is raised when control awaits value

	virtual QString prepareAndReturnValue() const = 0;	//	inner function, used to return normalized value for using it into web queries
	virtual bool parseAndSetValue(QString) = 0;			//	uses normalized value to set up control
	virtual void clear() = 0;							//	drops state to default. Does not hides control
	virtual bool valueAvailable() const = 0;			//	true if value differs from default
    virtual void setListening(bool) = 0;				//	sets this control as enabled\disabled. This affects refreshing.
public:
	QString defaultBuffer;								//  default buffer. You can use it directly instead of any global one.
	QString name;										//	name of this widget in server notation
	QString label;										//	name which is displayed to user

	explicit abs_control(QString cname, QString& assocBuffer, controlType Type = None);
	explicit abs_control(QString cname, controlType Type = None);
	QString getValue() const;		//	returns normalized value
	bool canGiveValue() const;			//	true if value can be extracted
	controlType myType();				//	return type of this control
	bool setValue(QString);				//	sets value from provided string
	bool isAwaitingValue();				//	true if value is awaited
	void setAwaiting();					//	shows control and sets it to default value
	void reset();						//	sets state to default

	//interface functions that are covering normal widget ones
	virtual bool hasFocus() const = 0;	//	returns true if inner input field has focus
	virtual void setFocus() const = 0;	//	sets focus
	virtual void show() = 0;			//	shows wrapped widget
	virtual void hide() = 0;			//	hides wrapped widget
	virtual void refresh() = 0;			//	refreshes control using value from buffer
	virtual void installEventFilter(QObject*) = 0;	//	sets filter to wrapped widget
	virtual QWidget* myWidget() = 0;				//	returns inner widget 
	virtual void makeConnectionBetweenControls(abs_control* another) = 0;	//	connects two controls,allowing jumps after editing finish
	void unassociateBuffer();								//	sets buffer to default and disables listening
	void associateBuffer(QString&);							//	sets buffer to provided
	virtual ~abs_control() {};								//	must be virtual to correctly delete inner widget
};

typedef QSharedPointer<abs_control> Control;

namespace control_utils
{
	class refresher : public QObject
		// refreshes buffer when spinbox emits valueChanged() with autochecking for focus.
	{
		Q_OBJECT
	private:
		QString** bufferPointer;
		BigButtonsSpinbox* spbox;
	public:
		refresher(QString** buff, BigButtonsSpinbox* sbox) : bufferPointer(buff), spbox(sbox) {};
	public slots:
		void sync(QString& sval);
	};
}