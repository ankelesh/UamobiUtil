#include "PrintingScaningWidget.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "debugtrace.h"
#include "widgets/utils/ElementsStyles.h"

void PrintingScaningWidget::_print(QString text)
{
#ifdef FTR_COM
	if (!printerSocket.isConnected())
	{
		printerSocket.connect(AppSettings->printerPortDesignation, AppSettings->printerPort);
	}
	if (!printerSocket.isConnected())
	{
		userInfo->setText(tr("Connection to printer failed! Check your port settings."));
		return;
	}
	if (!printerSocket.write(encoder.fromUnicode(text)))
	{
		userInfo->setText(tr("printing failed: ") + printerSocket.getErrors());
	}
#else
#ifdef DEBUG
	detrace_METHPERROR("_print", "seems here is no printer connection, data to print: " << text);
#endif
	userInfo->setText(tr("This build does not supports printing!"));
#endif
}
void PrintingScaningWidget::_handleRecord(RecEntity)
{

}

void PrintingScaningWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	switch (
		((overloads.count() > 3) ? 3 : overloads.count())
		)
	{
	case 3:
	{
		QStringList t;
		t << "date" << "parent" << "comment";
		localCache.insert(receiptNewDocument, overloads.at(2).assertedAndMappedCopy(
			QueryTemplates::receiptNewDocument,
			t, t
		));
	}
	case 2:
	{
		QStringList t;
		t << "barcode" << "control1" << "printer";
		localCache.insert(docGetItemLabel, overloads.at(1).assertedAndMappedCopy(docGetItemLabel,
			t,
			t));
	}
	case 1:
	{
		QStringList t;
        t << "barcode";
		localCache.insert(getItemInfo, overloads.at(0).assertedAndMappedCopy(
			QueryTemplates::getItemInfo,
			t, t));
	}
	default:
		break;
	}
	switch (3 - overloads.count())
	{
	case 3:
	{
		QStringList t;
        t << "barcode";
		localCache.insert(getItemInfo, OverloadableQuery(
            "doc_get_item_info&session=%1&barcode=%2",
			t, t));
	}
	case 2:
	{
		QStringList t;
		t << "barcode" << "control1" << "printer";
		localCache.insert(docGetItemLabel, OverloadableQuery(docGetItemLabel,
			t, t));
	}
	case 1:
	{
		QStringList t;
		t << "date" << "parent" << "comment";
		localCache.insert(receiptNewDocument, OverloadableQuery(
			QueryTemplates::receiptNewDocument,
			t,
			t));
	}
	default:
		return;
	}
}

void PrintingScaningWidget::submitPressed()
{
	if (awaiter->isAwaiting())
	{
		return;
	}
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &PrintingScaningWidget::item_confirmed_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(item_confirmed_response()));
#endif
	QStringList buffer;
	buffer << barcodeField->text();
	buffer << ((first_control.isNull()) ? "" : first_control->getValue());
	buffer << AppSettings->printerType;
	AppNetwork->execQueryByTemplate(localCache[docGetItemLabel], 3, buffer, awaiter);
	wipe();
}

void PrintingScaningWidget::backNeeded()
{
	emit backRequired();
}

void PrintingScaningWidget::item_confirmed_response()
{
	ResponseParser  parser(new RichtextResponseParser(awaiter->restext, awaiter->errtext));
	if (!parser->isSuccessfull())
	{
		userInfo->setText(parser->getErrors());
	}
	else
	{
		_print(parser->read().first()->value("code"));
	}
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
    hideProcessingOverlay();
}

void PrintingScaningWidget::barcodeConfirmed()
{
    if (awaiter->isAwaiting())
        return;
    showProcessingOverlay();
#ifdef QT_VERSION5X
    QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &PrintingScaningWidget::item_scaned_response);
#else
    QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(item_scaned_response()));
#endif
    AppNetwork->execQueryByTemplate(localCache[getItemInfo], barcodeField->text(), awaiter);
}

void PrintingScaningWidget::wipe()
{
}

PrintingScaningWidget::PrintingScaningWidget(QWidget* parent, IndependentBranchNode* searchScr)
	: NormalScaningWidget(parent, Q_NULLPTR, searchScr)
#ifdef FTR_COM
    , printerSocket(), encoder(QTextCodec::codecForName("CP1251"))
#endif
{
#ifdef FTR_COM
#ifdef DEBUG
	detrace_METHEXPL("connecting to printer: " << AppSettings->printerPortDesignation << AppSettings->printerPort);
#endif
	printerSocket.connect(AppSettings->printerPortDesignation, AppSettings->printerPort);
#endif
	quitButton->hide();
	userInfo->setMaximumHeight(calculateAdaptiveButtonHeight());
	quitButton->setMaximumHeight(calculateAdaptiveButtonHeight());
}

PrintingScaningWidget::~PrintingScaningWidget()
{
#ifdef FTR_COM
	printerSocket.disconnect();
#endif
}
