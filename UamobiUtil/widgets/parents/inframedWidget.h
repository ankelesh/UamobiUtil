#pragma once
#ifdef QT_VERSION5X
#include <QtWidgets/QWidget>
#else
#include <QtGui/QWidget>
#endif
#include <QtGui/QKeyEvent>
#include <QTime>
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
	QTime lastEventMark;
	int ltype;
	bool listeningKeyboard;
public:
	inframedWidget(QWidget* parent);

	virtual void hide();
	virtual void listenKeyboard();
	virtual void stopListeningKeyboard();
	virtual void setFocus();
protected:
	virtual void _captureNumbers();
	virtual void _backReaction();
	virtual void _numberReaction(int);
	virtual void _arrowReaction(int);
	virtual void _returnReaction();
private slots:
	void backReaction();
	void numberReaction(int);
	void arrowReaction(int);
	void returnReaction();
signals:
	void backRequired();
};
