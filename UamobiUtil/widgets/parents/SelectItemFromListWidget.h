#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#else
 // Qt 4 only imports
#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QBoxLayout>
#include <QtGui/QLabel>

#endif
// widgets imports
#include "widgets/parents/IndependentBranchNode.h"
#include "widgets/parents/AbstractListSelectionWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
// networking imports
#include "networking/RequestAwaiter.h"
#include "networking/things.h"
// util imports
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/ElementsStyles.h"



class SelectItemFromListWidget : public IndependentBranchNode
{
	Q_OBJECT
protected:

	RecEntity prototype;
	DataEntityListModel *entityModel;
	QueryTemplates::OverloadableQuery loadQuery;


	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QHBoxLayout* headerLayout;
	QHBoxLayout* footerLayout;
	QLabel* userHelp;
	QLineEdit* userinputField;
	MegaIconButton* searchButton;
	MegaIconButton* ordfilterButton;
	QListView* itemSelection;
	MegaIconButton* backButton;

	QIcon withOrd;
	QIcon withoutOrd;
	RequestAwaiter* awaiter;
	RecEntity awaitedEntity;

	virtual void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
public:
	SelectItemFromListWidget(QWidget* parent,
		RecEntity proto);

	virtual void show() override;
	void setOverload(const QueryTemplates::OverloadableQuery& load);
protected slots:
	void searchPrimed();
	void ordFilterSwitched(bool);
	void parse_response();
	void parse_pick_response();
	void was_timeout();
	void itemPicked(RecEntity);
public slots:
	void loadItems();
signals:
	void done(RecEntity);

	// Inherited via IndependentBranchNode
};