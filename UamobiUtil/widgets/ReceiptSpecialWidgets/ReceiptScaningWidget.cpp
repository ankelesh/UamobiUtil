#include "ReceiptScaningWidget.h"
#include "debugtrace.h"
#include <QApplication>
#ifdef QT_VERSION5X
#include <QtWidgets/QScroller>
#else
#include "legacy/qtCompatibility/scrollgrabber.h"
#endif
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#define DEBUG
ReceiptScaningWidget::ReceiptScaningWidget(GlobalAppSettings& go, QWidget* parent)
	: AbstractScaningWidget(go, parent),abstractNode(), captureInterface(), resultScreen(new DocResultsWidget(go,this)),
	searchScreen(new ItemSearchWidget(go,this)), capturer(new NormalCapturer(this, this)),
	controlsRequired(false), manSelected(false)
{
	mainLayout->addWidget(searchScreen);
	mainLayout->addWidget(resultScreen);
	searchScreen->hide();
	resultScreen->hide();
	current = innerWidget;
	untouchable = innerWidget;
	main = this;
	submitButton->setDisabled(true);
    mainTextView->installEventFilter(capturer->keyfilter);
	QScroller::grabGesture(mainTextView, QScroller::LeftMouseButtonGesture);
    innerWidget->installEventFilter(capturer->keyfilter);
    barcodeField->installEventFilter(new filters::LineEditHelper(this));
#ifdef QT_VERSION5X
	QObject::connect(resultScreen, &DocResultsWidget::backRequired, this, &ReceiptScaningWidget::hideCurrent);
	QObject::connect(searchScreen, &ItemSearchWidget::backRequired, this, &ReceiptScaningWidget::hideCurrent);
	QObject::connect(searchScreen, &ItemSearchWidget::itemSelected, this, &ReceiptScaningWidget::itemObtained);
	QObject::connect(resultScreen, &DocResultsWidget::documentSaved, this, &ReceiptScaningWidget::saveSuccesfull);
#else
    QObject::connect(resultScreen, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
    QObject::connect(searchScreen, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
    QObject::connect(searchScreen, SIGNAL(itemSelected(parsedItemSimplified)), this, SLOT(itemObtained(parsedItemSimplified)));
    QObject::connect(resultScreen, SIGNAL(documentSaved()), this, SLOT(saveSuccesfull()));
#endif
}


void ReceiptScaningWidget::submitPressed()
{
	if (!controlsRequired || awaiter.isAwaiting())
	{
		return;
	}
	if (first_control == Q_NULLPTR || controlsAvailable == 0)
	{ 
		return;
	}
	if (!first_control->canGiveValue())
        return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &ReceiptScaningWidget::item_confirmed_response);
#else
    QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(item_confirmed_response()));
#endif
	switch (controlsAvailable)
	{
	case 1:
		globalSettings.networkingEngine->recSubmit(barcodeField->text(), first_control->getValue(), &awaiter, RECEIVER_SLOT_NAME);
		break;
	case 2:
		globalSettings.networkingEngine->recSubmit(barcodeField->text(), first_control->getValue(), makeParamsFromList(second_control->name, second_control->getValue()), &awaiter, RECEIVER_SLOT_NAME);
		break;
	default:
		break;
	}
	awaiter.run();
	first_control->reset();
	barcodeField->clear();
}

void ReceiptScaningWidget::processNumber(QString num)
{
	barcodeField->setText(barcodeField->text() + num);
}

void ReceiptScaningWidget::processBarcode(QString bc)
{
    //detrace_METHEXPL("processing barcode as " << bc << " while replacing " << barcodeField->text());
    barcodeField->clear();
	barcodeField->setText(bc);
	barcodeConfirmed();
}

void ReceiptScaningWidget::barcodeConfirmed()
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
    this->setFocus();

#ifdef DEBUG
	detrace_METHEXPL("got text " << barcodeField->text() << " while preparing data");
#endif
#ifdef QT_VERSION5X
    QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &ReceiptScaningWidget::item_scaned_response);
#else
    QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(item_scaned_response()));
#endif
    globalSettings.networkingEngine->itemGetInfo(barcodeField->text(), &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
}

