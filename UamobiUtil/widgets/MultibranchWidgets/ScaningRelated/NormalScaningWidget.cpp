#include "NormalScaningWidget.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
// Qt 5 only imports
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
#include "widgets/utils/ElementsStyles.h"
#include "submodules/UNAQtCommons/barcodeHandling/BarcodeObserver.h"
#include "widgets/BranchingTools/BranchException.h"
#include "datacore/EntityListEntity.h"
void NormalScaningWidget::_handleRecord(RecEntity e)
{
	if (e.isNull())
	{
#ifdef DEBUG
		detrace_METHPERROR("_handleRecord", "Null record given");
#endif
		return;
	}
	if (e->myType() == UniformXmlObject::Order)
	{
		setDocument(upcastRecord<OrderEntity>(e));
	}
	else if (e->myType() == UniformXmlObject::Document)
	{
		setDocument(upcastRecord<FullDocumentEntity>(e));
	}
	else
	{
		setDocument(e);
	}
}

NormalScaningWidget::NormalScaningWidget(QWidget* parent, IndependentBranchNode* resScreen,
	IndependentBranchNode* searchScr)
	: AbstractScaningWidget(independent_nodes::NormalScaning, parent), 
	abstractNode(),
	resultScreen(resScreen), 
	searchScreen(searchScr),
	localCache()
{
#ifdef DEBUG
	detrace_DCONSTR("NormalScaning");
#endif

	if (resultScreen == Q_NULLPTR)
		resultScreen = new DocResultsWidget(this);
	else
		resultScreen->setParent(this);
	if (searchScreen == Q_NULLPTR)
		searchScreen = new PagedSearchWidget(RecEntity(new ShortItemEntity()), this);
	else
		searchScreen->setParent(this);
	using namespace QueryTemplates;
	mainLayout->addWidget(searchScreen);
	mainLayout->addWidget(resultScreen);
	searchScreen->hide();
	resultScreen->hide();
	current = innerWidget;
	untouchable = innerWidget;
	main = this;
	submitButton->setDisabled(true);

#ifdef Q_OS_ANDROID
	barcodeField->installEventFilter(new filters::LineEditHelper(this));
#endif
#ifdef QT_VERSION5X
	QObject::connect(resultScreen, &IndependentBranchNode::backRequired, this, &NormalScaningWidget::hideCurrent);
	QObject::connect(searchScreen, &IndependentBranchNode::backRequired, this, &NormalScaningWidget::hideCurrent);
	QObject::connect(searchScreen, &IndependentBranchNode::done, this, &NormalScaningWidget::itemObtained);
	QObject::connect(resultScreen, &IndependentBranchNode::done, this, &NormalScaningWidget::saveSuccesfull);
	QObject::connect(BarcodeObs, &BarcodeObserver::barcodeCaught, this, &NormalScaningWidget::barcodeCaught);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &NormalScaningWidget::item_confirmed_response);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &NormalScaningWidget::item_scaned_response);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &NormalScaningWidget::document_confirmed_response);
#else
	QObject::connect(resultScreen, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(searchScreen, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
    QObject::connect(searchScreen, SIGNAL(done(RecEntity)), this, SLOT(itemObtained(RecEntity)));
    QObject::connect(resultScreen, SIGNAL(done(RecEntity)), this, SLOT(saveSuccesfull(RecEntity)));
    QObject::connect(BarcodeObs, SIGNAL(barcodeCaught(QString)), this, SLOT(barcodeCaught(QString)));
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(item_confirmed_response()));
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(item_scaned_response()));
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(document_confirmed_response()));
#endif
	BarcodeObs->activate();
}

void NormalScaningWidget::submitPressed()
{
	if (awaiter->isAwaiting() || barcodeField->text().isEmpty())
	{
		return;
	}
    showProcessingOverlay();
	if (!first_control.isNull()) first_control->blockSignals(true);
	if (!second_control.isNull()) second_control->blockSignals(true);
	userInfo->clear();
	QStringList buffer;
	buffer << barcodeField->text();
	buffer << ((first_control.isNull()) ? "" : first_control->getValue());
	buffer << ((second_control.isNull()) ? "" : second_control->getValue());
	buffer << "";
	buffer << "";
	AppNetwork->execQueryByTemplate(localCache[receiptAddItem], 5, buffer, awaiter);
    awaiter->deliverResultTo(receiptAddItem);
	wipe();
	if (!first_control.isNull()) first_control->blockSignals(false);
	if (!second_control.isNull()) second_control->blockSignals(false);

}

