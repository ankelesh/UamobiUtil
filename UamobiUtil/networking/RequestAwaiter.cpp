#include "RequestAwaiter.h"
#include <QTextDecoder>
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif
const char* RECEIVER_SLOT_NAME = "requestIncoming";

RequestAwaiter::RequestAwaiter(int interval, QObject* parent)
	: QObject(parent), timer(new QTimer(this)), awaiting(false), timeoutinterval(interval),
	awaitedReply(Q_NULLPTR), deliverTo(0)
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
	awaiting = true;
	wastimeout = false;
	restext = QString();
	errtext = QString();
	deliverTo = 0;
}

void RequestAwaiter::deliverResultTo(long long int num)
{
	deliverTo = num;
}

bool RequestAwaiter::deliverHere(long long int num)
{
	return deliverTo == num;
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



void RequestAwaiter::setReplyToAwait(QNetworkReply* toAwait)
{
	if (awaitedReply != Q_NULLPTR)
		return;
	awaitedReply = toAwait;
#ifdef QT_VERSION5X
	QObject::connect(toAwait, &QNetworkReply::finished, this, &RequestAwaiter::requestIncoming);
	QObject::connect(toAwait, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this, &RequestAwaiter::replyError);
#else
	QObject::connect(toAwait, SIGNAL(finished()), this, SLOT(requestIncoming()));
	QObject::connect(toAwait, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(replyError(QNetworkReply::NetworkError)));
#endif
	run();
}


void RequestAwaiter::stopAwaiting()
{
	awaiting = false;
	wastimeout = false;
	timer->stop();
}

void RequestAwaiter::timeout()
{
#ifdef DEBUG
	// detrace_METHCALL("RequestAwaiter::timeout");
	 //detrace_METHEXPL(timer->remainingTime() << " msecs remaining");
#endif
	awaiting = false;
	wastimeout = true;
	deliverTo = 0;
	emit requestTimeout();
}

void RequestAwaiter::requestIncoming()
{
    QTextDecoder td(QTextCodec::codecForName("CP1251"));
	if (!awaitedReply->error())
        restext = td.toUnicode(awaitedReply->readAll());
	if (awaitedReply->error() == QNetworkReply::NoError) {
		if (awaitedReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200)
			errtext =
			awaitedReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()
			+ " "
			+ awaitedReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
	}
	else
	{
		errtext = awaitedReply->errorString();
    }
	awaiting = false;
	timer->stop();
	wastimeout = false;
    if (awaitedReply != Q_NULLPTR) {
		awaitedReply->deleteLater();
        awaitedReply = Q_NULLPTR;
	}
#ifdef DEBUG
    detrace_METHEXPL("received packet: " << restext.left(400) << " | " << errtext);
#endif
	emit requestSuccess(restext, errtext);
	emit requestReceived(deliverTo);
}

void RequestAwaiter::replyError(QNetworkReply::NetworkError error)
{
#ifdef DEBUG
	detrace_METHEXPL("Reply error! " << (int) error);
#endif
}
