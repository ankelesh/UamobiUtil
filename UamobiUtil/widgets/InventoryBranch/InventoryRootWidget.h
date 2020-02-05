#pragma once
#include "widgets/parents/inframedWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "networking/things.h"
#include "widgets/InventoryBranch/InventoryParamsWidget.h"
#include "widgets/InventorySpecialWidgets/ParentDocumentWidget.h"
#include "widgets/MultibranchWidgets/BarcodeFilterSelectionSubbranch.h"

/*
	This widget is used as core one for inventory branch. It has nothing to display
	and works like organized workspace. 

	Supported modes:
		inventory - none
		inventory - partialinventory

	Update:
	now this widget is calling barcodeFilterSelection only when document was initiated within scanned widget.
	This led to changes in goToSelection.

*/



class InventoryRootWidget : public inframedWidget, abstractNode
{
	Q_OBJECT
public:
	enum steps { parentSelection,filterSelection, Scaning };
protected:
	QVBoxLayout* mainLayout;
	InventoryParamsWidget* innerWidget;
	ParentDocumentWidget* docSelectionWidget;
	InventoryScaningWidget* scaningWidget;
	BarcodeFilterSelectionSubbranch* bfilterWidget;

	QHash<QString, QString> options;	//	options of this mode. UNUSED
	parsedMode modeItself;				//	mode object which is used for netrequests

	void processOptions();				//	uses options to modify this widget
	void openCorrespondingSubbranch();	//	opens subbranch depending on settings.UNUSED
	 
public:
	InventoryRootWidget(QHash<QString, QString> settings,
		parsedMode mode = parsedMode("inventory", "inventory",""),
		QWidget* parent = Q_NULLPTR);

private slots:
	void documentAcquired(parsedDocument);	//	triggered when supplier is selected
	void hideCurrent();			//	hides current widget to show inner
	void backTo();				//	returns to previous step
	void backToStep(int step);	//	returns to specified step
	void continueToScaning();	//	is triggered when all data is gathered and ScaningWidget can be created
	void prescaningRequest();	//	is used only in case when barcodeFilterSelection exists
};
