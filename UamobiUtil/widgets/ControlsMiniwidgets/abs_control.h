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

	Update:
		Now this widget is not QObject to avoid unused levels of inheritance and layouts. Now it fully incapsulates
		any widget behind it's interfaces.
		
		This widget is buffer-based. You can associate it with buffer and then just refresh instead of overriding value.
		Warning! do not associate it with temporary variables. 

		To construct control without meaning it's full type - use fabricateControl functions. They will use initstr value to define which
		control type must be created.

*/

class abs_control
	//control interfaces
{
public:
	enum controlType { None, Int }; // Types of controls by their inner types.

protected:
	QString* associatedBuffer;
	controlType type;	//	default None
	bool isAwaiting;	//	tech value, is raised when control awaits value

	virtual QString prepareAndReturnValue() const = 0;	//	inner function, used to return normalized value for using it into web queries
	virtual bool parseAndSetValue(QString) = 0;			//	uses normalized value to set up control
	virtual void clear() = 0;							//	drops state to default. Does not hides control
	virtual bool valueAvailable() const = 0;			//	true if value differs from default
    virtual void setListening(bool) = 0;
public:
	QString defaultBuffer;
	QString name;
	QString label;

	explicit abs_control(QString cname, QString& assocBuffer, controlType Type = None);
	explicit abs_control(QString cname, controlType Type = None);
	QString getValue() const;		//	returns normalized value
	bool canGiveValue() const;			//	true if value can be extracted
	controlType myType();				//	return type of this control
	bool setValue(QString);				//	sets value from provided string
	bool isAwaitingValue();				//	true if value is awaited
	void setAwaiting();					//	shows control and sets it to default value
	void reset();						//	sets state to default
	virtual bool hasFocus() const = 0;	//	returns true if inner input field has focus
	virtual void setFocus() const = 0;
	virtual void show() = 0;
	virtual void hide() = 0;
	virtual void refresh() = 0;
	virtual void installEventFilter(QObject*) = 0;
	virtual QWidget* myWidget() = 0;
	void unassociateBuffer();
	void associateBuffer(QString&);
	virtual ~abs_control() {};
};
