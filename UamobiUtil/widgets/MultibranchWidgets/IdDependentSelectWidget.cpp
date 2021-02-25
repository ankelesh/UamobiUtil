#include "IdDependentSelectWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "networking/Parsers/RequestParser.h"
#include "widgets/ExtendedDelegates/ZebraListItemDelegate.h"

#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif

void IdDependentSelectWidget::_handleRecord(RecEntity e)
{
	if (e.isNull())
	{
#ifdef DEBUG
		detrace_METHPERROR("IdDependentSelectWidget::_handleRecord",
			"GIVEN WRONG ENTITY: EMPTY POINTER");
#endif
		return;
	}
	loadData(e);
}

IdDependentSelectWidget::IdDependentSelectWidget(RecEntity proto, QWidget* parent)
	: IndependentBranchNode(independent_nodes::IdDependentSelect, parent),
     dependency(),prototype(proto), entityModel(new DataEntityListModel(this)),
	mainLayout(new QVBoxLayout(this)), userInfo(new QLabel(this)),
	selectionView(new QListView(this)),
	buttonLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	pickButton(new MegaIconButton(this)), awaiter(new RequestAwaiter(AppSettings->timeoutInt, this)),
	awaitsConfirmation(), localCache()
{
#ifdef DEBUG
	detrace_DCONSTR("IdDependentSelect");
#endif
	this->setLayout(mainLayout);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(userInfo);
	mainLayout->addWidget(selectionView);
	mainLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(backButton);
	buttonLayout->addWidget(pickButton);

    userInfo->setText(tr("Please select item selected by: "));
    userInfo->setWordWrap(true);
    pickButton->setText(tr("pick_button"));
    backButton->setText(tr("back"));
	pickButton->setIcon(QIcon(":/resources/single"));
	backButton->setIcon(QIcon(":/resources/back"));
	pickButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	this->setFont(GENERAL_FONT);
	selectionView->setModel(entityModel);
	selectionView->setItemDelegate(new ZebraItemDelegate(this));
	_captureNumbers();
#ifdef QT_VERSION5X
	QObject::connect(backButton, &QPushButton::clicked, this, &IdDependentSelectWidget::backRequired);
	QObject::connect(pickButton, &QPushButton::clicked, this, &IdDependentSelectWidget::pickClicked);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &IdDependentSelectWidget::was_timeout);
	QObject::connect(selectionView, &QListView::clicked, entityModel, &DataEntityListModel::mapClickToEntity);
	QObject::connect(entityModel, &DataEntityListModel::dataEntityClicked, this, &IdDependentSelectWidget::itemSelected);
#else
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(pickButton, SIGNAL(clicked()), this, SLOT(pickClicked()));
	QObject::connect(awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
    QObject::connect(selectionView, SIGNAL(clicked(QModelIndex)), entityModel, SLOT(mapClickToEntity(QModelIndex)));
    QObject::connect(entityModel, SIGNAL(dataEntityClicked(RecEntity)), this, SLOT(itemSelected(RecEntity)));
#endif
}

void IdDependentSelectWidget::_numberReaction(int val)
{
	if (awaiter->isAwaiting())
		return;
	if (val >= -1 && val <= entityModel->rowCount() - 1)
	{
		if (val == -1)
		{
			if (entityModel->rowCount() > 10)
				val = 9;
			else
			{
				emit backRequired();
				return;
			}
		}
		QModelIndex index = entityModel->index(val);
		if (index.isValid())
		{
			entityModel->mapClickToEntity(index);
		}
	}
}

void IdDependentSelectWidget::_arrowReaction(int arrow)
{
	if (!selectionView->hasFocus())
	{
		selectionView->setCurrentIndex(entityModel->moveByArrow(arrow, selectionView->currentIndex()));
	}
}

void IdDependentSelectWidget::_returnReaction()
{
	entityModel->mapClickToEntity(selectionView->currentIndex());
}

void IdDependentSelectWidget::itemSelected(RecEntity item)
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &IdDependentSelectWidget::parse_select_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(parse_select_response()));
#endif
	awaitsConfirmation = item;
	if (localCache.contains(receiptGetOrderInfo))
		AppNetwork->execQueryByTemplate(localCache[receiptGetOrderInfo],
			item->getId(), dependency->getId(), awaiter);
	else
		AppNetwork->execQueryByTemplate(QueryTemplates::receiptGetOrderInfo, 
			item->getId(), 
			dependency->getId(), awaiter);
}

void IdDependentSelectWidget::parse_get_response()
{
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	PolyResponse response = RequestParser::parseResponse(parser , prototype);
	if (response.isError)
	{
		userInfo->setText(response.errtext); 
#ifdef DEBUG
			detrace_NRESPERR(response.errtext);
#endif
	}
	else if (response.objects.isEmpty())
	{
		userInfo->setText(tr("Request received, but no data provided: ") + dependency->getId());
	}
	else
	{
        entityModel->insertData(response.objects);
		selectionView->setCurrentIndex(entityModel->index(0));
	}
    QObject::disconnect(awaiter, SIGNAL(requestReceived()), Q_NULLPTR, Q_NULLPTR);
	hideProcessingOverlay();
}

void IdDependentSelectWidget::parse_select_response()
{
	if (awaiter->wasTimeout())
		return;
	RichtextResponseParser parser(awaiter->restext, awaiter->errtext);
	if (parser.isSuccessfull())
	{
		upcastRecord<OrderEntity>(awaitsConfirmation)->text = parser.read().first()->value("richdata");
		emit done(awaitsConfirmation);
	}
	else
	{
		userInfo->setText(parser.getErrors());
#ifdef DEBUG
		detrace_NRESPERR(parser.getErrors());
#endif
	}
    QObject::disconnect(awaiter, SIGNAL(requestReceived()), Q_NULLPTR, Q_NULLPTR);
	hideProcessingOverlay();
}

void IdDependentSelectWidget::was_timeout()
{
	userInfo->setText(tr("timeout: ") + QString::number(awaiter->getInterval()));
    QObject::disconnect(awaiter, SIGNAL(requestReceived()), Q_NULLPTR, Q_NULLPTR);
	hideProcessingOverlay();
}


void IdDependentSelectWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	switch (
		((overloads.count() > 2) ? 2 : overloads.count())
		)
	{
	case 2:
    {
        QStringList t;
        t<<"id";
        localCache.insert(receiptListOrders, overloads.at(1).assertedAndMappedCopy(
            receiptListOrders, t,t
		));
        Q_FALLTHROUGH();
    }
	case 1:
    {
        QStringList t;
        t << "itemId" << "depId";
        localCache.insert(receiptGetOrderInfo, overloads.at(0).assertedAndMappedCopy(
            receiptGetOrderInfo, t,t));
        break;
    }
	default:
		return;
	}
}

void IdDependentSelectWidget::_sendDataRequest()
{
}

void IdDependentSelectWidget::loadData(RecEntity dep)
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &IdDependentSelectWidget::parse_get_response);
#else
    QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(parse_get_response()));
#endif
    userInfo->setText(tr("Please select item selected by: ") + dep->getId());
    dependency = RecEntity(dep->clone());
	if (localCache.contains(receiptListOrders))
		AppNetwork->execQueryByTemplate(localCache[receiptListOrders], dependency->getId(), awaiter);
	else
    {
        QStringList t;
        t << dependency->getId();
        prototype->sendAssociatedGetRequest(t, awaiter);
    }
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
