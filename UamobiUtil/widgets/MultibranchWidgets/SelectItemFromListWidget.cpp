#include "SelectItemFromListWidget.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "networking/Parsers/RequestParser.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/ExtendedDelegates/ZebraListItemDelegate.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif

bool requiresOrd(int id)
{
	switch (id) 
	{
	case UniformXmlObject::Supplier:
		return true;
	default:
		return false;
	}
}

enum awaitingMethods
{
	Pick=4,
	Get
};

void SelectItemFromListWidget::_handleRecord(RecEntity)
{
}

SelectItemFromListWidget::SelectItemFromListWidget(
	QWidget* parent, RecEntity proto)
	: IndependentBranchNode(independent_nodes::SelectItemFromList, true,parent), 
	prototype(proto),
	entityModel(new DataEntityListModel(this)),  
	loadQuery(QueryTemplates::OverloadableQuery::defaultQuery()),
	mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	headerLayout(new QHBoxLayout(innerWidget)), footerLayout(new QHBoxLayout(innerWidget)),
	userHelp(new QLabel(innerWidget)), userinputField(new QLineEdit(innerWidget)),
	searchButton(new MegaIconButton(innerWidget)), ordfilterButton(new MegaIconButton(innerWidget)),
	itemSelection(new QListView(this)),
	backButton(new MegaIconButton(innerWidget)), withOrd(":/res/with.png"), withoutOrd(":/res/without.png"),
	awaiter(new RequestAwaiter(AppSettings->timeoutInt, this))
{
#ifdef DEBUG
	detrace_DCONSTR("SelectItemFromList");
#endif
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	innerWidget->setLayout(innerLayout);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	innerLayout->addWidget(userHelp);
	innerLayout->addLayout(headerLayout);
	innerLayout->addWidget(itemSelection);
	innerLayout->addLayout(footerLayout);
	headerLayout->addWidget(userinputField);
	headerLayout->addWidget(searchButton);
	headerLayout->addWidget(ordfilterButton);
	footerLayout->addWidget(backButton);
    userHelp->setText(tr("Please select item from list!"));
	userHelp->setFont(GENERAL_FONT);
	userHelp->setAlignment(Qt::AlignCenter);

	searchButton->setIcon(QIcon(":/res/search.png"));
    searchButton->setText(tr("Search"));
	searchButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	searchButton->setMinimumWidth(calculateAdaptiveWidth(0.2));
    searchButton->setMaximumWidth(calculateAdaptiveWidth(0.2));
	ordfilterButton->setMinimumWidth(calculateAdaptiveWidth(0.2));
    ordfilterButton->setMaximumWidth(calculateAdaptiveWidth(0.2));

	userinputField->setFont(GENERAL_FONT);
	userinputField->setMaximumWidth(calculateAdaptiveWidth(0.6));
	userinputField->setMinimumWidth(calculateAdaptiveWidth(0.4));
	userinputField->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	userinputField->setMaximumHeight(calculateAdaptiveButtonHeight());

    backButton->setText(tr("back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	ordfilterButton->setText(tr("W\\O"));
	ordfilterButton->setCheckable(true);
	ordfilterButton->setStyleSheet(CHECKBOX_BUTTON_STYLESHEET);
	ordFilterSwitched(true);
	innerWidget->installEventFilter(keyfilter);
	if (!requiresOrd(prototype->myType()))
	{
		ordfilterButton->setChecked(false);
		ordfilterButton->hide();
	}
	else
	{
		ordfilterButton->setChecked(true);
	}
	itemSelection->setModel(entityModel);
	itemSelection->setItemDelegate(new ZebraItemDelegate(this));
    userinputField->disconnect();
#ifdef QT_VERSION5X
	QObject::connect(searchButton, &QPushButton::clicked, this, &SelectItemFromListWidget::searchPrimed);
	QObject::connect(ordfilterButton, &QPushButton::toggled, this, &SelectItemFromListWidget::ordFilterSwitched);
	QObject::connect(backButton, &QPushButton::clicked, this, &SelectItemFromListWidget::backRequired);
	QObject::connect(userinputField, &QLineEdit::returnPressed, this, &SelectItemFromListWidget::searchPrimed);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &SelectItemFromListWidget::was_timeout);
	QObject::connect(entityModel, &DataEntityListModel::dataEntityClicked, this, &SelectItemFromListWidget::itemPicked);
	QObject::connect(itemSelection, &QListView::clicked, entityModel, &DataEntityListModel::mapClickToEntity);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &SelectItemFromListWidget::parse_pick_response);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &SelectItemFromListWidget::parse_response);
