#include "OrderSelectionWidget.h"

int specwidgets::_OrderListSelectionWidget::countElems()
{
	return allorders.count();
}

QString specwidgets::_OrderListSelectionWidget::elemToString(int i)
{
	return allorders.at(i).title + "|" + allorders.at(i).code+ "|" + allorders.at(i).text;
}

specwidgets::_OrderListSelectionWidget::_OrderListSelectionWidget(ordersResponse& ords, QWidget* parent)
	: AbstractListSelectionWidget(parent), allorders (ords)
{
	init();
}

void specwidgets::_OrderListSelectionWidget::itemSelectedFromList(QListWidgetItem*)
{
	emit orderPicked(allorders.at(currentRow()));
}

OrderSelectionWidget::OrderSelectionWidget(GlobalAppSettings& go, const parsedSupplier & suppl, QWidget* parent)
	: inframedWidget(parent), globalSettings(go), allOrders(), supplierInWork(suppl),
	mainLayout(new QVBoxLayout(this)), innerWidget(new QWidget(this)),
	innerLayout(new QVBoxLayout(innerWidget)), userInfo(new QLabel(innerWidget)),
	orderSelection(new specwidgets::_OrderListSelectionWidget(allOrders, innerWidget)),
	buttonLayout(new QHBoxLayout(innerWidget)), backButton(new QPushButton(innerWidget)),
	pickButton(new QPushButton(innerWidget))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(userInfo);
	innerLayout->addWidget(orderSelection);
	innerLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(backButton);
	buttonLayout->addStretch();
	buttonLayout->addWidget(pickButton);

	userInfo->setText(tr("order_selection_user_info"));
	pickButton->setText(tr("order_selection_pick_button"));
	backButton->setText(tr("order_selection_back"));

#ifdef QT_VERSION5X
	QObject::connect(backButton, &QPushButton::clicked, this, &OrderSelectionWidget::backRequired);
	QObject::connect(pickButton, &QPushButton::clicked, this, &OrderSelectionWidget::pickClicked);
	QObject::connect(orderSelection, &specwidgets::_OrderListSelectionWidget::orderPicked, this, &OrderSelectionWidget::orderSelected);

#else
	throw;
#endif

}

void OrderSelectionWidget::orderSelected(parsedOrder po)
{
	RequestAwaiter awaiter;
	globalSettings.networkingEngine->recGetOrderInfo(po.code, supplierInWork.code, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	while (awaiter.isAwaiting())
	{
		qApp->processEvents();
	}
	if (awaiter.wasTimeout())
	{
		userInfo->setText(tr("order_selection_timeout"));
	}
	else
	{

		parse_uniresults_functions::TypicalResponse resp =
			RequestParser::interpretAsRichtextResponse(awaiter.restext, awaiter.errtext);
		if (resp.resp)
		{
			detrace_METHEXPL("succesfully read")
			emit orderConfirmed(po, resp.errors);
		}
	}
}

void OrderSelectionWidget::setTimeoutMessage()
{
	userInfo->setText(tr("order_selection_timeout"));
}

void OrderSelectionWidget::loadOrders()
{
	RequestAwaiter awaiter;
	globalSettings.networkingEngine->recListOrders(supplierInWork.code, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	while (awaiter.isAwaiting())
	{
		qApp->processEvents();
	}
	if (awaiter.wasTimeout())
	{
		userInfo->setText(tr("order_selection_timeout"));
	}
	else
	{
		allOrders = RequestParser::interpretAsOrdersList(awaiter.restext, awaiter.errtext);
		orderSelection->reload();
	}
}
void OrderSelectionWidget::pickClicked()
{

}