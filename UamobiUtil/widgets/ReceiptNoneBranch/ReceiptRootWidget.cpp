#include "ReceiptRootWidget.h"

QHash<QString, QString> _initRecDefs()
{
	return QHash<QString, QString>();
}

void ReceiptRootWidget::processOptions()
{
	if (modeItself.submode == "warehouses")
	{
		suppliersSelect = new SuppliersSelectWidget(globalSettings, this, &DataUpdateEngine::getWarehousesList);
	}
	else
	{
		suppliersSelect = new SuppliersSelectWidget(globalSettings, this);
	}
	orderSelect = new OrderSelectionWidget(globalSettings, confirmedSupplier, this);

	
}

void ReceiptRootWidget::openCorrespondingSubbranch()
{
	_hideAny(suppliersSelect);
}

ReceiptRootWidget::ReceiptRootWidget(GlobalAppSettings& go, QHash<QString, QString> settings, QString submode, QWidget* parent)
	: inframedWidget(parent), abstractNode(), globalSettings(go),
	confirmedSupplier(), confirmedOrder(), mainLayout(new QVBoxLayout(this)),
	innerWidget(new ReceiptParametersWidget(go, parent)),
	suppliersSelect(),
	orderSelect(),
	scaning(new ReceiptScaningWidget(go, this)),
	options(settings), modeItself("receipt","receipt", submode)
{
	this->setLayout(mainLayout);
	processOptions();
	mainLayout->addWidget(innerWidget);
	mainLayout->addWidget(suppliersSelect);
	mainLayout->addWidget(orderSelect);
	mainLayout->addWidget(scaning);
	suppliersSelect->hide();
	orderSelect->hide();
	scaning->hide();

	current = innerWidget;
	untouchable = innerWidget;
	openCorrespondingSubbranch();
#ifdef QT_VERSION5X
	QObject::connect(suppliersSelect, &SuppliersSelectWidget::supplierAcquired, this, &ReceiptRootWidget::supplierAcquired);
	QObject::connect(suppliersSelect, &SuppliersSelectWidget::backRequired, this, &ReceiptRootWidget::backTo);
	QObject::connect(orderSelect, &OrderSelectionWidget::orderConfirmed, this, &ReceiptRootWidget::orderAcquired);
	QObject::connect(innerWidget, &ReceiptParametersWidget::backRequired, this, &ReceiptRootWidget::backTo);
	QObject::connect(innerWidget, &ReceiptParametersWidget::dataConfirmed, this, &ReceiptRootWidget::continueToScaning);
	QObject::connect(scaning, &ReceiptScaningWidget::backRequired, this, &ReceiptRootWidget::hideCurrent);
#else
	QObject::connect(suppliersSelect, SIGNAL(supplierAcquired(parsedSupplier)), this, SLOT(supplierAcquired(parsedSupplier)));
	QObject::connect(orderSelect, SIGNAL(orderConfirmed(parsedOrder, QString)), this, SLOT(orderAcquired(parsedOrder, QString)));
	QObject::connect(innerWidget, SIGNAL(backRequired()), this, SLOT(backTo()));
	QObject::connect(innerWidget, SIGNAL(dataConfirmed()), this, SLOT(continueToScaning()));
	QObject::connect(scaning, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
#endif
}

void ReceiptRootWidget::supplierAcquired(parsedSupplier supp)
{
	confirmedSupplier = supp;
	orderSelect->loadOrders();
	_hideAny(orderSelect);
}

void ReceiptRootWidget::orderAcquired(parsedOrder po, QString  richtext)
{
#ifdef DEBUG
	//detrace_SLOTCALL("orderAcquired", "ReceiptRootWidget");
#endif
	innerWidget->setMainView(richtext);
	confirmedOrder = po;
	_hideCurrent(innerWidget);
}

void ReceiptRootWidget::hideCurrent()
{
	_hideCurrent(innerWidget);
}

void ReceiptRootWidget::backTo()
{
	if (current == suppliersSelect)
	{
		emit backRequired();
		return;
	}
	_hideAny(orderSelect);
}

void ReceiptRootWidget::continueToScaning()
{
	scaning->setDocument(confirmedOrder);
	_hideAny(scaning);
}