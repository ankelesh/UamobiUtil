#include "scrollgrabber.h"
#include <QEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>

bool ScrollKeyFilter::eventFilter(QObject * o, QEvent * ev)
{
    if (ev->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent * mep = static_cast<QMouseEvent*>(ev);
        if (mep != Q_NULLPTR)
        {
            if (mep->button() == Qt::LeftButton)
            {
                lastCoord = mep->y();
                return QObject::eventFilter(o, ev);
            }
        }
    }
    else if (ev->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent * mep = static_cast<QMouseEvent*>(ev);
        if (mep != Q_NULLPTR)
        {
            if (mep->button() == Qt::LeftButton)
            {
                if ( mep->y() - lastCoord >20 || mep->y() - lastCoord < -20)
                {
                    int val =  mep->y() - lastCoord;
                    emit scrollDistanceObtained(-val);
                    return true;
                }
                else
                {
                    return QObject::eventFilter(o,ev);
                }
            }
        }
    }
    return QObject::eventFilter(o, ev);

}

ScrollKeyFilter::ScrollKeyFilter(QObject *parent)
{

}


QScroller::QScroller(QAbstractScrollArea *from, QScroller::ScrollerGestureType what, QObject *parent)
    :QObject(parent), grabbed(from), filter(new ScrollKeyFilter(this))
{
    from->viewport()->installEventFilter(filter);
    QObject::connect(filter, SIGNAL(scrollDistanceObtained(int)), this, SLOT(processScrollDistance(int)));
}

void QScroller::grabGesture(QAbstractScrollArea *from, QScroller::ScrollerGestureType what)
{
    QScroller* tmp = new QScroller(from, what, from);
}

void QScroller::processScrollDistance(int dist)
{
    qDebug() << "setting value " << dist;
    grabbed->verticalScrollBar()->setValue(grabbed->verticalScrollBar()->value() + dist);
}
