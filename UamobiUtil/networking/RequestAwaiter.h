#pragma once
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkReply>
#include "QueryTemplates.h"
/*
	This file contains Awaiter class - wrapper for network replies, which guaranties sequenced 
	requests to be synchronized. Also it tracks timeouts, parses response and can deliver results via 
	flag defining which method sent request. Use it as second wrapper in chain 
	DataEngine -> awaiter -> handler. Generally it is better to use qt interfaces without wrappers,
	but this object is uniting all actions required in one class. Possible replacements:
	connect directly your handler method to network reply, upcast it via sender(), then use common
	function for determining if it was timeout and to extract data strings. 
*/


class RequestAwaiter : public QObject	//	This class is awaiting sent request
{
	Q_OBJECT
private:
	//	tracks timeouts
	QTimer* timer;
	// determines if awaiter is busy.
	bool awaiting;
	// true when timeout happened
	bool wastimeout;
	// interval for timer
	int timeoutinterval;
	// pointer to awaited reply, used for correct deleting
	QNetworkReply* awaitedReply;
	long long int deliverTo;
public:
	QString restext;		//	Request result. is overwritten when new response arrives
	QString errtext;		//	error string. Provides info about errors
public:
	RequestAwaiter(int interval = 1000, QObject* parent = Q_NULLPTR);
	void run();				//	primes awaiter - launches timer countdown, raises awaiting flag, drops timeout flag
	void deliverResultTo(long long int num);
	bool deliverHere(long long int num);
	bool isAwaiting();		//	true if there was no timeout and no response
	bool wasTimeout();		//	true if there was timeout
	int getInterval();		//	returns interval
	void setReplyToAwait(QNetworkReply* toAwait); 
	void stopAwaiting();
public slots:
	void timeout();			//	sets wastimeout flag, stops awaiting
	void requestIncoming();	//	receives data strings, parses them and fills restext and errtext
	void replyError(QNetworkReply::NetworkError);
signals:
	void requestSuccess(QString, QString);	//	emitted when response arrived with results
	void requestReceived(int mnum = 0);					//	emitted when response arrived
	void requestTimeout();					//	emitted when timeout appeared
};
