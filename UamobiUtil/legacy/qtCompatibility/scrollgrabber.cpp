#include "scrollgrabber.h"
#include <QEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>

bool ScrollKeyFilter::eventFilter(QObject* o, QEvent* ev)
{
	if (ev->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent* mep = static_cast<QMouseEvent*>(ev);
		if (mep != Q_NULLPTR)
		{
			if (mep->button() == Qt::LeftButton)
			{
				lastCoord = mep->y();
				return QObject::eventFilter(o, ev);
			}
		}
	}
	else if (ev->type() == QEvent::MouseMove)
	{
		QMouseEvent* mep = static_cast<QMouseEvent*>(ev);
		if (mep != Q_NULLPTR)
		{
			if (mep->button() == Qt::LeftButton)
			{
				int val = mep->y() - lastCoord;
				emit scrollDistanceObtained(-(val * 2));
				return true;
			}
		}
	}
	return QObject::eventFilter(o, ev);
}

ScrollKeyFilter::ScrollKeyFilter(QObject* /*parent*/)
{
}

QScroller::QScroller(QAbstractScrollArea* from, QScroller::ScrollerGestureType /*what*/, QObject* parent)
	:QObject(parent), grabbed(from), filter(new ScrollKeyFilter(this))
{
	from->viewport()->installEventFilter(filter);
	QObject::connect(filter, SIGNAL(scrollDistanceObtained(int)), this, SLOT(processScrollDistance(int)));
}

void QScroller::grabGesture(QAbstractScrollArea* from, QScroller::ScrollerGestureType what)
{
    new QScroller(from, what, from);
}

void QScroller::processScrollDistance(int dist)
{
	grabbed->verticalScrollBar()->setValue(grabbed->verticalScrollBar()->value() + dist);
}
