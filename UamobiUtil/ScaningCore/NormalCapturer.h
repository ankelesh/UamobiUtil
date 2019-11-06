#pragma once
#include "widgets/utils/EventsAndFilters.h"

#include <QTime>

/*
	This file contains interface that must be inherited for all objects that require capturer and simple capturer that
	tries to be reusable.

*/

class captureInterface
	// this interface provides two buffers for storing temp values and
	// a lot of pure virtual functions that are required for handling values retrived by capturer
{
public:
	QString barcodeBuffer;
	QString numberBuffer;
	virtual bool isManualInFocus() = 0;
	virtual bool handleScannedBarcode() = 0;
	virtual bool handleNumberInbuffer() = 0;
	virtual int flushControl(int) = 0;
	virtual void processBackPress() = 0;
	virtual void removeManualFocus() = 0;
	virtual void setControlFocus(int) = 0;
	virtual bool isControlFocused() = 0;
};

class NormalCapturer : public QObject
	// This class is working as keyfilter, but also it processes all key presses and releases in it
{
	Q_OBJECT
private:
	// control vars
	captureInterface* widgetToApply;	//	pointer to widget which is recepient of barcodes
	bool isScaning;			//	flag defining that this capturer is awaiting scaning values
	bool isAwaitingControlValue;	//	flag defining that this capturer is awaiting values for control
protected:
	QTime lastKeyReleaseTimepoint;	//	timepoint used for defining where to put obtained keys
	int controlIndex;				//	index of current control
	int controlNumber;				//	number of total controls

public:
	filters::NoKeyEvents* keyfilter; // filter which is allocated on all widgets that can be in focus
	explicit NormalCapturer(captureInterface* inter, QObject* whereApply);
	void skipControls(int q);	//	moves index to q
	void clearCaptureEngine();	//	clears engine by resetting it to scaning phase
	void setPhase(int);			//	sets phase where -1 is scaning and any + value is new index
	void setControlNumber(int);	//	sets total number of controls
	QString stateOfEngine();	//	debugging method which returns description of all variables
protected slots:
	void pressScan();			//	triggered on buttons that can be SCAN
	void pressReturn();			//	triggered on return
	void handleElement(QString);	//	triggered on any non-control elem
	void handleBack();			//	triggered on esc\back buttons
	void handleErase();			//	triggered on backspace
};
