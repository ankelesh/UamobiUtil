#pragma once
#include "widgets/utils/EventsAndFilters.h"

#include <QTime>




class captureInterface
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
	virtual bool isControlFocused()=0;
};


class NormalCapturer : public QObject
{
    Q_OBJECT
private:
	// control vars
	captureInterface* widgetToApply;
	bool isScaning;
	bool isAwaitingControlValue;
protected:
	QTime lastKeyReleaseTimepoint;
	int controlIndex;
	int controlNumber;

	

public:
    filters::NoKeyEvents* keyfilter;
	explicit NormalCapturer(captureInterface* inter, QObject * whereApply);
	void skipControls(int q);
    void clearCaptureEngine();
	void setPhase(int);
	void setControlNumber(int);
    QString stateOfEngine();
protected slots:
	void pressScan();
	void pressReturn();
	void handleElement(QString);
    void handleBack();
	void handleErase();
	
};
