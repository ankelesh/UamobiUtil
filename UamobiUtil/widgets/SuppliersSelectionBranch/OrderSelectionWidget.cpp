#include "OrderSelectionWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

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
	orderSelection->installEventFilter(keyfilter);
	innerWidget->installEventFilter(keyfilter);

#ifdef QT_VERSION5X
	QObject::connect(backButton, &QPushButton::clicked, this, &OrderSelectionWidget::backRequired);
	QObject::connect(pickButton, &QPushButton::clicked, this, &OrderSelectionWidget::pickClicked);
	QObject::connect(orderSelection, &specwidgets::_OrderListSelectionWidget::orderPicked, this, &OrderSelectionWidget::orderSelected);
	QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &OrderSelectionWidget::was_timeout);
#else
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(pickButton, SIGNAL(clicked()), this, SLOT(pickClicked()));
	QObject::connect(orderSelection, SIGNAL(orderPicked(parsedOrder)), this, SLOT(orderSelected(parsedOrder)));
	QObject::connect(&awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
#endif
}

bool OrderSelectionWidget::isExpectingControl(int val)
{
	if (awaiter.isAwaiting())
		return false;
#ifdef DEBUG
	//detrace_METHCALL("isExpectingControl(" << val);
#endif
	if (val >= -1 && val <= allOrders.count() - 1)
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
#ifdef DEBUG
	//detrace_METHCALL("orderSelected(" << Po.text );
#endif
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &OrderSelectionWidget::parse_select_response);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(parse_select_response()));
#endif
	po = Po;
	globalSettings.networkingEngine->recGetOrderInfo(po.code, supplierInWork.code, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
}

void OrderSelectionWidget::parse_order_response()
{
	allOrders = RequestParser::interpretAsOrdersList(awaiter.restext, awaiter.errtext);
#ifdef DEBUG
	parse_uniresults_functions::ordersResponse temp;
	for (int i = 0; i < allOrders.count(); ++i)
	{
		if (allOrders.at(i).code == "18833981")
			temp.push_back(allOrders.at(i));
	}
	allOrders = temp;
#endif
	orderSelection->reload();
	if (!awaiter.errtext.isEmpty())
	{
		userInfo->setText(awaiter.errtext);
	}
	if (!allOrders.isEmpty())
	{
		orderSelection->setCurrentRow(0);
	}
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void OrderSelectionWidget::parse_select_response()
{
	if (awaiter.wasTimeout())
		return;
	parse_uniresults_functions::TypicalResponse resp =
		RequestParser::interpretAsRichtextResponse(awaiter.restext, awaiter.errtext);
#ifdef DEBUG
	//detrace_METHEXPL("resp status: " << resp.resp);
#endif
	if (!awaiter.errtext.isEmpty())
	{
		userInfo->setText(awaiter.errtext);
	}
	if (resp.resp)
	{
#ifdef DEBUG
		//		detrace_METHEXPL("text transmitted to parms: " << resp.errors << "| while original package packet was " << awaiter.restext);
#endif
		emit orderConfirmed(po, resp.errors);
	}
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void OrderSelectionWidget::was_timeout()
{
#ifdef DEBUG
	//	detrace_METHCALL("WasTimeout");
#endif
	setTimeoutMessage();
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void OrderSelectionWidget::setTimeoutMessage()
{
	userInfo->setText(tr("order_selection_timeout"));
}

void OrderSelectionWidget::loadOrders()
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &OrderSelectionWidget::parse_order_response);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(parse_order_response()));
#endif
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