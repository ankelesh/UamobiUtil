#pragma once
#include <QtCore/QObject>
#include <QtCore/QTimer>

/*
	This file contains Awaiter class - a simple solution to await request. It has dataupdateengine-compatible interface and
	can wait until response will arrive - or timeout happens. method isAwaiting provides simple interface - when it returns
	true, you must wait. Then you can check timeout flag to determine if it was succesfull. Also, this class
	has normal signal-slot interface to work like standard Qt object

	Update: new signal added - is emitted when request is received, but without arguments

*/

extern const char* RECEIVER_SLOT_NAME; // char string used in dataupdeng to call slot using Qmetacall

class RequestAwaiter : public QObject	//	This class is awaiting sent request
{
	Q_OBJECT
private:
	QTimer* timer;
	bool awaiting;
	bool wastimeout;
	int timeoutinterval;
public:
	QString restext;		//	Request result. is overwritten when new response arrives
	QString errtext;		//	error string. Provides info about errors
public:
	RequestAwaiter(int interval = 1000, QObject* parent = Q_NULLPTR);
	void run();				//	primes awaiter - launches timer countdown, raises awaiting flag, drops timeout flag
	bool isAwaiting();		//	true if there was no timeout and no response
	bool wasTimeout();		//	true if there was timeout
	int getInterval();		//	returns interval
	void stopAwaiting();
private slots:
	void timeout();			//	sets wastimeout flag, stops awaiting
	void requestIncoming(QString, QString);	//	receives data strings
signals:
	void requestSuccess(QString, QString);	//	emitted when response arrived with results
	void requestReceived();					//	emitted when response arrived
	void requestTimeout();					//	emitted when timeout appeared
};
