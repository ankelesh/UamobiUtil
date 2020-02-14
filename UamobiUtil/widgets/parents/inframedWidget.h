#pragma once
#ifdef QT_VERSION5X
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif
#include <QtGui/QKeyEvent>
#include "widgets/utils/EventsAndFilters.h"
/*
		This widget is parent for everything what is going to use one-frame interface.
		It carries keyfilter which allready is awaiting for normal mobile key reactions.
		You can override these reactions or disable this filter.

*/

class inframedWidget : public QWidget
{
	Q_OBJECT
protected:
	filters::GeneralPurposeFilter* keyfilter;
	virtual void keyReleaseEvent(QKeyEvent* kev) override;
public:
	inframedWidget(QWidget* parent);
	inframedWidget(bool installFilter, QWidget * parent);
	virtual bool isExpectingControl(int);	//	Enhanced: now false is returned if value was not used
	filters::GeneralPurposeFilter* getFilter() { return keyfilter; };
	virtual void show();
	void installEventFilter(QObject*);
protected slots:
	virtual void returnReaction();
	virtual void backReaction();
	virtual void controlReaction(int);
signals:
	void backRequired();
};
