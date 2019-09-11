#pragma once

#include "widgets/SuppliersSelectionBranch/SuppliersSelectWidget.h"
#include "widgets/ReceiptSpecialWidgets/ReceiptParametersWidget.h"




QHash<QString, QString> _initRecDefs();
const QHash<QString, QString> RECEIPT_NONE_SETTINGS_DEFAULTS(_initRecDefs());

class ReceiptRootWidget : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
	GlobalAppSettings& globalSettings;
	
	parsedSupplier confirmedSupplier;
	parsedOrder confirmedOrder;

	QVBoxLayout* mainLayout;
	   
	ReceiptParametersWidget* innerWidget;

	SuppliersSelectWidget* suppliersSelect;
	OrderSelectionWidget* orderSelect;

	QHash<QString, QString> options;

	void processOptions();

public:
	ReceiptRootWidget(GlobalAppSettings& go, QHash<QString, QString> settings = RECEIPT_NONE_SETTINGS_DEFAULTS,
		QWidget* parent = Q_NULLPTR);

private slots:;
	void supplierAcquired(parsedSupplier);
	void orderAcquired(parsedOrder, QString);
	void hideCurrent();
};


