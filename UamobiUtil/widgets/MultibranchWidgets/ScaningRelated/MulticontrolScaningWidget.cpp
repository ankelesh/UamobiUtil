#include "MulticontrolScaningWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "ScaningCore/BarcodeObserver.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "widgets/MultibranchWidgets/DocResultsWidget.h"
#include "widgets/MultibranchWidgets/PagedSearchWidget.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif

void MulticontrolScaningWidget::_handleRecord(RecEntity e)
{
	if (e.isNull())
	{
#ifdef DEBUG
		detrace_METHPERROR("_handleRecord", "No entity provided");
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
	else if (e->myType() == UniformXmlObject::LesserDocument)
	{
		setDocument(FullDocument(new FullDocumentEntity(e->getId(), 
			QString(), QDate::currentDate().toString(DATE_ENCODING_FORMAT))));
	}
}

void MulticontrolScaningWidget::_sendDataRequest()
{
}

void MulticontrolScaningWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
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
		t << "barcode" << "control1" << "control2" << "show_code" << "params" << "joined" << "mapped";
		QStringList t2;
		t2 << "barcode" << "joined";
		localCache.insert(receiptAddItemExpDate,overloads.at(1).assertedAndMappedCopy(
			receiptAddItemExpDate,
			t, t2));
	}
	case 1:
	{
		QStringList t;
		t << "barcode" << "printer";
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
		t << "barcode" << "printer";
		localCache.insert(getItemInfo, OverloadableQuery(
			QueryTemplates::getItemInfo,
			t, t));
	}
	case 2:
	{
		QStringList t;
		t << "barcode" << "control1" << "control2" << "show_code" << "params" << "joined" << "mapped";
		QStringList t2;
		t2 << "barcode" << "joined";
		localCache.insert(receiptAddItemExpDate, OverloadableQuery(receiptAddItemExpDate,
			t, t2));
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

MulticontrolScaningWidget::MulticontrolScaningWidget(QWidget* parent, 
	IndependentBranchNode* resScreen, IndependentBranchNode* searchScr)
	: AbstractScaningWidget(independent_nodes::MulticontrolScaning, parent),
	abstractNode(),
	resultScreen(resScreen), searchScreen(searchScr), toControls(new MegaIconButton(this)),
	controlList(new ControlListWidget(this)), localCache()
{

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
	mainLayout->addWidget(controlList);
	searchScreen->hide();
	resultScreen->hide();
	current = innerWidget;
	untouchable = innerWidget;
	main = this;
	submitButton->setDisabled(true);
	innerLayout->insertWidget(innerLayout->count() - 1, toControls);
	toControls->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);
	toControls->setText(tr("Enter data"));
	controlList->hide();
	toControls->setDisabled(true);

#ifdef QT_VERSION5X
	QObject::connect(resultScreen, &IndependentBranchNode::backRequired, this, &MulticontrolScaningWidget::hideCurrent);
	QObject::connect(searchScreen, &IndependentBranchNode::backRequired, this, &MulticontrolScaningWidget::hideCurrent);
	QObject::connect(searchScreen, &IndependentBranchNode::done, this, &MulticontrolScaningWidget::itemObtained);
	QObject::connect(resultScreen, &IndependentBranchNode::done, this, &MulticontrolScaningWidget::saveSuccesfull);
	QObject::connect(BarcodeObs, &BarcodeObserver::barcodeCaught, this, &MulticontrolScaningWidget::barcodeCaught);
	QObject::connect(toControls, &MegaIconButton::clicked, this, &MulticontrolScaningWidget::openControlList);
	QObject::connect(controlList, &ControlListWidget::backRequired, this, &MulticontrolScaningWidget::hideCurrent);
	QObject::connect(controlList, &ControlListWidget::controlsConfirmed, this, &MulticontrolScaningWidget::submitPressed);
	QObject::connect(controlList, &ControlListWidget::controlsConfirmed, this, &MulticontrolScaningWidget::hideCurrent);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &MulticontrolScaningWidget::document_confirmed_response);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &MulticontrolScaningWidget::item_confirmed_response);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &MulticontrolScaningWidget::item_scaned_response);
#else
	QObject::connect(resultScreen, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(searchScreen, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(searchScreen, SIGNAL(done(RecEntity)), this, SLOT(itemObtained(RecEntity)));
	QObject::connect(resultScreen, SIGNAL(done(RecEntity)), this, SLOT(saveSuccesfull(RecEntity)));
	QObject::connect(BarcodeObs, SIGNAL(barcodeCaught(QString)), this, SLOT(barcodeCaught(QString)));
	QObject::connect(toControls, SIGNAL(clicked()), this, SLOT(openControlList()));
	QObject::connect(controlList, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(controlList, SIGNAL(controlsConfirmed()), this, SLOT(submitPressed()));
	QObject::connect(controlList, SIGNAL(controlsConfirmed()), this, SLOT(hideCurrent()));
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(document_confirmed_response()));
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(item_confirmed_response()));
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(item_scaned_response()));
#endif
	BarcodeObs->activate();
}

