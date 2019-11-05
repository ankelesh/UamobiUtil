#pragma once
#include "widgets/parents/inframedWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "networking/things.h"
#include "widgets/InventoryBranch/InventoryParamsWidget.h"
#include "widgets/InventorySpecialWidgets/InventoryScaningWidget.h"
#include "widgets/InventorySpecialWidgets/ParentDocumentWidget.h"

class InventoryRootWidget : public inframedWidget, abstractNode
{
	Q_OBJECT
public:
	enum steps {parentSelection, Scaning};
protected:
	GlobalAppSettings& globalSettings;
	QVBoxLayout* mainLayout;
	InventoryParamsWidget* innerWidget;
	ParentDocumentWidget* docSelectionWidget;
	InventoryScaningWidget* scaningWidget;

	QHash<QString, QString> options;	//	options of this mode. UNUSED
	parsedMode modeItself;				//	mode object which is used for netrequests

	void processOptions();				//	uses options to modify this widget
	void openCorrespondingSubbranch();	//	opens subbranch depending on settings.UNUSED

public:
	InventoryRootWidget(GlobalAppSettings& go, QHash<QString, QString> settings,
		QString submode = "",
		QWidget* parent = Q_NULLPTR);

private slots:
	void documentAcquired(parsedDocument);	//	triggered when supplier is selected
	void hideCurrent();			//	hides current widget to show inner
	void backTo();				//	returns to previous step
	void backToStep(int step);	//	returns to specified step
	void continueToScaning(Document);	//	is triggered when all data is gathered and ScaningWidget can be created
};
