#include "ItemSearchWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
int specwidgets::_ItemSelectionList::countElems()
{
	return allitems.count();
}

void specwidgets::_ItemSelectionList::itemSelectedFromList(QListWidgetItem*)
{
	emit itemPicked(allitems.at(currentRow()));
}

QString specwidgets::_ItemSelectionList::elemToString(int ind)
{
	return allitems.at(ind).title;
}

specwidgets::_ItemSelectionList::_ItemSelectionList(parse_uniresults_functions::searchResponse & items, QWidget * parent)
	: AbstractListSelectionWidget(parent), allitems(items.values)
{
	init();
}

ItemSearchWidget::ItemSearchWidget(GlobalAppSettings & go, QWidget* parent)
	: inframedWidget(parent), allitems(), globalSettings(go), mainLayout(new QVBoxLayout(this)),
	searchPanel(new QHBoxLayout(this)),searchInput(new QLineEdit(this)),
	searchButton(new MegaIconButton(this)), listHeaderLayout(new QHBoxLayout(this)),
	previousButton(new MegaIconButton(this)), nextButton(new MegaIconButton(this)),
	indexationInfo(new QLabel(this)), itemList(new specwidgets::_ItemSelectionList(allitems, this)),
	footerLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)), 
	 currentpage(0)
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

	indexationInfo->setText("0 " + QString(QChar(0x2014))+" 0");
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

	QObject::connect(searchButton, &MegaIconButton::clicked, this, &ItemSearchWidget::doSearch);
	QObject::connect(nextButton, &MegaIconButton::clicked, this, &ItemSearchWidget::nextPage);
	QObject::connect(previousButton, &MegaIconButton::clicked, this, &ItemSearchWidget::previousPage);
	QObject::connect(itemList, &specwidgets::_ItemSelectionList::itemPicked, this, &ItemSearchWidget::itemSelected);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &ItemSearchWidget::backRequired);
	QObject::connect(searchInput, &QLineEdit::editingFinished, this, &ItemSearchWidget::doSearch);
	QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &ItemSearchWidget::was_timeout);
#else
    QObject::connect(searchButton, SIGNAL(clicked()), this, SLOT(doSearch()));
    QObject::connect(nextButton, SIGNAL(clicked()), this, SLOT(nextPage()));
    QObject::connect(previousButton, SIGNAL(clicked()), this, SLOT(previousPage()));
    QObject::connect(itemList, SIGNAL(itemPicked(parsedItemSimplified)), this, SIGNAL(itemSelected(parsedItemSimplified)));
    QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
    QObject::connect(searchInput, SIGNAL(editingFinished()), this, SLOT(doSearch()));
    QObject::connect(&awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
#endif


}

void ItemSearchWidget::refresh()
{
	indexationInfo->setText(allitems.from + " " + QChar(0x2014) + " " + allitems.to);
	itemList->reload();
}

void ItemSearchWidget::doSearch()
{
	if (awaiter.isAwaiting())
		return;
	toSearch = searchInput->text();
	currentpage = 0;
	loadResults();
}

void ItemSearchWidget::loadResults()
{
	if (awaiter.isAwaiting())
	{
		return;
	}
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &ItemSearchWidget::search_response);
#else
    QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(search_response()));
#endif
    globalSettings.networkingEngine->docSearchItems(toSearch, currentpage, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	nextButton->setDisabled(allitems.last);
	previousButton->setDisabled(currentpage == 0);
	showProcessingOverlay();
}

void ItemSearchWidget::clear()
{
	searchInput->setText("");
	itemList->clear();
	toSearch = "";
	currentpage = 0;
}

void ItemSearchWidget::nextPage()
{
	if (awaiter.isAwaiting())
		return;
	if (allitems.last)
		return;
	++currentpage;
	loadResults();
}

void ItemSearchWidget::previousPage()
{
	if (awaiter.isAwaiting())
		return;
	if (currentpage <= 0)
		return;
	--currentpage;
	loadResults();
}

void ItemSearchWidget::search_response()
{
	allitems = RequestParser::interpretAsSearchResponse(awaiter.restext, awaiter.errtext);
	refresh();
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void ItemSearchWidget::was_timeout()
{
	itemList->clear();
	itemList->addItem(tr("item_search_timeout: ") + QString::number(globalSettings.timeoutInt));
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}
