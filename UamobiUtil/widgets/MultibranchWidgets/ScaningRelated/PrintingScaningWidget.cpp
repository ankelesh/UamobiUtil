#include "PrintingScaningWidget.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#include "widgets/utils/ElementsStyles.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/ExtendedLabels.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/ExtendedDialogs.h"
#include "PrinterWrappers/AbsPrinterWrapper.h"
#include "PrinterWrappers/PrinterWrapperFactory.h"
void PrintingScaningWidget::_print(QString text)
{
	if (printerWrapper->isValid() && printerWrapper->isReady())
	{
		printerWrapper->print(text);
	}
	else
	{
		printerWrapper->retryOpeningConnection();
	}
}
void PrintingScaningWidget::_handleRecord(RecEntity)
{

}

void PrintingScaningWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	switch (overloads.count())
	{
	default:
	case 3:
	{
		QStringList t;
		t << "date" << "parent" << "comment";
		localCache.insert(receiptNewDocument, overloads.at(2).assertedAndMappedCopy(
			QueryTemplates::receiptNewDocument,
			t, t
		));
        Q_FALLTHROUGH();
	}
	case 2:
	{
		QStringList t;
		t << "barcode" << "control1" << "printer";
		localCache.insert(docGetItemLabel, overloads.at(1).assertedAndMappedCopy(docGetItemLabel,
			t,
			t));
        Q_FALLTHROUGH();
	}
	case 1:
	{
		QStringList t;
        t << "barcode";
		localCache.insert(getItemInfo, overloads.at(0).assertedAndMappedCopy(
			QueryTemplates::getItemInfo,
			t, t));
        Q_FALLTHROUGH();
	}
	case 0:
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
        Q_FALLTHROUGH();
	case 2:
	{
		QStringList t;
		t << "barcode" << "control1" << "printer";
		localCache.insert(docGetItemLabel, OverloadableQuery(docGetItemLabel,
			t, t));
	}
        Q_FALLTHROUGH();
	case 1:
	{
		QStringList t;
		t << "date" << "parent" << "comment";
		localCache.insert(receiptNewDocument, OverloadableQuery(
			QueryTemplates::receiptNewDocument,
			t,
			t));
	}
        Q_FALLTHROUGH();
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
	userInfo->clear();
	QStringList buffer;
	buffer << barcodeField->text();
	buffer << ((first_control.isNull()) ? "" : first_control->getValue());
	buffer << AppSettings->printerType;
	AppNetwork->execQueryByTemplate(localCache[docGetItemLabel], 3, buffer, awaiter);
    awaiter->deliverResultTo(docGetItemLabel);
	wipe();
}

void PrintingScaningWidget::backNeeded()
{
	emit backRequired();
}

void PrintingScaningWidget::item_confirmed_response()
{
    if (!awaiter->deliverHere(docGetItemLabel))
        return;
	ResponseParser  parser(new RichtextResponseParser(awaiter->restext, awaiter->errtext));
	if (!assertAndShowError(this, parser, tr("Error!")))
	{
		_print(parser->read().first()->value("code"));
    }
    hideProcessingOverlay();
}

void PrintingScaningWidget::barcodeConfirmed()
{
    if (awaiter->isAwaiting())
        return;
    showProcessingOverlay();
	userInfo->clear();
    AppNetwork->execQueryByTemplate(localCache[getItemInfo], barcodeField->text(), awaiter);
    awaiter->deliverResultTo(getItemInfo);
}

void PrintingScaningWidget::wipe()
{
    if (!first_control.isNull())
    {
        first_control->setValue("1");
    }
    if (!second_control.isNull())
    {
        second_control->setValue("1");
    }
}

PrintingScaningWidget::PrintingScaningWidget(QWidget* parent, IndependentBranchNode* searchScr)
	: NormalScaningWidget(parent, Q_NULLPTR, searchScr), printerWrapper(PrinterWrapperFactory::fabricate(this)),
	connectionState(new SemaphorLabel(this))

{
#ifdef DEBUG
	detrace_DCONSTR("PrintingScaning");
#endif
	topPanelLayout->addWidget(connectionState);
	connectionState->setState(SemaphorLabel::opfail);
	connectionState->setText(tr("printer"));
	quitButton->hide();
	userInfo->setMaximumHeight(calculateAdaptiveButtonHeight());
    quitButton->setMaximumHeight(calculateAdaptiveButtonHeight());
#ifdef QT_VERSION5X
	QObject::connect(printerWrapper, &AbsPrinterWrapper::error, this, &PrintingScaningWidget::wrapperError);
	QObject::connect(printerWrapper, &AbsPrinterWrapper::connected, this, &PrintingScaningWidget::wrapperOk);
#else
    QObject::connect(printerWrapper, SIGNAL(error(QString)), this, SLOT(wrapperError(QString)));
    QObject::connect(printerWrapper, SIGNAL(connected()), this, SLOT(wrapperOk()));
#endif
	printerWrapper->establishConnection();
}

PrintingScaningWidget::~PrintingScaningWidget()
{
}

void PrintingScaningWidget::wrapperOk()
{
    connectionState->setState(SemaphorLabel::opsuccess);
}

void PrintingScaningWidget::wrapperError(QString errtext)
{
	connectionState->setState(SemaphorLabel::opfail);
    if (!isVisible())
        QTimer::singleShot(100, this, SLOT(reshowError()));
    else
        ErrorMessageDialog::showErrorInfo( tr("Printer error"), errtext);
}

void PrintingScaningWidget::reshowError()
{
    ErrorMessageDialog::showErrorInfo(tr("Printer error"), printerWrapper->errors());
}