#else
	QObject::connect(searchButton, SIGNAL(clicked()), this, SLOT(searchPrimed()));
	QObject::connect(ordfilterButton, SIGNAL(toggled(bool)), this, SLOT(ordFilterSwitched(bool)));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
    QObject::connect( userinputField, SIGNAL(returnPressed()), this, SLOT(searchPrimed()), Qt::QueuedConnection);
	QObject::connect(awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
	QObject::connect(entityModel, SIGNAL(dataEntityClicked(RecEntity)), this, SLOT(itemPicked(RecEntity)));
	QObject::connect(itemSelection, SIGNAL(clicked(QModelIndex)), entityModel, SLOT(mapClickToEntity(QModelIndex)));
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(parse_response()));
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(parse_pick_response()));
#endif
}

void SelectItemFromListWidget::show()
{
	inframedWidget::show();
	setFocus();
}



void SelectItemFromListWidget::searchPrimed()
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
	if (!loadQuery.isDefault())
	{
		AppNetwork->execQueryByTemplate(loadQuery,
			userinputField->text(),
			((ordfilterButton->isChecked()) ? "true" : "false"),
				awaiter);
		awaiter->deliverResultTo(Get);
		return;
	}
	QStringList buffer;
	buffer << userinputField->text() << ((ordfilterButton->isChecked()) ? "true" : "false");
	prototype->sendAssociatedGetRequest(buffer, awaiter);
	awaiter->deliverResultTo(Get);
}

void SelectItemFromListWidget::ordFilterSwitched(bool)
{
	if (ordfilterButton->isChecked())
	{
		ordfilterButton->setIcon(withOrd);
	}
	else
	{
		ordfilterButton->setIcon(withoutOrd);
	}
}


void SelectItemFromListWidget::parse_response()
{
	if (!awaiter->deliverHere(Get))
		return;
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	PolyResponse response = RequestParser::parseResponse(parser, prototype);
	if (response.isError)
	{
		userHelp->setText(response.errtext);
#ifdef DEBUG
		detrace_NRESPERR(response.errtext);
#endif
	}
	else if (response.isEmpty())
	{
		userHelp->setText(tr("no data received!"));
	}
	else
	{
        entityModel->insertData(response.objects);
	}
	hideProcessingOverlay();
}

bool SelectItemFromListWidget::isExpectingControl(int val)
{
	if (awaiter->isAwaiting())
		return false;
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

void SelectItemFromListWidget::parse_pick_response()
{
	if (!awaiter->deliverHere(Pick))
		return;
	SimpliestResponceParser parser(awaiter->restext, awaiter->errtext);
	if (parser.isSuccessfull())
	{
		emit done(RecEntity(awaitedEntity->clone()));
	}
	else
	{
		userHelp->setText(parser.getErrors());
#ifdef DEBUG
		detrace_NRESPERR(parser.getErrors());
#endif
	}
	hideProcessingOverlay();
}

void SelectItemFromListWidget::was_timeout()
{
    userHelp->setText(tr("timeout: ") + QString::number(awaiter->getInterval()));
	hideProcessingOverlay();
}

void SelectItemFromListWidget::itemPicked(RecEntity e)
{
	if (awaiter->isAwaiting())
		return;
	awaitedEntity = e;
	if (selectQuery.isDefault())
	{
		if (!e->sendAssociatedPostRequest(QStringList(), awaiter))
		{
			emit done(RecEntity(e->clone()));
		}
	}
	else
	{
		AppNetwork->execQueryByTemplate(selectQuery, e->getId(), awaiter);
	}
	awaiter->deliverResultTo(Pick);
	showProcessingOverlay();
}


void SelectItemFromListWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	switch (
		((overloads.count() > 2) ? 2 : overloads.count())
		)
	{
	case 2:
    {
        QStringList t;
        t << "id";
        selectQuery = overloads.at(1).assertedAndMappedCopy(
            QueryTemplates::applyBarcodeFilter,t,t
        );
    }
        Q_FALLTHROUGH();
	case 1:
    {
        QStringList t;
        t << "text" << "has_orders";
		QStringList t2;
		t2 << "text";

		loadQuery = overloads.first().assertedAndMappedCopy(
			QueryTemplates::receiptListSuppliers,
            t,t2
		);
    }
        Q_FALLTHROUGH();
	default:
		return;
	}
}

void SelectItemFromListWidget::_sendDataRequest()
{
	searchPrimed();
}