void MulticontrolScaningWidget::setDocument(FullDocument doc)
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &MulticontrolScaningWidget::document_confirmed_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(document_confirmed_response()));
#endif
	document = FullDocument(static_cast<FullDocumentEntity*>(doc->clone()));
	AppNetwork->execQueryByTemplate(localCache[receiptNewDocument],
		QDate::currentDate().toString(DATE_ENCODING_FORMAT), document->docId, document->comment, awaiter);
	awaiter->deliverResultTo(receiptNewDocument);
}

void MulticontrolScaningWidget::setDocument(Order o)
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
	AppNetwork->execQueryByTemplate(localCache[receiptNewDocument],
		QDate::currentDate().toString(DATE_ENCODING_FORMAT), o->code, "", awaiter);
	awaiter->deliverResultTo(receiptNewDocument);
}

void MulticontrolScaningWidget::useControls(IControlList& clist)
{
	if (clist.isEmpty())
	{
		controlList->clearControls();
		toControls->setDisabled(true);
	}
	else
	{
		controlList->useControls(clist);
		toControls->setDisabled(false);
		toControls->setFocus();
	}
}

void MulticontrolScaningWidget::submitPressed()
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
	buffer << ((second_control.isNull()) ? "" : second_control->getValue());
	buffer << "";
	buffer << "";
	buffer << controlList->joinedControls();
	buffer << controlList->mappedControls();
	AppNetwork->execQueryByTemplate(localCache[receiptAddItemExpDate], 7, buffer, awaiter);
	awaiter->deliverResultTo(receiptAddItemExpDate);
	wipe();
}

void MulticontrolScaningWidget::searchRequired()
{
	_hideAny(searchScreen);
}

void MulticontrolScaningWidget::backNeeded()
{
	resultScreen->loadData();
	_hideAny(resultScreen);
}

void MulticontrolScaningWidget::barcodeConfirmed()
{
	if (awaiter->isAwaiting())
		return;
	userInfo->clear();
	showProcessingOverlay();
	AppNetwork->execQueryByTemplate(localCache[getItemInfo], barcodeField->text(), "", awaiter);
	awaiter->deliverResultTo(getItemInfo);
}

void MulticontrolScaningWidget::item_scaned_response()
{
	if (!awaiter->deliverHere(getItemInfo))
		return;
	ResponseParser  parser(new LinearListWithSublistParser(awaiter->restext, awaiter->errtext));
	NetRequestResponse<InputControlEntity> response =
		RequestParser::parseResponse<InputControlEntity>(parser);
	if (response.isError || response.additionalObjects.isEmpty())
	{
		userInfo->setText(response.errtext);
#ifdef DEBUG
		detrace_NRESPERR(response.errtext);
#endif
	}
	else
	{
		mainTextView->setText(response.additionalObjects.first()->value("richdata"));
		itemSuppliedValues.clear();
		itemSuppliedValues = response.additionalObjects.first()->directFieldsAccess();
	}
	useControls(response.objects);
	hideProcessingOverlay();
}

void MulticontrolScaningWidget::item_confirmed_response()
{
	if (!awaiter->deliverHere(receiptAddItemExpDate))
		return;
	ResponseParser  parser(new LinearListWithSublistParser(awaiter->restext, awaiter->errtext));
	NetRequestResponse<InputControlEntity> response =
		RequestParser::parseResponse<InputControlEntity>(parser);
	if (response.isError || response.additionalObjects.isEmpty())
	{
		userInfo->setText(response.errtext);
#ifdef DEBUG
		detrace_NRESPERR(response.errtext);
#endif
	}
	else
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

void MulticontrolScaningWidget::document_confirmed_response()
{
	if (!awaiter->deliverHere(receiptNewDocument))
		return;
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	NetRequestResponse<FullDocumentEntity> response =
		RequestParser::parseResponse<FullDocumentEntity>(parser);
	if (response.isError)
	{
		userInfo->setText(response.errtext);
	}
	else
	{
		if (response.isEmpty())
		{
			userInfo->setText(tr("document creation error"));
#ifdef DEBUG
			detrace_NRESPERR("document creation error");
#endif
		}
		else
		{
			document = response.objects.first();
			userInfo->setText(modename + " (" + document->docId + ")\n" + document->supplier);
			mainTextView->setHtml(document->comment);
		}
	}
	hideProcessingOverlay();
}

void MulticontrolScaningWidget::hideCurrent()
{

	_hideCurrent(innerWidget);
}

void MulticontrolScaningWidget::saveSuccesfull(RecEntity e)
{
	_hideCurrent(innerWidget);
	BarcodeObs->deactivate();
	emit done(e);
}

void MulticontrolScaningWidget::itemObtained(RecEntity item)
{
	barcodeField->setText(item->getId());
	barcodeConfirmed();
	_hideCurrent(innerWidget);
}

void MulticontrolScaningWidget::barcodeCaught(QString bc)
{
	barcodeField->setText(bc);
	barcodeConfirmed();
}

void MulticontrolScaningWidget::openControlList()
{
	_hideAny(controlList);
}

