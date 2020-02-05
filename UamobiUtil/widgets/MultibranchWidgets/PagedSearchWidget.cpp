#include "PagedSearchWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"


void PagedSearchWidget::_handleRecord(RecEntity)
{
	loadResults();
}

void PagedSearchWidget::setIndexation(XmlObjects settings)
{
	if (settings.isEmpty())
		indexationInfo->setText("No indexation in data");
	else
	{
		indexationInfo->setText(
			settings.first()->value("from")
			+ " -- " +
			settings.first()->value("to")
		);
		nextButton->setDisabled(settings.first()->value("last") == true);
		previousButton->setDisabled(currentpage == 0);
	}
}

PagedSearchWidget::PagedSearchWidget(RecEntity proto, QWidget* parent)
	: IndependentBranchNode(independent_nodes::PagedSearch, true, parent),
	entityModel(new DataEntityListModel(this)),
	prototype(proto), mainLayout(new QVBoxLayout(this)),
	searchPanel(new QHBoxLayout(this)), searchInput(new QLineEdit(this)),
	searchButton(new MegaIconButton(this)), listHeaderLayout(new QHBoxLayout(this)),
	previousButton(new MegaIconButton(this)), nextButton(new MegaIconButton(this)),
	indexationInfo(new QLabel(this)), itemList(new QListView(this)),
	footerLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	currentpage(0), toSearch(), awaiter(new RequestAwaiter(AppSettings->timeoutInt, this))
{
	this->setLayout(mainLayout);
	mainLayout->addLayout(searchPanel);
	searchPanel->addWidget(searchInput);
	searchPanel->addWidget(searchButton);
	mainLayout->addLayout(listHeaderLayout);
	listHeaderLayout->addWidget(previousButton);
	listHeaderLayout->addWidget(indexationInfo);
	listHeaderLayout->addWidget(nextButton);
	mainLayout->addWidget(itemList);
	mainLayout->addLayout(footerLayout);
	footerLayout->addWidget(backButton);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	QFont scf = makeFont(0.04);

	searchButton->setText(tr("item_search_search"));
	searchButton->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);
	searchButton->setIcon(QIcon(":/res/search.png"));
	searchButton->setMinimumWidth(calculateAdaptiveWidth(0.2));
	searchButton->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum));

	indexationInfo->setText("0 " + QString(QChar(0x2014)) + " 0");
	indexationInfo->setFont(scf);
	indexationInfo->setAlignment(Qt::AlignCenter);

	backButton->setText(tr("item_search_back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	nextButton->setIcon(QIcon(":/res/nextpage.png"));
	nextButton->setStyleSheet(NAVIGATE_BUTTONS_STYLESHEET);
	nextButton->setDisabled(true);

	previousButton->setIcon(QIcon(":/res/prevpage.png"));
	previousButton->setStyleSheet(NAVIGATE_BUTTONS_STYLESHEET);
	previousButton->setDisabled(true);

	searchInput->setFont(scf);
	searchInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	searchInput->setMinimumWidth(calculateAdaptiveWidth(0.6));
	searchInput->setMaximumWidth(calculateAdaptiveWidth(0.8));
	searchInput->setMaximumHeight(calculateAdaptiveButtonHeight());

	itemList->setFont(scf);
#ifdef QT_VERSION5X

	QObject::connect(searchButton, &MegaIconButton::clicked, this, &PagedSearchWidget::doSearch);
	QObject::connect(nextButton, &MegaIconButton::clicked, this, &PagedSearchWidget::nextPage);
	QObject::connect(previousButton, &MegaIconButton::clicked, this, &PagedSearchWidget::previousPage);
	QObject::connect(itemList, &QListView::clicked, entityModel, &DataEntityListModel::mapClickToEntity);
	QObject::connect(entityModel, &DataEntityListModel::dataEntityClicked, this, &PagedSearchWidget::done);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &PagedSearchWidget::backRequired);
	QObject::connect(searchInput, &QLineEdit::editingFinished, this, &PagedSearchWidget::doSearch);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &PagedSearchWidget::was_timeout);
#else
	QObject::connect(searchButton, SIGNAL(clicked()), this, SLOT(doSearch()));
	QObject::connect(nextButton, SIGNAL(clicked()), this, SLOT(nextPage()));
	QObject::connect(previousButton, SIGNAL(clicked()), this, SLOT(previousPage()));
	QObject::connect(entityModel, SIGNAL(dataEntityClicked(RecEntity)), this, SIGNAL(done(RecEntity)));
	QObject::connect(itemList, SIGNAL(clicked(const QModelIndex&)), entityModel, SLOT(mapClickToEntity(const QModelIndex&)));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(searchInput, SIGNAL(editingFinished()), this, SLOT(doSearch()));
	QObject::connect(awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
#endif
}

void PagedSearchWidget::refresh()
{
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	PolyResponse response(RequestParser::parseResponse(parser, prototype));
	if (response.isError)
	{
		indexationInfo->setText(response.errtext);
	}
	else
	{
		setIndexation(response.additionalObjects);
		entityModel->setData(response.objects);
	}
}

void PagedSearchWidget::doSearch()
{
	if (awaiter->isAwaiting())
		return;
	toSearch = searchInput->text();
	currentpage = 0;
	loadResults();
}

void PagedSearchWidget::loadResults()
{
	if (awaiter->isAwaiting())
	{
		return;
	}
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &PagedSearchWidget::search_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(search_response()));
#endif
	prototype->sendAssociatedGetRequest(QStringList{ QString::number(currentpage) }, awaiter);
	showProcessingOverlay();
}

void PagedSearchWidget::clear()
{
	searchInput->setText("");
	entityModel->reset();
	toSearch = "";
	currentpage = 0;
}

void PagedSearchWidget::nextPage()
{
	if (awaiter->isAwaiting())
		return;
	++currentpage;
	loadResults();
}

void PagedSearchWidget::previousPage()
{
	if (awaiter->isAwaiting())
		return;
	if (currentpage <= 0)
		return;
	--currentpage;
	loadResults();
}

void PagedSearchWidget::search_response()
{
	refresh();
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void PagedSearchWidget::was_timeout()
{
	indexationInfo->setText(tr("item_search_timeout: ") + QString::number(awaiter->getInterval()));
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void PagedSearchWidget::_sendDataRequest()
{
	loadResults();
}
