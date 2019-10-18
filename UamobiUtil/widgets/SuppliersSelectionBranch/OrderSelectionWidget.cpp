#include "OrderSelectionWidget.h"
#include "widgets/utils/ElementsStyles.h"

int specwidgets::_OrderListSelectionWidget::countElems()
{
	return allorders.count();
}

QString specwidgets::_OrderListSelectionWidget::elemToString(int i)
{
	return allorders.at(i).title + "|" + allorders.at(i).code + "|" + allorders.at(i).text;
}

specwidgets::_OrderListSelectionWidget::_OrderListSelectionWidget(ordersResponse& ords, QWidget* parent)
	: AbstractListSelectionWidget(parent), allorders(ords)
{
	init();
}

void specwidgets::_OrderListSelectionWidget::itemSelectedFromList(QListWidgetItem*)
{
	emit orderPicked(allorders.at(currentRow()));
}

OrderSelectionWidget::OrderSelectionWidget(GlobalAppSettings& go, const parsedSupplier& suppl, QWidget* parent)
	: inframedWidget(parent), globalSettings(go), allOrders(), supplierInWork(suppl),
	mainLayout(new QVBoxLayout(this)), innerWidget(new QWidget(this)),
	innerLayout(new QVBoxLayout(innerWidget)), userInfo(new QLabel(innerWidget)),
	orderSelection(new specwidgets::_OrderListSelectionWidget(allOrders, innerWidget)),
	buttonLayout(new QHBoxLayout(innerWidget)), backButton(new MegaIconButton(innerWidget)),
	pickButton(new MegaIconButton(innerWidget)), awaiter(go.timeoutInt, this)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(userInfo);
	innerLayout->addWidget(orderSelection);
	innerLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(backButton);
	buttonLayout->addWidget(pickButton);

	userInfo->setText(tr("order_selection_user_info"));
	pickButton->setText(tr("order_selection_pick_button"));
	backButton->setText(tr("order_selection_back"));
	pickButton->setIcon(QIcon(":/res/single.png"));
	backButton->setIcon(QIcon(":/res/back.png"));
	pickButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	this->setFont(makeFont(0.04));
#ifdef QT_VERSION5X
	QObject::connect(backButton, &QPushButton::clicked, this, &OrderSelectionWidget::backRequired);
	QObject::connect(pickButton, &QPushButton::clicked, this, &OrderSelectionWidget::pickClicked);
	QObject::connect(orderSelection, &specwidgets::_OrderListSelectionWidget::orderPicked, this, &OrderSelectionWidget::orderSelected);
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &OrderSelectionWidget::was_timeout);
#else
	QObject::connect(backButton, SIGNAL(clicked), this, SIGNAL(backRequired()));
	QObject::connect(pickButton, SIGNAL(clicked()), this, SLOT(pickClicked()));
	QObject::connect(orderSelection, SIGNAL(orderPicked(parsedOrder)), this, SLOT(orderSelected(parsedOrder)));
#endif
}

bool OrderSelectionWidget::isExpectingControl(int val)
{
	if (awaiter.isAwaiting())
		return false;
	if (val >= -1 && val < allOrders.count() - 1)
	{
		if (val == -1)
		{
			if (allOrders.count() > 10)
				val = 9;
			else
			{
				emit backRequired();
				return false;
			}
		}
		orderSelected(allOrders.at(val));
		return true;
	}
	return false;
}

void OrderSelectionWidget::orderSelected(parsedOrder Po)
{
	if (awaiter.isAwaiting())
		return;
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &OrderSelectionWidget::parse_select_response);
	globalSettings.networkingEngine->recGetOrderInfo(po.code, supplierInWork.code, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	po = Po;
}


void OrderSelectionWidget::parse_order_response()
{
	allOrders = RequestParser::interpretAsOrdersList(awaiter.restext, awaiter.errtext);
	orderSelection->reload();
	if (!awaiter.errtext.isEmpty())
	{
		userInfo->setText(awaiter.errtext);
	}
	if (!allOrders.isEmpty())
	{
		orderSelection->setCurrentRow(0);
	}
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(parse_order_response()));
}

void OrderSelectionWidget::parse_select_response()
{
	parse_uniresults_functions::TypicalResponse resp =
		RequestParser::interpretAsRichtextResponse(awaiter.restext, awaiter.errtext);
	if (!awaiter.errtext.isEmpty())
	{
		userInfo->setText(awaiter.errtext);
	}
	if (resp.resp)
	{
#ifdef DEBUG
		//detrace_METHEXPL("succesfully read");
#endif
		emit orderConfirmed(po, resp.errors);
	}
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(parse_select_response()));
}

void OrderSelectionWidget::was_timeout()
{
	setTimeoutMessage();
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(parse_order_response()));
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(parse_select_response()));
}

void OrderSelectionWidget::setTimeoutMessage()
{
	userInfo->setText(tr("order_selection_timeout"));
}

void OrderSelectionWidget::loadOrders()
{
	if (awaiter.isAwaiting())
		return;
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &OrderSelectionWidget::parse_order_response);
	globalSettings.networkingEngine->recListOrders(supplierInWork.code, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
}
void OrderSelectionWidget::pickClicked()
{
	if (awaiter.isAwaiting())
		return;
	if (orderSelection->currentItem() != Q_NULLPTR)
	{
		orderSelected(allOrders.at(orderSelection->currentRow()));
	}
}