void ReceiptScaningWidget::setDocument(parsedOrder po)
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &ReceiptScaningWidget::document_confirmed_response);
#else
    QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(document_confirmed_response()));
#endif
    globalSettings.networkingEngine->recNew(QDate::currentDate(), po.code, "", &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	
}

void ReceiptScaningWidget::hideCurrent()
{
	_hideCurrent(innerWidget);
}

void ReceiptScaningWidget::itemObtained(parsedItemSimplified item)
{
	barcodeField->setText(item.barcode);
	barcodeConfirmed();
	_hideCurrent(innerWidget);
}

void ReceiptScaningWidget::saveSuccesfull()
{
	emit backRequired();
}

bool ReceiptScaningWidget::isManualInFocus()
{
	return barcodeField->hasFocus();
}

bool ReceiptScaningWidget::handleScannedBarcode()
{
	if (!barcodeBuffer.isEmpty()) {
		processBarcode(barcodeBuffer);

		return true;
	}
	return false;
}

bool ReceiptScaningWidget::handleNumberInbuffer()
{
	if (controlsRequired)
	{
		switch (controlsAvailable)
		{
		case 2:
			second_control->refresh();
		case 1:
			first_control->refresh();
			return true;
		default:
			return false;
		}
	}
	return false;
}

void ReceiptScaningWidget::processBackPress()
{
    emit backRequired();
}

void ReceiptScaningWidget::removeManualFocus()
{
    this->setFocus();
	manSelected = false;
}

void ReceiptScaningWidget::setControlFocus(int val)
{
	if (controlsRequired)
	{
		switch (val)
		{
		case 0:
			if (controlsAvailable >= 1)
			{
				first_control->setFocus();
			}
			return;
		case 1:
			if (controlsAvailable >= 2)
			{
				second_control->setFocus();
			}
		default:
			return;
		}
	}
}

int ReceiptScaningWidget::flushControl(int cnum)
{
	if (controlsAvailable == 1)
		submitPressed();
	else
	{
		switch (cnum)
		{
		case 0:
			first_control->unassociateBuffer();
			second_control->associateBuffer(numberBuffer);
			numberBuffer.clear();
			second_control->setAwaiting();
			break;
		case 1:
			submitPressed();
		}
	}
	return cnum+1;
}

void ReceiptScaningWidget::switchedFocus()
{
	if (controlsRequired)
	{
		if (controlsAvailable >= 1)
		{
			first_control->setFocus();
			manSelected = true;
		}
	}
	else
	{
		if (manSelected)
		{
			this->setFocus();
		}
		else
		{
			barcodeField->setFocus();
		}
		manSelected = !manSelected;
	}
}

void ReceiptScaningWidget::syncControlAndBuffer(QString v)
{
	numberBuffer = v;
}

bool ReceiptScaningWidget::isControlFocused()
{
	switch (controlsAvailable)
	{
	case 1:
		return first_control->hasFocus();
	case 2:
		return first_control->hasFocus() || second_control->hasFocus();
	default:
		return false;
	}
}


void ReceiptScaningWidget::searchRequired()
{
	_hideAny(searchScreen);
}

void ReceiptScaningWidget::backNeeded()
{
	_hideAny(resultScreen);
}

