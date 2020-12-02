#include "inframedWidget.h"
//#define DEBUG
#ifdef DEBUG
#include <debugtrace.h>
#endif
#include "ScaningCore/BarcodeObserver.h"
#include <QTime>
using namespace filters;
inframedWidget::inframedWidget(QWidget* parent)
	: QWidget(parent), lastEventMark(QTime::currentTime()), ltype(0), listeningKeyboard(false)
{

}

void inframedWidget::hide()
{
	QWidget::hide();
}

void inframedWidget::listenKeyboard()
{
	if (!listeningKeyboard)
	{
#ifdef QT_VERSION5X
		QObject::connect(BarcodeObs, &BarcodeObserver::escapeCaught, this, &inframedWidget::backReaction);
		QObject::connect(BarcodeObs, &BarcodeObserver::numberPressed, this, &inframedWidget::numberReaction);
		QObject::connect(BarcodeObs, &BarcodeObserver::arrowCaught, this, &inframedWidget::arrowReaction);
		QObject::connect(BarcodeObs, &BarcodeObserver::returnCaught, this, &inframedWidget::returnReaction);
#else
		QObject::connect(BarcodeObs, SIGNAL(escapeCaught()), this, SLOT(backReaction()));
		QObject::connect(BarcodeObs, SIGNAL(numberPressed(int)), this, SLOT(numberReaction(int)));
		QObject::connect(BarcodeObs, SIGNAL(arrowCaught(int)), this, SLOT(arrowReaction(int)));
		QObject::connect(BarcodeObs, SIGNAL(returnCaught()), this, SLOT(returnReaction()));
#endif
		listeningKeyboard = true;
	}
}

void inframedWidget::stopListeningKeyboard()
{
	if (listeningKeyboard)
	{
		this;
		bool a = BarcodeObs->disconnect(this);
		listeningKeyboard = false;
		BarcodeObs->stopNumberCatching();
	}
}

void inframedWidget::setFocus()
{
	QWidget::setFocus();
}

void inframedWidget::_captureNumbers()
{
	BarcodeObs->catchNumbers();
}

void inframedWidget::_backReaction()
{
	emit backRequired();
}

void inframedWidget::_numberReaction(int)
{
}

void inframedWidget::_arrowReaction(int)
{

}

void inframedWidget::_returnReaction()
{
}

void inframedWidget::backReaction()
{
	if (ltype == 1)
		if (lastEventMark.msecsTo(QTime::currentTime()) < 50)
			return;
	if (!isHidden())
	{
		_backReaction();
		ltype = 1;
		lastEventMark = QTime::currentTime();
	};

}

void inframedWidget::numberReaction(int num)
{
	if (ltype == 2)
		if (lastEventMark.msecsTo(QTime::currentTime()) < 50)
			return;
	if (!isHidden())
	{
		_numberReaction(num);
		ltype = 2;
		lastEventMark = QTime::currentTime();
	};
}

void inframedWidget::arrowReaction(int arr)
{
	if (ltype == 3)
		if (lastEventMark.msecsTo(QTime::currentTime()) < 50)
			return;
	if (!isHidden())
	{
		_arrowReaction(arr);
		ltype = 3;
		lastEventMark = QTime::currentTime();
	};
}

void inframedWidget::returnReaction()
{
	if (ltype == 4)
		if (lastEventMark.msecsTo(QTime::currentTime()) < 50)
			return;
	if (!isHidden())
	{
		_returnReaction();
		ltype = 4;
		lastEventMark = QTime::currentTime();
	};
}
