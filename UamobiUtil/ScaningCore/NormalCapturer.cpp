#include "NormalCapturer.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
NormalCapturer::NormalCapturer(captureInterface* whereToApply, QObject * parent)
	: QObject(parent), widgetToApply(whereToApply), isScaning(false), isAwaitingControlValue(false),
	keyfilter(new filters::NoKeyEvents(this)),
	lastKeyReleaseTimepoint(), controlIndex(0), controlNumber(0)
{
	parent->installEventFilter(keyfilter);
#ifdef QT_VERSION5X
	QObject::connect(keyfilter, &filters::NoKeyEvents::returnObtained, this, &NormalCapturer::pressReturn);
	QObject::connect(keyfilter, &filters::NoKeyEvents::unknownObtained, this, &NormalCapturer::pressScan);
	QObject::connect(keyfilter, &filters::NoKeyEvents::numberObtained, this, &NormalCapturer::handleElement);
	QObject::connect(keyfilter, &filters::NoKeyEvents::backRequired, this, &NormalCapturer::handleBack);
#else
    QObject::connect(keyfilter, SIGNAL(returnObtained()), this, SLOT(pressReturn()));
    QObject::connect(keyfilter, SIGNAL(unknownObtained()), this, SLOT(pressScan()));
    QObject::connect(keyfilter, SIGNAL(numberObtained(QString)), this, SLOT(handleElement(QString)));
    QObject::connect(keyfilter, SIGNAL(backRequired()), this, SLOT(handleBack()));
#endif
}

void NormalCapturer::pressScan()
{
    if (isScaning)
		return;
    if (widgetToApply->isManualInFocus())
        widgetToApply->removeManualFocus();
	if (!widgetToApply->barcodeBuffer.isEmpty())
	{
		widgetToApply->handleScannedBarcode();
	}
	clearCaptureEngine();
}

void NormalCapturer::pressReturn()
{
    if (widgetToApply->isManualInFocus())
        return;
#ifdef DEBUG
    detrace_METHEXPL("return captured in capturer while buffer was " << widgetToApply->barcodeBuffer);
#endif
    if (isScaning)
	{
        if (widgetToApply->barcodeBuffer.isEmpty())
            return;
 #ifdef DEBUG
        detrace_METHEXPL("before calling handler from capturer: " << widgetToApply->barcodeBuffer);
#endif
        widgetToApply->handleScannedBarcode();
        isScaning = false;
        isAwaitingControlValue = true;
		controlIndex = 0;
		widgetToApply->setControlFocus(controlIndex);
	}
	else
		if (isAwaitingControlValue)
		{
			++controlIndex;
			if (!widgetToApply->numberBuffer.isEmpty())
			{
				widgetToApply->handleNumberInbuffer();
			}
		}
		else
		{
			
		}
}

void NormalCapturer::handleElement(QString elem)
{
    if (widgetToApply->isManualInFocus())
    {
        return;
    }
	if (isScaning)
	{
#ifdef DEBUG
		detrace_METHEXPL("got value " << elem << " in scaning phase of capturer");
#endif
		
		if (lastKeyReleaseTimepoint.msecsTo(QTime::currentTime()) < 30)
		{
			widgetToApply->barcodeBuffer += elem;
			lastKeyReleaseTimepoint = QTime::currentTime();
		}
		else
		{
			widgetToApply->barcodeBuffer.clear();
			widgetToApply->barcodeBuffer.reserve(15);
			widgetToApply->barcodeBuffer += elem;
			lastKeyReleaseTimepoint = QTime::currentTime();
		}

	}
	else if (isAwaitingControlValue)
	{
		if (lastKeyReleaseTimepoint.msecsTo(QTime::currentTime()) > 30)
		{
			widgetToApply->numberBuffer += elem;
			lastKeyReleaseTimepoint = QTime::currentTime();
		}
		else
		{
			pressScan();
			widgetToApply->barcodeBuffer += elem;
		}
	}
	else
	{
        pressScan();
	}
}

void NormalCapturer::skipControls()
{
}

void NormalCapturer::clearCaptureEngine()
{
	isScaning = true;
	isAwaitingControlValue = false;
	widgetToApply->barcodeBuffer.clear();
	widgetToApply->barcodeBuffer.reserve(15);
	widgetToApply->numberBuffer.clear();
	controlIndex = 0;
	lastKeyReleaseTimepoint = QTime::currentTime();
}

void NormalCapturer::handleBack()
{
	widgetToApply->processBackPress();
}

void NormalCapturer::setPhase(int ph)
{
	if (ph = -1)
	{
		clearCaptureEngine();
		pressScan();
	}
	else if (ph < controlNumber)
	{
		isAwaitingControlValue = true;
		isScaning = false;
		controlIndex = ph;
	}

}
