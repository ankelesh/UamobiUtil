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
	int numberCatchingActive;
	static BarcodeObserver* _instanse;

	bool eventFilter(QObject* object, QEvent* event);
	bool _testPressureForBarcode(QKeyEvent* ev, QObject* target);
	bool _testReleaseForBarcode(QEvent* ev);
	bool _testGlobalHotkeyPress(QKeyEvent* ev, QObject* target);
	bool _testGlobalHotkeyRelease(QKeyEvent* ev, QObject* target);
	bool _testNumberReleased(QKeyEvent* ev);
public:
	BarcodeObserver(QChar pref, QChar suff, QObject* parent = Q_NULLPTR);
	void resetCapture(QChar pref, QChar suff);
	void activate();
	void deactivate();
	void catchNumbers();
	void stopNumberCatching();
	bool activated();
	static BarcodeObserver* instanse();

	static void init();
signals:
	void barcodeCaught(QString);
	void prefixCaught();
	void suffixCaught();
	void escapeCaught();
	void arrowCaught(int arrow);
	void numberPressed(int num);
	void returnCaught();
};