void NormalScaningWidget::barcodeConfirmed()
{
	if (awaiter->isAwaiting() || barcodeField->text().isEmpty())
		return;
    showProcessingOverlay();
	userInfo->clear();
	AppNetwork->execQueryByTemplate(localCache[getItemInfo], barcodeField->text(), "", awaiter);
    awaiter->deliverResultTo(getItemInfo);
}

void NormalScaningWidget::barcodeCaught(QString bc)
{
	barcodeField->setText(bc);
	barcodeConfirmed();
}

void NormalScaningWidget::docs_registration_complete()
{
	if (!awaiter->deliverHere(addIdToParentDocs)) return;
	hideProcessingOverlay();
	extraParentDocuments.clear();

}

void NormalScaningWidget::registerExtraParents(QString docid)
{
	if (awaiter->isAwaiting() || extraParentDocuments.isNull())
		return;
	if (!extraParentDocuments->list.isEmpty()) 
	{
		showProcessingOverlay();
		AppNetwork->execQueryByTemplate(localCache[addIdToParentDocs], docid, extraParentDocuments->joinIds(), awaiter);
		awaiter->deliverResultTo(addIdToParentDocs);
	}
}


void NormalScaningWidget::searchRequired()
{
	_hideAny(searchScreen);
}

void NormalScaningWidget::backNeeded()
{
	if (current == untouchable)
	{
		resultScreen->loadData();
		_hideAny(resultScreen);
	}
}

void NormalScaningWidget::useControls(const IControlList & cvals)
{
	AbstractScaningWidget::useControls(cvals);
	switch (controlsAvailable)
	{
	case 2:
#ifdef QT_VERSION5X
		QObject::connect(second_control.data(), &abs_control::editingFinished, this, &NormalScaningWidget::submitPressed);
		QObject::connect(first_control.data(), &abs_control::editingFinished,
			second_control.data(), &abs_control::setFocus);
#else
        QObject::connect(second_control.data(), SIGNAL(editingFinished()), this, SLOT(submitPressed()));
        QObject::connect(first_control.data(), SIGNAL(editingFinished()), second_control.data(), SLOT(setFocus()));
#endif
		first_control->setFocus();
		break;
	case 1:
#ifdef QT_VERSION5X
        QObject::connect(first_control.data(), &abs_control::editingFinished, this, &NormalScaningWidget::submitPressed);
#else
        QObject::connect(first_control.data(), SIGNAL(editingFinished()), this, SLOT(submitPressed()));
#endif
		first_control->setFocus();
		break;
	default:
		break;
	}
}

void NormalScaningWidget::item_scaned_response()
{
    if (!awaiter->deliverHere(getItemInfo))return;
	ResponseParser  parser(new LinearListWithSublistParser(awaiter->restext, awaiter->errtext));
	NetRequestResponse<InputControlEntity> response = 
		RequestParser::parseResponse<InputControlEntity>(parser);
	if (!assertAndShowError(this, parser, tr("Error!"), response.additionalObjects.isEmpty()))
	{
		mainTextView->setText(response.additionalObjects.first()->value("richdata"));
		itemSuppliedValues.clear();
		itemSuppliedValues = response.additionalObjects.first()->directFieldsAccess();
	}
    useControls(response.objects);
	hideProcessingOverlay();
}

void NormalScaningWidget::item_confirmed_response()
{
    if (!awaiter->deliverHere(receiptAddItem)) return;
	ResponseParser  parser(new LinearListWithSublistParser(awaiter->restext, awaiter->errtext));
	NetRequestResponse<InputControlEntity> response =
		RequestParser::parseResponse<InputControlEntity>(parser);
	if (!assertAndShowError(this, parser, tr("Error!"), response.additionalObjects.isEmpty()))
	{
		mainTextView->setText(response.additionalObjects.first()->value("richdata"));
		itemSuppliedValues.clear();
		itemSuppliedValues = response.additionalObjects.first()->directFieldsAccess();
	}
    useControls(response.objects);
	hideProcessingOverlay();
	if (!barcodeField->text().isEmpty())
	{
		awaiter->stopAwaiting();
		barcodeConfirmed();
	}
}

void NormalScaningWidget::document_confirmed_response()
{
    if(!awaiter->deliverHere(receiptNewDocument)) return;
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	NetRequestResponse<FullDocumentEntity> response =
		RequestParser::parseResponse<FullDocumentEntity>(parser);
	if (!assertAndShowError(this, parser, tr("Error!")))
	{
		if (response.isEmpty())
		{
			userInfo->setText(tr("document creation error"));
		}
		else
		{
            document = response.objects.first();
			userInfo->setText(modename + " (" + document->docId + ")\n" + document->supplier);
			mainTextView->setHtml(document->comment);
			registerExtraParents(document->docId);
		}
    }
    else
    {
        throwException(new BranchException(BranchException::ToModeSelection));
    }
	hideProcessingOverlay();
}

