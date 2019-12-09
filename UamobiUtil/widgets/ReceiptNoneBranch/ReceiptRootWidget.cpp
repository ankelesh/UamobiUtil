#include "ReceiptRootWidget.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

QHash<QString, QString> _initRecDefs()
{
	return QHash<QString, QString>();
}

void ReceiptRootWidget::processOptions()
{
	if (modeItself.submode.contains("warehouse", Qt::CaseInsensitive))
	{
		suppliersSelect = new SuppliersSelectionBranch(globalSettings, this, "warehouses");
	}
	else
	{
		suppliersSelect = new SuppliersSelectionBranch(globalSettings, this, "suppliers");
	}
	suppliersSelect->loadItems();
}

void ReceiptRootWidget::openCorrespondingSubbranch()
{
	_hideAny(suppliersSelect);
}

ReceiptRootWidget::ReceiptRootWidget(GlobalAppSettings& go, QHash<QString, QString> settings, parsedMode mode, QWidget* parent)
	: inframedWidget(parent), abstractNode(), globalSettings(go),
	confirmedSupplier(), confirmedOrder(), mainLayout(new QVBoxLayout(this)),
	innerWidget(new ReceiptParametersWidget(go, parent)),
	suppliersSelect(),
	scaning(new ReceiptScaningWidget(go, this)),
	options(settings), modeItself(mode)
{
	this->setLayout(mainLayout);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	processOptions();
	mainLayout->addWidget(innerWidget);
	mainLayout->addWidget(suppliersSelect);
	mainLayout->addWidget(scaning);
	suppliersSelect->hide();
	scaning->hide();
	
	current = innerWidget;
	untouchable = innerWidget;
	main = innerWidget;
	openCorrespondingSubbranch();
	removeEventFilter(keyfilter);
#ifdef QT_VERSION5X
	QObject::connect(suppliersSelect, &SuppliersSelectionBranch::orderReady, this, &ReceiptRootWidget::orderAcquired);
	QObject::connect(suppliersSelect, &SuppliersSelectionBranch::backRequired, this, &ReceiptRootWidget::backTo);
	QObject::connect(innerWidget, &ReceiptParametersWidget::backRequired, this, &ReceiptRootWidget::backTo);
	QObject::connect(innerWidget, &ReceiptParametersWidget::dataConfirmed, this, &ReceiptRootWidget::continueToScaning);
	QObject::connect(innerWidget, &ReceiptParametersWidget::backTo, this, &ReceiptRootWidget::backToStep);
	QObject::connect(scaning, &ReceiptScaningWidget::backRequired, this, &ReceiptRootWidget::hideCurrent);
#else
    QObject::connect(suppliersSelect, SIGNAL(orderReady(parsedOrder,QString)), this, SLOT(orderAcquired(parsedOrder,QString)));
    QObject::connect(suppliersSelect, SIGNAL(backRequired()), this, SLOT(backTo()));
	QObject::connect(innerWidget, SIGNAL(backRequired()), this, SLOT(backTo()));
	QObject::connect(innerWidget, SIGNAL(dataConfirmed()), this, SLOT(continueToScaning()));
	QObject::connect(innerWidget, SIGNAL(backTo(int)), this, SLOT(backToStep(int)));
	QObject::connect(scaning, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
#endif
}

void ReceiptRootWidget::supplierAcquired(parsedSupplier supp)
{
#ifdef DEBUG
	detrace_SLOTCALL("supplierAcquired", "receiptRootWidget");
#endif
	confirmedSupplier = supp;
}

void ReceiptRootWidget::orderAcquired(parsedOrder po, QString  richtext)
{
#ifdef DEBUG
    detrace_SLOTCALL("orderAcquired", "ReceiptRootWidget");
#endif
	innerWidget->setMainView(richtext);
	confirmedOrder = po;
	_hideCurrent(innerWidget);
	innerWidget->setFocus();
}

void ReceiptRootWidget::hideCurrent()
{
	_hideCurrent(innerWidget);
	innerWidget->setFocus();
	scaning->clear();
}

void ReceiptRootWidget::backTo()
{
#ifdef DEBUG
	detrace_METHCALL("backTo");
#endif
	if (current == suppliersSelect)
	{
		emit backRequired();
		return;
	}
    else if (current == untouchable)
    {
       _hideAny(suppliersSelect);
    }
}

void ReceiptRootWidget::backToStep(int step)
{
	switch (step)
	{
	case toSupplier:
		_hideAny(suppliersSelect);
		return;
	default:
		return;
	}
}

void ReceiptRootWidget::continueToScaning()
{
	scaning->setDocument(confirmedOrder);
	scaning->setModeName(modeItself.name);
	_hideAny(scaning);
}
