#include "ObservedScaningWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "widgets/MultibranchWidgets/Observers/SkippedNode.h"


void ObservedScaningWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
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
		localCache.insert(receiptAddItemExpDate, overloads.at(1).assertedAndMappedCopy(
			receiptAddItemExpDate,
			t, t2));
	}
	case 1:
	{
		QStringList t;
		t << "barcode" << "code" << "printer";
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
		t << "barcode"  << "code" << "printer";
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
	_hideAny(static_cast<inframedWidget*>(observerNode));
}


ObservedScaningWidget::ObservedScaningWidget(QWidget* parent,
 	IndependentBranchNode* observer, 
	IndependentBranchNode* resScreen, 
	IndependentBranchNode* searchScr)
	: MulticontrolScaningWidget(parent, resScreen, searchScr),
	observerNode(observer)
{
	if (observerNode == Q_NULLPTR)
	{
		observerNode = new SkippedNode(this);
		observerNode->hide();
	}
	else
	{
		observerNode->setParent(this);
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
	showProcessingOverlay();
	if (e.isNull())
		AppNetwork->execQueryByTemplate(localCache[getItemInfo], barcodeField->text(), "", awaiter);
	else
		AppNetwork->execQueryByTemplate(localCache[getItemInfo], barcodeField->text(), e->getId(), "", awaiter);
	awaiter->deliverResultTo(getItemInfo);
}