void NormalScaningWidget::hideCurrent()
{
	_hideCurrent(innerWidget);
}

void NormalScaningWidget::saveSuccesfull(RecEntity e)
{
    _hideCurrent(innerWidget);
	BarcodeObs->deactivate();
	emit done(e);
}

void NormalScaningWidget::itemObtained(RecEntity item)
{
	barcodeField->setText(item->getId());
	barcodeConfirmed();
	_hideCurrent(innerWidget);
}

void NormalScaningWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	switch (overloads.count())
	{
	default:
	case 4:
	{
		QStringList t;
		t << "current_doc" << "parent_docs";
		localCache.insert(addIdToParentDocs, overloads.at(3).assertedAndMappedCopy(
			addIdToParentDocs, t, t
		));
	}
	Q_FALLTHROUGH();
	case 3:
    {
        QStringList t;
        t<< "date" << "parent" << "comment" ;
		localCache.insert(receiptNewDocument, overloads.at(2).assertedAndMappedCopy(
			QueryTemplates::receiptNewDocument,
            t,t
		));
    }
        Q_FALLTHROUGH();
	case 2:
    {
        QStringList t;
        t << "barcode"<<"control1" << "control2" << "show_code" << "params" ;
        QStringList t2;
        t2 << "barcode" << "control1";
        localCache.insert(receiptAddItem, overloads.at(1).assertedAndMappedCopy(receiptAddItem,
            t,
            t2));
    }
        Q_FALLTHROUGH();
	case 1:
    {
        QStringList t;
        t << "barcode" << "printer";
        localCache.insert(getItemInfo, overloads.at(0).assertedAndMappedCopy(
			QueryTemplates::getItemInfo,
            t,t));
    }
        Q_FALLTHROUGH();
	case 0:
		break;
	}
	switch (4 - overloads.count())
	{
	case 4:
    {
        QStringList t;
        t << "barcode" << "printer";
        localCache.insert(getItemInfo, OverloadableQuery(
			QueryTemplates::getItemInfo,
            t,t));
    }
        Q_FALLTHROUGH();
	case 3:
    {
        QStringList t;
        t << "barcode"<<"control1" << "control2" << "show_code" << "params" ;
        QStringList t2;
        t2 << "barcode" << "control1";
		localCache.insert(receiptAddItem, OverloadableQuery(receiptAddItem,
            t, t2));
    }
        Q_FALLTHROUGH();
	case 2:
    {
        QStringList t;
        t<< "date" << "parent" << "comment" ;
		localCache.insert(receiptNewDocument, OverloadableQuery(
			QueryTemplates::receiptNewDocument,
            t,
            t));
    }
	case 1:
	{
		QStringList t;
		t << "current_doc" << "parent_docs";
		localCache.insert(addIdToParentDocs, OverloadableQuery(
			addIdToParentDocs,
			t, t
		));
	}
        Q_FALLTHROUGH();
	default:
		return;
	}
}

void NormalScaningWidget::_sendDataRequest()
{
}

void NormalScaningWidget::setDocument(Order o)
{
	if (awaiter->isAwaiting())
		return;
    showProcessingOverlay();
	AppNetwork->execQueryByTemplate(localCache[receiptNewDocument],
		QDate::currentDate().toString(DATE_ENCODING_FORMAT), o->code, "", awaiter);
    awaiter->deliverResultTo(receiptNewDocument);
}

void NormalScaningWidget::setDocument(RecEntity e)
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
	AppNetwork->execQueryByTemplate(localCache[receiptNewDocument],
		QDate::currentDate().toString(DATE_ENCODING_FORMAT), e->getId(), "", awaiter);
	awaiter->deliverResultTo(receiptNewDocument);
}

void NormalScaningWidget::setDocument(FullDocument doc)
{
	if (awaiter->isAwaiting())
		return;
    showProcessingOverlay();
    document = FullDocument(static_cast<FullDocumentEntity*>(doc->clone()));
	AppNetwork->execQueryByTemplate(localCache[receiptNewDocument],
		QDate::currentDate().toString(DATE_ENCODING_FORMAT), document->docId, document->comment, awaiter);
    awaiter->deliverResultTo(receiptNewDocument);
}


