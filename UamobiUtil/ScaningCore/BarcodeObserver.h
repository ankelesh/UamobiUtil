#pragma once
#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QKeySequence>

#define BarcodeObs BarcodeObserver::instanse()

class BarcodeObserver : public QObject
{
	Q_OBJECT
protected:
	QKeySequence prefix;
	QKeySequence suffix;
	QString buffer;
	bool prefixFound;

	bool active;

	static BarcodeObserver* _instanse;

	bool eventFilter(QObject* object, QEvent* event);

public:
	BarcodeObserver(QChar pref, QChar suff, QObject* parent = Q_NULLPTR);
	void resetCapture(QChar pref, QChar suff);
	void activate();
	void deactivate();
	static BarcodeObserver* instanse();
signals:
	void barcodeCaught(QString);
	void prefixCaught();
	void suffixCaught();

};





