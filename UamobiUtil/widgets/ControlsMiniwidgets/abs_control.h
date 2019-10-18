#pragma once
#ifdef QT_VERSION5X
#include <QtWidgets/qwidget.h>
#include <QtWidgets/QBoxLayout>
#else
#include <QtGui/qwidget.h>
#include <QtGui/QBoxLayout>
#endif


enum controlType {None, Quantity};

class abs_control : public QWidget
{
	Q_OBJECT
protected:
	QHBoxLayout* mainLayout;

	controlType type;
	bool isAwaiting;

	virtual QString prepareAndReturnValue() const =0;
	virtual bool parseAndSetValue(QString) = 0;
	virtual void clear() = 0;
	virtual bool valueAvailable() const = 0;

public:
	explicit abs_control(controlType Type = None, QWidget* parent = Q_NULLPTR);
	QString getValue() const;
	bool canGiveValue() const;
	controlType myType();
	bool setValue(QString);
	bool isAwaitingValue();
	void setAwaiting();
	void reset();

};
