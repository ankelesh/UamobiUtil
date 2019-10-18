#include "EventsAndFilters.h"
#include <QtGui/QKeyEvent>
#include <QApplication>

#ifdef Q_OS_ANDROID
#include <QInputMethod>
#endif
#ifdef DEBUG
#include <debugtrace.h>
#endif

bool filters::NoKeyEvents::eventFilter(QObject* object, QEvent* ev)
{
#ifdef DEBUG
	detrace_METHFRECALL("NoKeyEvents::eventFilter");
#endif
	// Captures key press events
	if (ev->type() == QEvent::KeyRelease)
	{
#ifdef DEBUG
		detrace_METHFRECALL("key release captured with text");
#endif
        QKeyEvent* keyptr = static_cast<QKeyEvent*>(ev);
#ifdef DEBUG
		detrace_METHFRECALL(keyptr->text() << "<<");
#endif
		// Sorts: now only return, back and numbers must be intercepted
		if (keyptr->key() == Qt::Key_Return)
		{
#ifdef DEBUG
			detrace_METHFRECALL("Caught kind of return");

#endif
			emit returnObtained();
		}
		else if (keyptr->key() == Qt::Key_Back || keyptr->key() == Qt::Key_Escape)
		{
#ifdef DEBUG

			detrace_METHFRECALL("Caught kind of back");

#endif
			emit backRequired();
		}
		else if (keyptr->key() == Qt::Key_Backspace)
		{
			emit eraseRequired();
		}
		else if (keyptr->key() == 0 || keyptr->key() == 33554431)
		{
			emit unknownObtained();
		}
		else
		{
#ifdef DEBUG
			detrace_METHFRECALL("Caught non-controlling value");
#endif
			emit numberObtained(keyptr->text());
		}
		return true;
	}
	else if (ev->type() == QEvent::KeyPress)
	{ // here was breaker -> possibly it was the solution of old problem? Insert here return if bug restores
        QKeyEvent* keyptr = static_cast<QKeyEvent*>(ev);
		if (keyptr->key() == 0 || keyptr->key() == 33554431)
		{
			emit unknownObtained();
			return true;
		}
	}
	return QObject::eventFilter(object, ev);
}

bool filters::CaptureBackFilter::eventFilter(QObject* object, QEvent* ev)
{
	// Captures key press events
	if (ev->type() == QEvent::KeyRelease)
	{
        QKeyEvent* keyptr = static_cast<QKeyEvent*>(ev);
		// Sorts: now only return, back and numbers must be intercepted
		if (keyptr->key() == Qt::Key_Back || keyptr->key() == Qt::Key_Escape)
		{
			emit backRequired();
			return true;
		}
	}
    return QObject::eventFilter(object, ev);
}
bool filters::LineEditHelper::eventFilter(QObject *watched, QEvent *event)
{
       bool temp = QObject::eventFilter(watched, event);
#ifdef Q_OS_ANDROID
       if (event->type() == QEvent::KeyPress)
       {
        qApp->inputMethod()->commit();
       }
#endif
        return temp;
}
