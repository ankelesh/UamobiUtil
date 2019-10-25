#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QLabel>
#include <QtWidgets/QBoxLayout>

#else
 // Qt 4 only imports
#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>

#endif

#include "widgets/parents/inframedWidget.h"
#include "widgets/parents/AbstractListSelectionWidget.h"
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

using parse_uniresults_functions::ordersResponse;
namespace specwidgets
{
	class _OrderListSelectionWidget : public AbstractListSelectionWidget
	{
		Q_OBJECT
	protected:
		ordersResponse& allorders;
		// Inherited via AbstractListSelectionWidget
		virtual int countElems() override;

		virtual QString elemToString(int);
	public:
		_OrderListSelectionWidget(ordersResponse& ords, QWidget* parent = Q_NULLPTR);
	private slots:
		virtual void itemSelectedFromList(QListWidgetItem*);
	signals:
		void orderPicked(parsedOrder);
	};
}

class OrderSelectionWidget : public inframedWidget
{
	Q_OBJECT
protected:
	GlobalAppSettings& globalSettings;
	const parsedSupplier& supplierInWork;

	ordersResponse allOrders;

	QVBoxLayout* mainLayout;
	QWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QLabel* userInfo;
	specwidgets::_OrderListSelectionWidget* orderSelection;
	QHBoxLayout* buttonLayout;
	MegaIconButton* backButton;
	MegaIconButton* pickButton;

	RequestAwaiter awaiter;

	parsedOrder po;
public:
	OrderSelectionWidget(GlobalAppSettings& go, const parsedSupplier& suppl, QWidget* = Q_NULLPTR);

	virtual bool isExpectingControl(int) override;
private slots:
	void pickClicked();
	void orderSelected(parsedOrder);
	void parse_order_response();
	void parse_select_response();
	void was_timeout();
public slots:
	void loadOrders();
	void setTimeoutMessage();
signals:
	void orderConfirmed(parsedOrder, QString);
};
