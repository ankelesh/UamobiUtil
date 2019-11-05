#include "RequestAwaiter.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif
const char* RECEIVER_SLOT_NAME = "requestIncoming";

RequestAwaiter::RequestAwaiter(int interval, QObject* parent)
	: QObject(parent), timer(new QTimer(this)), awaiting(false), timeoutinterval(interval)
{
#ifdef DEBUG
    //detrace_METHEXPL("interval was:" << interval );
#endif
	timer->setInterval(interval);
	timer->setSingleShot(true);

#ifdef DEBUG
#ifdef QT_VERSION5X
	//detrace_METHEXPL("Timer in inner measures: " << timer->intervalAsDuration().count());
#endif
#endif
#ifdef QT_VERSION5X
	QObject::connect(timer, &QTimer::timeout, this, &RequestAwaiter::timeout);
#else
   QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
#endif
}

void RequestAwaiter::run()
{
	timer->setInterval(timeoutinterval);
	timer->start();

#ifdef DEBUG
	//detrace_METHEXPL("now with interval " << timer->interval() << " remaining is " << timer->remainingTime());
#endif

	awaiting = true;
	wastimeout = false;
	restext = QString();
	errtext = QString();
}

bool RequestAwaiter::isAwaiting()
{
	return awaiting;
}

bool RequestAwaiter::wasTimeout()
{
	return wastimeout;
}

int RequestAwaiter::getInterval()
{
	return timeoutinterval;
}

void RequestAwaiter::timeout()
{
#ifdef DEBUG
   // detrace_METHCALL("RequestAwaiter::timeout");
	//detrace_METHEXPL(timer->remainingTime() << " msecs remaining");
#endif
	awaiting = false;
	wastimeout = true;
	emit requestTimeout();
}

void RequestAwaiter::requestIncoming(QString a, QString b)
{/*
	if (a.isEmpty() && b.isEmpty())
		return;*/
#ifdef DEBUG
    //detrace_METHCALL("RequestAwaiter::requestIncoming");
	//detrace_METHEXPL(timer->remainingTime() << " msecs remaining");
	detrace_METHTEXTS("RequestAwaiter::requestincoming", "a, b", a << "|" << b);
#endif
	restext = a; errtext = b;

	awaiting = false;
    timer->stop();
	wastimeout = false;
	emit requestSuccess(a, b);
	emit requestReceived();
}
