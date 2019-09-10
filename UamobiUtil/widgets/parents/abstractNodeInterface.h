#pragma once

#include <QtCore/QPointer>

#include "widgets/parents/inframedWidget.h"


/*
	Attempts to make uniform interface for any node

	NOT DONE

	Problems:
		slots incompatible, must be QObject, but this will lead to double QObjects in child
		no idea how to dynamically connect or delete widgets of really different types


*/
class abstractNode
{
protected:
	inframedWidget* current;
	inframedWidget* untouchable;

	abstractNode() {};
	abstractNode(inframedWidget* c, inframedWidget* u) : current(c), untouchable(u) {};
public slots:
	virtual void hideCurrent(inframedWidget* replacement);
	virtual void hideAny(inframedWidget* replacement);

};

class abstractDynamicNode
{
protected:
	QList<QPointer<inframedWidget>* > allWidgets;
	QPointer<inframedWidget>* current;
	QPointer<inframedWidget>* untouchable;

	virtual void hideAndDeleteCurrent(QPointer<inframedWidget>* replacement);
	virtual void hideAndDeleteAny(QPointer<inframedWidget>* replacement);

};