#include "NormalCapturer.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
NormalCapturer::NormalCapturer(captureInterface* whereToApply, QObject* parent)
	: QObject(parent), widgetToApply(whereToApply), isScaning(false), isAwaitingControlValue(false),
	keyfilter(new filters::NoKeyEvents(this)),
	lastKeyReleaseTimepoint(), controlIndex(0), controlNumber(0)
{
#ifdef DEBUG
	// detrace_DCONSTR("NormalCapturer");
#endif
	parent->installEventFilter(keyfilter);

#ifdef QT_VERSION5X
#ifdef DEBUG
	bool state;
	state =
#endif
		QObject::connect(keyfilter, &filters::NoKeyEvents::returnObtained, this, &NormalCapturer::pressReturn);
#ifdef DEBUG
	//detrace_CONNECTSTAT("returnObtained->pressReturn", state);
	state =
#endif

		QObject::connect(keyfilter, &filters::NoKeyEvents::unknownObtained, this, &NormalCapturer::pressScan);
#ifdef DEBUG
	//detrace_CONNECTSTAT("unknownObtained->pressScan", state);
	state =
#endif
		QObject::connect(keyfilter, &filters::NoKeyEvents::numberObtained, this, &NormalCapturer::handleElement);
#ifdef DEBUG
	//detrace_CONNECTSTAT("numberObtainerd->handleElement", state);
	state =
#endif
		QObject::connect(keyfilter, &filters::NoKeyEvents::backRequired, this, &NormalCapturer::handleBack);
	QObject::connect(keyfilter, &filters::NoKeyEvents::eraseRequired, this, &NormalCapturer::handleErase);
#ifdef DEBUG
	//detrace_CONNECTSTAT("backRequired->handleBack", state);
#endif
#else
	bool state = true;
	state &= QObject::connect(keyfilter, SIGNAL(returnObtained()), this, SLOT(pressReturn()));
	state &= QObject::connect(keyfilter, SIGNAL(unknownObtained()), this, SLOT(pressScan()));
	state &= QObject::connect(keyfilter, SIGNAL(numberObtained(QString)), this, SLOT(handleElement(QString)));
	state &= QObject::connect(keyfilter, SIGNAL(backRequired()), this, SLOT(handleBack()));
	state &= QObject::connect(keyfilter, SIGNAL(eraseRequired()), this, SLOT(handleErase()));
#ifdef DEBUG
	detrace_CONNECTSTAT("Capturer slots", state);
#endif
#endif
}

void NormalCapturer::skipControls(int q)
{
	controlIndex += q;
	if (controlIndex >= controlNumber)
	{
		pressScan();
	}
}

void NormalCapturer::pressScan()
{
#ifdef DEBUG
	detrace_METHCALL("NormalCapturer::pressScan");
#endif

	if (isScaning)
		return;
	if (widgetToApply->isManualInFocus())
	{
#ifdef DEBUG
		detrace_METHCALL("removeManualFocus");
#endif

		widgetToApply->removeManualFocus();
	}
	if (!widgetToApply->barcodeBuffer.isEmpty())
	{
#ifdef DEBUG
		detrace_METHCALL("handleScannedBarcode");
#endif

		widgetToApply->handleScannedBarcode();
	}
	if (isAwaitingControlValue)
	{
#ifdef DEBUG
		detrace_METHEXPL("flushing Q");
#endif
		if (widgetToApply->numberBuffer.isEmpty())
		{
#ifdef DEBUG
			detrace_METHEXPL("empty buffer! aborting!");
#endif
			clearCaptureEngine();
			return;
		}
		widgetToApply->handleNumberInbuffer();
		widgetToApply->flushControl(controlIndex);
		isScaning = true;
	}
#ifdef DEBUG
	detrace_METHEXPL("passed all checks, clearing");
#endif
	clearCaptureEngine();
}

void NormalCapturer::pressReturn()
{
#ifdef DEBUG
	detrace_METHCALL("NormalCapturer::pressReturn");
#endif

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

#ifdef DEBUG
		detrace_METHINVOK("handleScannedBarcode", "WidgetToApply", " press return", "capturer");
#endif

		widgetToApply->handleScannedBarcode();
		isScaning = false;
		widgetToApply->barcodeBuffer.clear();
		isAwaitingControlValue = true;
		controlIndex = 0;

#ifdef DEBUG
		detrace_METHINVOK("setControlFocus", "WidgetToApply", " press return", "capturer");
#endif

		widgetToApply->setControlFocus(controlIndex);
	}
	else
		if (isAwaitingControlValue)
		{

#ifdef DEBUG
			detrace_METHEXPL("flushing control #" << controlIndex << " from " << controlNumber);
#endif

			controlIndex = widgetToApply->flushControl(controlIndex);
			if (controlIndex >= controlNumber)
			{
				pressScan();
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
		detrace_METHEXPL(elem);
#endif

		if (lastKeyReleaseTimepoint.msecsTo(QTime::currentTime()) < 50)
		{
			widgetToApply->barcodeBuffer += elem;
			lastKeyReleaseTimepoint = QTime::currentTime();

#ifdef DEBUG
			detrace_METHEXPL("toBarcode");
#endif
		}
		else
		{
			widgetToApply->barcodeBuffer.clear();
			widgetToApply->barcodeBuffer.reserve(15);
			widgetToApply->barcodeBuffer += elem;

#ifdef DEBUG
			detrace_METHEXPL("cleaningTobarcode");
#endif

			lastKeyReleaseTimepoint = QTime::currentTime();
		}
	}
	else if (isAwaitingControlValue)
	{
		if (lastKeyReleaseTimepoint.msecsTo(QTime::currentTime()) > 50)
		{
#ifdef DEBUG
			detrace_METHEXPL("toNum");
#endif
			if (elem == ".")
			{
				if (widgetToApply->numberBuffer.contains("."))
				{
					lastKeyReleaseTimepoint = QTime::currentTime();
					return;
				}
			}
			widgetToApply->numberBuffer += elem;
			widgetToApply->handleNumberInbuffer();
			lastKeyReleaseTimepoint = QTime::currentTime();
		}
		else
		{
			lastKeyReleaseTimepoint = QTime::currentTime();
		}
	}
	else
    {
       // detrace_METHEXPL("pressing scan during non-scan");
        //pressScan();
        lastKeyReleaseTimepoint = QTime::currentTime();
	}
}

void NormalCapturer::clearCaptureEngine()
{
#ifdef DEBUG
	detrace_METHCALL("NormalCapturer::clearCaptureEngine");
#endif

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

void NormalCapturer::handleErase()
{
	if (!widgetToApply->numberBuffer.isEmpty())
	{
		widgetToApply->numberBuffer.chop(1);
		widgetToApply->handleNumberInbuffer();
	}
}

void NormalCapturer::setPhase(int ph)
{
#ifdef DEBUG
	detrace_METHEXPL("ph set: " << ph << "|" << controlNumber);
#endif
	if (ph == -1)
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

#ifdef DEBUG
	detrace_METHEXPL(stateOfEngine());
#endif

}

void NormalCapturer::setControlNumber(int num)
{
	controlNumber = num;
	if (controlIndex >= num)
		controlIndex = 0;
}

QString NormalCapturer::stateOfEngine()
{
	return QString("State: ") + ((isScaning) ? "Scaning" : ((isAwaitingControlValue) ? "Control" : "other"))
		+ " and control index " + QString::number(controlIndex) + " BBuffer:|" +
		widgetToApply->barcodeBuffer + " NBuffer: |" + widgetToApply->numberBuffer + "\n";
}
