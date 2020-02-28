#include "ObservedScaningWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "widgets/MultibranchWidgets/Observers/SkippedNode.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif


void ObservedScaningWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	switch (
		((overloads.count() > 4) ? 4 : overloads.count())
		)
	{
	case 4:
	{
		QStringList t;
		t << "barcode" << "obtained";
		localCache.insert(QueryTemplates::setVersionForBarcode,
			overloads.at(3).assertedAndMappedCopy(
				QueryTemplates::setVersionForBarcode, t, t
			));
	}
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
		localCache.insert(receiptAddItemExpDate, overloads.at(1).assertedAndMappedCopy(
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
	switch (4 - overloads.count())
	{
	case 4:
	{
		QStringList t;
		t << "barcode" << "obtained";
		localCache.insert(QueryTemplates::setVersionForBarcode,
			OverloadableQuery(QueryTemplates::setVersionForBarcode, t, t
			));
	}
	case 3:
	{
		QStringList t;
		t << "barcode"  << "printer";
		QStringList t2;
		t2 << "barcode" << "printer";
		localCache.insert(getItemInfo, OverloadableQuery(
			QueryTemplates::getItemInfo,
			t, t2));
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
void ObservedScaningWidget::barcodeConfirmed()
{
	observerNode->processRecord(RecEntity(new BarcodeEntity(barcodeField->text(), QString())));
	_hideAny(observerNode);
	userInfo->clear();
}


void ObservedScaningWidget::item_confirmed_response()
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
		if (!itemFromObserver.isNull())
		{
			AppNetwork->execQueryByTemplate(localCache[setVersionForBarcode],
					barcodeField->text(), itemFromObserver->getId(), Q_NULLPTR);
		}
	}
	useControls(response.objects);
	hideProcessingOverlay();
	if (!barcodeField->text().isEmpty())
	{
		awaiter->stopAwaiting();
		barcodeConfirmed();
	}
}


ObservedScaningWidget::ObservedScaningWidget(QWidget* parent,
 	IndependentBranchNode* observer, 
	IndependentBranchNode* resScreen, 
	IndependentBranchNode* searchScr)
	: MulticontrolScaningWidget(parent, resScreen, searchScr),
	observerNode(observer)
{
#ifdef DEBUG
	detrace_DCONSTR("ObservedScaning");
#endif

	if (observerNode == Q_NULLPTR)
	{
		observerNode = new SkippedNode(this);
		observerNode->hide();
	}
	else
	{
		observerNode->setParent(this);
		mainLayout->addWidget(observerNode);
		observerNode->hide();
	}
#ifdef QT_VERSION5X
	QObject::connect(observerNode, &IndependentBranchNode::done, this, &ObservedScaningWidget::observerDone);
	QObject::connect(observerNode, &IndependentBranchNode::backRequired, this, &ObservedScaningWidget::hideCurrent);
#else
	QObject::connect(observerNode, SIGNAL(done(RecEntity)), this, SLOT(observerDone(RecEntity)));
	QObject::connect(observerNode, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
#endif
}

void ObservedScaningWidget::observerDone(RecEntity e)
{
	if (awaiter->isAwaiting())
		return;
	if (e.isNull())
		return;
	if (!(e->getId() == barcodeField->text()))
		itemFromObserver = RecEntity(e->clone());
	else
		itemFromObserver.clear();
	_hideAny(innerWidget);
	MulticontrolScaningWidget::barcodeConfirmed();
}
