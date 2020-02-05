#include "IdDependentSelectWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

void IdDependentSelectWidget::_handleRecord(RecEntity e)
{
	if (e.isNull())
#ifdef DEBUG
		detrace_METHPERROR("IdDependentSelectWidget::_handleRecord",
			"GIVEN WRONG ENTITY: EMPTY POINTER" );
#endif
	if (e->myType() == UniformXmlObject::Supplier)
		loadData(e);
	else
#ifdef DEBUG
		detrace_METHPERROR("IdDependentSelectWidget::_handleRecord",
			"GIVEN WRONG ENTITY: NO DEPENDECY.PROVIDED: " << e->myType());
#endif
}

IdDependentSelectWidget::IdDependentSelectWidget(RecEntity proto, QWidget* parent)
	: IndependentBranchNode(independent_nodes::IdDependentSelect, parent),
	prototype(proto), dependency(), entityModel(new DataEntityListModel(this)),
	mainLayout(new QVBoxLayout(this)), userInfo(new QLabel(this)),
	selectionView(new QListView(this)),
	buttonLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	pickButton(new MegaIconButton(this)), awaiter(new RequestAwaiter(AppSettings->timeoutInt, this))
{
	this->setLayout(mainLayout);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(userInfo);
	mainLayout->addWidget(selectionView);
	mainLayout->addLayout(buttonLayout);
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
	QObject::connect(backButton, &QPushButton::clicked, this, &IdDependentSelectWidget::backRequired);
	QObject::connect(pickButton, &QPushButton::clicked, this, &IdDependentSelectWidget::pickClicked);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &IdDependentSelectWidget::was_timeout);
#else
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(pickButton, SIGNAL(clicked()), this, SLOT(pickClicked()));
	QObject::connect(awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
#endif
}

bool IdDependentSelectWidget::isExpectingControl(int val)
{
	if (awaiter->isAwaiting())
		return false;
#ifdef DEBUG
	//detrace_METHCALL("isExpectingControl(" << val);
#endif
	if (val >= -1 && val <= entityModel->rowCount() - 1)
	{
		if (val == -1)
		{
			if (entityModel->rowCount() > 10)
				val = 9;
			else
			{
				emit backRequired();
				return false;
			}
		}
		QModelIndex index = entityModel->index(val);
		if (index.isValid())
		{
			entityModel->mapClickToEntity(index);
			return true;
		}
	}
	return false;
}

void IdDependentSelectWidget::itemSelected(RecEntity item)
{
#ifdef DEBUG
	//detrace_METHCALL("orderSelected(" << Po.text );
#endif
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &IdDependentSelectWidget::parse_select_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(parse_select_response()));
#endif
	awaitsConfirmation = item;
	AppNetwork->execQueryByTemplate(QueryTemplates::receiptGetOrderInfo, 
		QString::number(item->getId()), 
		QString::number(dependency->getId()), awaiter);
}

void IdDependentSelectWidget::parse_get_response()
{
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	PolyResponse response = RequestParser::parseResponse(parser , prototype);
	if (response.isError)
	{
		userInfo->setText(response.errtext);
	}
	else
	{
		entityModel->setData(response.objects);
	}
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void IdDependentSelectWidget::parse_select_response()
{
	if (awaiter->wasTimeout())
		return;
	RichtextResponseParser parser(awaiter->restext, awaiter->errtext);
	if (parser.isSuccessfull())
	{
		upcastRecord<OrderEntity>(awaitsConfirmation)->text = parser.read().first()->value("richtext");
		emit done(awaitsConfirmation);
	}
	else
	{
		userInfo->setText(parser.getErrors());
	}
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void IdDependentSelectWidget::was_timeout()
{
#ifdef DEBUG
	//	detrace_METHCALL("WasTimeout");
#endif
	setTimeoutMessage();
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void IdDependentSelectWidget::setTimeoutMessage()
{
	userInfo->setText(tr("order_selection_timeout"));
}

void IdDependentSelectWidget::_sendDataRequest()
{
	loadData(dependency);
}

void IdDependentSelectWidget::loadData(RecEntity dep)
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &IdDependentSelectWidget::parse_get_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(parse_order_response()));
#endif
	dependency.reset(dep->clone());
	dependency->sendAssociatedPostRequest(QStringList(), awaiter);
}
void IdDependentSelectWidget::pickClicked()
{
	if (awaiter->isAwaiting())
		return;
	if (selectionView->currentIndex().isValid())
	{
		entityModel->mapClickToEntity(selectionView->currentIndex());
	}
}