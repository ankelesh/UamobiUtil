#include "ReceiptRootWidget.h"

QHash<QString, QString> _initRecDefs()
{
	return QHash<QString, QString>();
}

void ReceiptRootWidget::processOptions()
{
	_hideAny(suppliersSelect);
}

ReceiptRootWidget::ReceiptRootWidget(GlobalAppSettings& go, QHash<QString, QString> settings, QWidget* parent)
	: inframedWidget(parent), abstractNode(), globalSettings(go),
	confirmedSupplier(), confirmedOrder(), mainLayout(new QVBoxLayout(this)),
	innerWidget(new ReceiptParametersWidget( go, parent)), 
	suppliersSelect(new SuppliersSelectWidget(go, this)),
	orderSelect(new OrderSelectionWidget(go, confirmedSupplier, this)),
	options(settings)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	mainLayout->addWidget(suppliersSelect);
	mainLayout->addWidget(orderSelect);
	suppliersSelect->hide();
	orderSelect->hide();

	current = innerWidget;
	untouchable = innerWidget;

	processOptions();

	QObject::connect(suppliersSelect, &SuppliersSelectWidget::supplierAcquired, this, &ReceiptRootWidget::supplierAcquired);
	QObject::connect(orderSelect, &OrderSelectionWidget::orderConfirmed, this, &ReceiptRootWidget::orderAcquired);

}

void ReceiptRootWidget::supplierAcquired(parsedSupplier supp)
{
	confirmedSupplier = supp;
	orderSelect->loadOrders();
	_hideAny(orderSelect);
}

void ReceiptRootWidget::orderAcquired(parsedOrder po, QString  richtext)
{
	detrace_SLOTCALL("orderAcquired", "ReceiptRootWidget")
	innerWidget->setMainView(richtext);
	confirmedOrder = po;
	_hideCurrent(innerWidget);
}

void ReceiptRootWidget::hideCurrent()
{
}
