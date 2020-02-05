#include "SelectItemFromListWidget.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "networking/Parsers/RequestParser.h"


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
	//footerLayout->addStretch();
	QFont scf = makeFont(0.04);
	userHelp->setText(tr("suppliers_selection_widget_user_tip"));
	userHelp->setFont(scf);
	userHelp->setAlignment(Qt::AlignCenter);

	searchButton->setIcon(QIcon(":/res/search.png"));
	searchButton->setText(tr("suppliers_selection_search!"));
	searchButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	searchButton->setMinimumWidth(calculateAdaptiveWidth(0.2));
	ordfilterButton->setMinimumWidth(calculateAdaptiveWidth(0.2));

	userinputField->setFont(scf);
	userinputField->setMaximumWidth(calculateAdaptiveWidth(0.6));
	userinputField->setMinimumWidth(calculateAdaptiveWidth(0.4));
	userinputField->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	userinputField->setMaximumHeight(calculateAdaptiveButtonHeight());

	backButton->setText(tr("suppliers_selection_back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	ordfilterButton->setText(tr("W\\O"));
	ordfilterButton->setCheckable(true);
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
#ifdef QT_VERSION5X
	QObject::connect(searchButton, &QPushButton::clicked, this, &SelectItemFromListWidget::searchPrimed);
	QObject::connect(ordfilterButton, &QPushButton::toggled, this, &SelectItemFromListWidget::ordFilterSwitched);
	QObject::connect(backButton, &QPushButton::clicked, this, &SelectItemFromListWidget::backRequired);
	QObject::connect(userinputField, &QLineEdit::editingFinished, this, &SelectItemFromListWidget::searchPrimed);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &SelectItemFromListWidget::was_timeout);
	QObject::connect(entityModel, &DataEntityListModel::dataEntityClicked, this, &SelectItemFromListWidget::itemPicked);
	QObject::connect(itemSelection, &QListView::clicked, entityModel, &DataEntityListModel::mapClickToEntity);
#else
	QObject::connect(searchButton, SIGNAL(clicked()), this, SLOT(searchPrimed()));
	QObject::connect(ordfilterButton, SIGNAL(toggled(bool)), this, SLOT(ordFilterSwitched(bool)));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(userinputField, SIGNAL(editingFinished()), this, SLOT(searchPrimed()));
	QObject::connect(awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
	QObject::connect(entityModel, SIGNAL(dataEntityClicked(RecEntity)), this, SLOT(itemPicked(RecEntity)));
	QObject::connect(itemSelection, SIGNAL(clicked(const QModelIndex&)), entityModel, SLOT(mapClickToEntity(const QModelIndex&)));
#endif
}

void SelectItemFromListWidget::show()
{
    loadItems();
	inframedWidget::show();
	setFocus();
}

void SelectItemFromListWidget::setOverload(const QueryTemplates::OverloadableQuery& oq)
{
	loadQuery = oq;
}


void SelectItemFromListWidget::searchPrimed()
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &SelectItemFromListWidget::parse_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(parse_response()));
#endif
	if (!loadQuery.isDefault())
	{
		AppNetwork->execQueryByTemplate(loadQuery,
			userinputField->text(),
			((ordfilterButton->isChecked()) ? "hasorders=true" : ""),
				awaiter);
		return;
	}
	QStringList buffer;
	buffer << userinputField->text() << ((ordfilterButton->isChecked()) ? "hasorders=true" : "");
	prototype->sendAssociatedGetRequest(buffer, awaiter);
}

void SelectItemFromListWidget::ordFilterSwitched(bool)
{
	if (ordfilterButton->isChecked())
	{
		ordfilterButton->setIcon(withOrd);
		ordfilterButton->setStyleSheet(CHECKED_BUTTONS_STYLESHEET);
	}
	else
	{
		ordfilterButton->setIcon(withoutOrd);
		ordfilterButton->setStyleSheet(UNCHECKED_BUTTONS_STYLESHEET);
	}
}


void SelectItemFromListWidget::parse_response()
{
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	PolyResponse response = RequestParser::parseResponse(parser, prototype);
	if (response.isError)
	{
		userHelp->setText(response.errtext);
	}
	else
	{
		entityModel->setData(response.objects);
	}
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void SelectItemFromListWidget::parse_pick_response()
{
	SimpliestResponceParser parser(awaiter->restext, awaiter->errtext);
	if (parser.isSuccessfull())
	{
		emit done(RecEntity(awaitedEntity->clone()));
	}
	else
	{
		userHelp->setText(parser.getErrors());
	}
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void SelectItemFromListWidget::was_timeout()
{
	userHelp->setText(tr("suppliers_select_widget_timeout!") + QString::number(awaiter->getInterval()));
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void SelectItemFromListWidget::itemPicked(RecEntity e)
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &SelectItemFromListWidget::parse_pick_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(parse_pick_response()));
#endif
	awaitedEntity = e;
	e->sendAssociatedPostRequest(QStringList(), awaiter);
}

void SelectItemFromListWidget::loadItems()
{
	searchPrimed();
}

void SelectItemFromListWidget::_sendDataRequest()
{
	loadItems();
}
