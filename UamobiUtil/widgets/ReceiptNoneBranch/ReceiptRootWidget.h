#pragma once

#include "widgets/SuppliersSelectionBranch/SuppliersSelectWidget.h"
#include "widgets/ReceiptSpecialWidgets/ReceiptParametersWidget.h"
#include "widgets/ReceiptSpecialWidgets/ReceiptScaningWidget.h"

QHash<QString, QString> _initRecDefs();
const QHash<QString, QString> RECEIPT_NONE_SETTINGS_DEFAULTS(_initRecDefs());

/*
	This widget is used as "root" for every other receipt step. It has nothing to display and used mostly for
	single-widget like interface. It's contents are another widgets. Inner widget (default) is ReceiptParameters

	This root supports 3 modes now: 
				receipt - None 
				receipt - Warehouses.
				receipt - ordersAndSldCorrection
*/

class ReceiptRootWidget : public inframedWidget, abstractNode
{
	Q_OBJECT

public:
	enum steps { toSupplier, toOrders, toScaning };	//	this enum represents steps in "network request chain"
protected:
	GlobalAppSettings& globalSettings;		//	uses GAS

	parsedSupplier confirmedSupplier;		//	stores pair of order\supplier for network requests
	parsedOrder confirmedOrder;

	QVBoxLayout* mainLayout;

	ReceiptParametersWidget* innerWidget;

	SuppliersSelectWidget* suppliersSelect;
	OrderSelectionWidget* orderSelect;
	ReceiptScaningWidget* scaning;

	QHash<QString, QString> options;	//	options of this mode. UNUSED
	parsedMode modeItself;				//	mode object which is used for netrequests

	void processOptions();				//	uses options to modify this widget
	void openCorrespondingSubbranch();	//	opens subbranch depending on settings.UNUSED

public:
	ReceiptRootWidget(GlobalAppSettings& go, QHash<QString, QString> settings = RECEIPT_NONE_SETTINGS_DEFAULTS,
		QString submode = "",
		QWidget* parent = Q_NULLPTR);

private slots:
	void supplierAcquired(parsedSupplier);	//	triggered when supplier is selected
	void orderAcquired(parsedOrder, QString);	//	when order selected
	void hideCurrent();			//	hides current widget to show inner
	void backTo();				//	returns to previous step
	void backToStep(int step);	//	returns to specified step
	void continueToScaning();	//	is triggered when all data is gathered and ScaningWidget can be created
};
