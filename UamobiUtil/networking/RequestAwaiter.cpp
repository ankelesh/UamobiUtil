#include "RequestAwaiter.h"

#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

const char* RECEIVER_SLOT_NAME = "requestIncoming";

RequestAwaiter::RequestAwaiter(int interval, QObject* parent)
	: QObject(parent), timer(new QTimer(this)), awaiting(false)
{
#ifdef DEBUG
    detrace_METHEXPL("interval was:" << interval );
#endif
	timer->setInterval(interval);
	timer->setSingleShot(true);
#ifdef QT_VERSION5X
	QObject::connect(timer, &QTimer::timeout, this, &RequestAwaiter::timeout);
#else
    detrace_CONNECTSTAT("timer",  QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timeout())));
#endif
}

void RequestAwaiter::run()
{
    detrace_METHCALL("run");
	timer->start();
    detrace_METHEXPL("timer started, buffers wiped");
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

void RequestAwaiter::timeout()
{
#ifdef DEBUG
    detrace_METHCALL("RequestAwaiter::timeout");
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
    detrace_METHCALL("RequestAwaiter::requestIncoming");
	detrace_METHTEXTS("RequestAwaiter::requestincoming", "a, b", a << "|" << b);
#endif
	restext = a; errtext = b;

	awaiting = false;
	timer->stop();
    detrace_METHEXPL(timer->isActive());
	wastimeout = false;
	emit requestSuccess(a, b);
	emit requestReceived();
}
