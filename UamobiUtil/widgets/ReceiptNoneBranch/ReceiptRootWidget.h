#pragma once

#include "widgets/SuppliersSelectionBranch/SuppliersSelectWidget.h"
#include "widgets/ReceiptSpecialWidgets/ReceiptParametersWidget.h"
#include "widgets/ReceiptSpecialWidgets/ReceiptScaningWidget.h"

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
	ReceiptScaningWidget* scaning;

	QHash<QString, QString> options;
	parsedMode modeItself;

	void processOptions();
	void openCorrespondingSubbranch();

public:
	ReceiptRootWidget(GlobalAppSettings& go, QHash<QString, QString> settings = RECEIPT_NONE_SETTINGS_DEFAULTS,
		QString submode = "",
		QWidget* parent = Q_NULLPTR);

private slots:
	void supplierAcquired(parsedSupplier);
	void orderAcquired(parsedOrder, QString);
	void hideCurrent();
	void backTo();
	void continueToScaning();
};
