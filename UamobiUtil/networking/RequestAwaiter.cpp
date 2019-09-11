#include "RequestAwaiter.h"
#include "debugtrace.h"

const char* RECEIVER_SLOT_NAME = "requestIncoming";

RequestAwaiter::RequestAwaiter(int interval, QObject* parent)
	: QObject(parent),timer(new QTimer(this)), awaiting(false)
{
	timer->setInterval(interval);
	timer->setSingleShot(true);
#ifdef QT_VERSION5X
	QObject::connect(timer, &QTimer::timeout, this, &RequestAwaiter::timeout);
#else
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
#endif	
}

void RequestAwaiter::run()
{
	timer->start();
	awaiting = true;
	wastimeout = false;
	restext = errtext = "";
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
	detrace_METHCALL("timeout")
	awaiting = false;
	wastimeout = true;
	emit requestTimeout();
}

void RequestAwaiter::requestIncoming(QString a, QString b)
{/*
	if (a.isEmpty() && b.isEmpty())
		return;*/
	detrace_METHCALL("RequestAwaiter::requestIncoming")
	detrace_METHTEXTS("RequestAwaiter::requestincoming", "a, b", a << "|" << b)
	restext = a; errtext = b;
	detrace_METHEXPL("not awaiting")
	awaiting = false;
	timer->stop();
	wastimeout = false;
	emit requestSuccess(a,b);
}