void ReceiptScaningWidget::useControls(QVector<QPair<QString, QString> >& cvals)
{
	detrace_METHCALL("useControls, cVars: " << controlsAvailable << " & " << controlsRequired);
	numberBuffer.clear();
	switch (cvals.length())
	{
	case 0:
		switch (controlsAvailable)
		{
		case 2:
			detrace_METHEXPL("deleting second_control, cvs " << controlsAvailable << " & " <<controlsRequired );
			innerLayout->removeWidget(second_control->myWidget());
			delete second_control;
			second_control = Q_NULLPTR;
		case 1:
			detrace_METHEXPL("deleting first_control, cvs " << controlsAvailable << " & " << controlsRequired);
			innerLayout->removeWidget(first_control->myWidget());
			delete first_control;
			first_control = Q_NULLPTR;
		case 0:

			detrace_METHEXPL("resetting vars");
			setFocus();
			controlsRequired = false;
			submitButton->setDisabled(true);
			controlsAvailable = 0;
			controlsRequired = false;
		}
		return;
	case 1:

		if (controlsAvailable >= 1)
		{
			if (first_control->name == cvals.at(0).first)
			{
				first_control->reset();
				first_control->show();
			}
			else
			{
				detrace_METHEXPL("deleting first_control, cvs " << controlsAvailable << " & " << controlsRequired);
				innerLayout->removeWidget(first_control->myWidget());
				delete first_control;
				first_control = fabricateControl(numberBuffer, cvals.at(0).first, innerLayout, innerWidget);
				first_control->installEventFilter(keyfilter);
			}
			if (second_control != Q_NULLPTR)
			{
				detrace_METHEXPL("deleting second_control, cvs " << controlsAvailable << " & " << controlsRequired);
				innerLayout->removeWidget(second_control->myWidget());
				delete second_control;
				second_control = Q_NULLPTR;
				--controlsAvailable;
			}
		}
		else
		{
			first_control = fabricateControl(numberBuffer, cvals.at(0).first, innerLayout, innerWidget);
			first_control->installEventFilter(keyfilter);
			++controlsAvailable;
		}
		capturer->setControlNumber(1);
		first_control->setAwaiting();
		first_control->show();
		controlsRequired = true;
		submitButton->setDisabled(false);
		this->setFocus();
		return;
	case 2:
	default:
		switch (controlsAvailable)
		{
		case 2:
			if (!(cvals.at(1).first == second_control->name))
			{
				detrace_METHEXPL("deleting second_control, cvs " << controlsAvailable << " & " << controlsRequired);
				innerLayout->removeWidget(second_control->myWidget());
				delete second_control;
				
				second_control = Q_NULLPTR;
			}
		case 1:
			if (!(cvals.at(1).first == first_control->name))
			{
				detrace_METHEXPL("deleting first_control, cvs " << controlsAvailable << " & " << controlsRequired);
				innerLayout->removeWidget(first_control->myWidget());
				delete first_control;
				first_control = Q_NULLPTR;
			}
		case 0:
			if (first_control == Q_NULLPTR)
			{

				first_control = fabricateControl(numberBuffer, cvals.at(0).first, innerLayout, innerWidget);
				first_control->installEventFilter(keyfilter);
			}
			else
			{
				first_control->reset();
			}
			if (second_control == Q_NULLPTR)
			{
				second_control = fabricateControl(cvals.at(1).first, innerLayout, innerWidget);
				second_control->installEventFilter(keyfilter);
			}
			else
			{
				second_control->reset();
			}
		}
		capturer->setControlNumber(2);
		controlsAvailable = 2;
	}
	first_control->setAwaiting();
	controlsRequired = true;
	submitButton->setDisabled(false);
	this->setFocus();
	first_control->show();
	second_control->show();
}

void ReceiptScaningWidget::item_scaned_response()
{
	parse_uniresults_functions::CombinedNamedLinearResponse resp = RequestParser::interpretAsItemInfo(awaiter.restext, awaiter.errtext);
	mainTextView->setText(resp.named.value("richdata"));
	itemSuppliedValues = resp.named;
	useControls(resp.linear);
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void ReceiptScaningWidget::item_confirmed_response()
{
	parse_uniresults_functions::CombinedNamedLinearResponse resp = RequestParser::interpretAsItemInfo(awaiter.restext, awaiter.errtext);
	itemSuppliedValues = resp.named;
	mainTextView->setText(itemSuppliedValues.value("richdata"));
	useControls(resp.linear);
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
	if (!barcodeField->text().isEmpty())
	{
		barcodeConfirmed();
	}
}

void ReceiptScaningWidget::document_confirmed_response()
{
	document = RequestParser::interpretAsDocumentResponse(awaiter.restext, awaiter.errtext);
	userInfo->setText(tr("receipt_scaning_mode_name") + "(" + document.docId + ")\n" + document.supplier);
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void ReceiptScaningWidget::was_timeout()
{
	AbstractScaningWidget::was_timeout();
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}
