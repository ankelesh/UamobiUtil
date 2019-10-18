#ifndef SCROLLGRABBER_H
#define SCROLLGRABBER_H

#include <QObject>
#include <QAbstractScrollArea>


class ScrollKeyFilter : public QObject
{
    Q_OBJECT
protected:
    int lastCoord;

    bool eventFilter(QObject *, QEvent *);
public:
    explicit ScrollKeyFilter(QObject * parent);
signals:
    void scrollDistanceObtained(int);
};







class QScroller : public QObject
{
    Q_OBJECT
    QAbstractScrollArea * grabbed;
    ScrollKeyFilter * filter;
public:
    enum ScrollerGestureType { LeftMouseButtonGesture};

    explicit QScroller(QAbstractScrollArea* from, ScrollerGestureType what, QObject *parent = 0);

    static void grabGesture(QAbstractScrollArea * from, ScrollerGestureType what);
signals:

public slots:
    void processScrollDistance(int dist);
};

#endif // SCROLLGRABBER_H
