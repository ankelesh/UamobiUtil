#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QLabel>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QListView>
#else
 // Qt 4 only imports
#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>
#include <QtGui/QListView>
#endif

#include "widgets/parents/IndependentBranchNode.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "networking/RequestAwaiter.h"
#include "networking/Parsers/RequestParser.h"

/*
	This widget is oriented at allowing user to select order he needs.

	Warning: this widget requires supplier.

	Generally this widget is simplified version of SupplierSelect - no search.

	__ASSOCIATED_DATABASE_FUNCTION__   :  P'ordersResponse' list_orders(supplier_code)
	__ASSOCIATED_DATABASE_FUNCTION__   :  P'TypicalResponse' rec_get_order_info(order_code ,supplier_code)

*/

class IdDependentSelectWidget : public IndependentBranchNode
{
	Q_OBJECT
protected:
	RecEntity dependency;
	RecEntity prototype;

	DataEntityListModel* entityModel;

	QVBoxLayout* mainLayout;
	QLabel* userInfo;
	QListView* selectionView;
	QHBoxLayout* buttonLayout;
	MegaIconButton* backButton;
	MegaIconButton* pickButton;

	RequestAwaiter* awaiter;

	RecEntity awaitsConfirmation;

	void _handleRecord(RecEntity) override;
public:
	IdDependentSelectWidget(RecEntity proto, QWidget* = Q_NULLPTR);

	virtual bool isExpectingControl(int) override;
private slots:
	void pickClicked();
	void itemSelected(RecEntity);
	void parse_get_response();
	void parse_select_response();
	void was_timeout();
public slots:
	void loadData(RecEntity dependency);
	void setTimeoutMessage();

	// Inherited via IndependentBranchNode
	virtual void _sendDataRequest() override;
};
