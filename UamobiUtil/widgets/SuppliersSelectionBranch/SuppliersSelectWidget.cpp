#include "SuppliersSelectWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
//#define DEBUG_FILTER
#ifdef DEBUG
#include "debugtrace.h"
#endif

QString specwidgets::_SupplierSelectionWidget::elemToString(int i)
{
	return suppliers.at(i).name;
}

int specwidgets::_SupplierSelectionWidget::countElems()
{
	return suppliers.count();
}

specwidgets::_SupplierSelectionWidget::_SupplierSelectionWidget(QVector<parsedSupplier>& supplist, QWidget* parent)
	: AbstractListSelectionWidget(parent), suppliers(supplist)
{
	init();
}

void specwidgets::_SupplierSelectionWidget::itemSelectedFromList(QListWidgetItem*)
{
	emit supplierPicked(suppliers.at(currentRow()));
}

SuppliersSelectWidget::SuppliersSelectWidget(GlobalAppSettings& go, QWidget* parent,
	QString item, interpretsPointers::interpretAsSupplierLike inter, bool isOrdNeeded)
	: AbstractSearchAndPickWidget(go, parent, item, &DataUpdateEngine::recListTemplated, isOrdNeeded ), allsuppliers(), interpreter(inter)
{
	specwidgets::_SupplierSelectionWidget * supplierSelection = new specwidgets::_SupplierSelectionWidget(allsuppliers, this);
#ifdef QT_VERSION5X
	QObject::connect(supplierSelection, &specwidgets::_SupplierSelectionWidget::supplierPicked, this, &SuppliersSelectWidget::supplierPicked);
#else
	QObject::connect(supplierSelection, SIGNAL(supplierPicked(parsedSupplier)), this, SLOT(supplierPicked(parsedSupplier)));
#endif
	emplaceSelection(supplierSelection);
}


bool SuppliersSelectWidget::isExpectingControl(int val)
{
	if (awaiter.isAwaiting())
		return false;
	if (val >= -1 && val <= allsuppliers.count() - 1)
	{
		if (val == -1)
		{
			if (allsuppliers.count() > 10)
				val = 9;
			else
			{
				emit backRequired();
				return false;
			}
		}
		supplierPicked(allsuppliers.at(val));
		return true;
	}

	return false;
}

void SuppliersSelectWidget::supplierPicked(parsedSupplier supp)
{
	if (awaiter.isAwaiting())
		return;
	hideProcessingOverlay();
	emit supplierAcquired(supp);
}

void SuppliersSelectWidget::interpretResult()
{
	allsuppliers = interpreter(awaiter.restext, awaiter.errtext);
}



SuppliersSelectionBranch::SuppliersSelectionBranch(GlobalAppSettings& go, QWidget* parent, QString item,
	interpretsPointers::interpretAsSupplierLike inter)
	: SuppliersSelectWidget(go, parent, item, inter), abstractNode(),
	orderSelection(new OrderSelectionWidget(go, confirmedSupplier, this))
{
	mainLayout->addWidget(orderSelection);
	orderSelection->hide();

	current = innerWidget;
	untouchable = innerWidget;
	main = this;
#ifdef QT_VERSION5X
	QObject::connect(orderSelection, &OrderSelectionWidget::orderConfirmed, this, &SuppliersSelectionBranch::orderAcquired);
	QObject::connect(orderSelection, &OrderSelectionWidget::backRequired, this, &SuppliersSelectionBranch::hideCurrent);
#else
	QObject::connect(orderSelection, SIGNAL(orderConfirmed(parsedOrder, QString)), this, SLOT(orderAcquired(parsedOrder)));
	QObject::connect(orderSelection, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
#endif
}

void SuppliersSelectionBranch::show()
{
	inframedWidget::show();
	itemSelection->setFocus();
}

void SuppliersSelectionBranch::hideCurrent()
{
	if (current == innerWidget)
	{
		emit backRequired();
	}
	else
	{
		_hideCurrent(innerWidget);
	}
}

void SuppliersSelectionBranch::supplierPicked(parsedSupplier ps)
{
#ifdef DEBUG
	//detrace_METHCALL("supplier picked");
#endif
	SuppliersSelectWidget::supplierPicked(ps);
	confirmedSupplier = ps;
	_hideAny(orderSelection);
	orderSelection->loadOrders();
}

void SuppliersSelectionBranch::orderAcquired(parsedOrder po)
{
}