#include "ItemSearchWidget.h"

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
	searchButton(new QPushButton(this)), listHeaderLayout(new QHBoxLayout(this)),
	previousButton(new QPushButton(this)), nextButton(new QPushButton(this)),
	indexationInfo(new QLabel(this)), itemList(new specwidgets::_ItemSelectionList(allitems, this)),
	footerLayout(new QHBoxLayout(this)), backButton(new QPushButton(this)), 
	 currentpage(0)
{
	this->setLayout(mainLayout);
	mainLayout->addLayout(searchPanel);
	searchPanel->addWidget(searchInput);
	searchPanel->addWidget(searchButton);
	mainLayout->addLayout(listHeaderLayout);
	listHeaderLayout->addWidget(previousButton);
	listHeaderLayout->addWidget(nextButton);
	listHeaderLayout->addWidget(indexationInfo);
	mainLayout->addWidget(itemList);
	mainLayout->addLayout(footerLayout);
	footerLayout->addWidget(backButton);
	footerLayout->addStretch();

	searchButton->setText(tr("item_search_search"));
	indexationInfo->setText("0 — 0");
	backButton->setText(tr("item_search_back"));

#ifdef QT_VERSION5X

	QObject::connect(searchButton, &QPushButton::clicked, this, &ItemSearchWidget::doSearch);
	QObject::connect(nextButton, &QPushButton::clicked, this, &ItemSearchWidget::nextPage);
	QObject::connect(previousButton, &QPushButton::clicked, this, &ItemSearchWidget::previousPage);
	QObject::connect(itemList, &specwidgets::_ItemSelectionList::itemPicked, this, &ItemSearchWidget::itemSelected);
	QObject::connect(backButton, &QPushButton::clicked, this, &ItemSearchWidget::backRequired);

#else
	throw;
#endif


}

void ItemSearchWidget::refresh()
{
	indexationInfo->setText(allitems.from + " -- " + allitems.to);
	itemList->reload();
}

void ItemSearchWidget::doSearch()
{
	toSearch = searchInput->text();
	currentpage = 0;
	loadResults();
}

void ItemSearchWidget::loadResults()
{
	RequestAwaiter awaiter;
	globalSettings.networkingEngine->docSearchItems(toSearch, currentpage, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	while (awaiter.isAwaiting())
	{
		qApp->processEvents();
	}
	if (awaiter.wasTimeout())
	{
		return;
	}
	else
	{
		allitems = RequestParser::interpretAsSearchResponse(awaiter.restext, awaiter.errtext);
		refresh();
	}
	nextButton->setDisabled(allitems.last);
	previousButton->setDisabled(currentpage == 0);
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
	if (allitems.last)
		return;
	++currentpage;
	loadResults();
}

void ItemSearchWidget::previousPage()
{
	if (currentpage <= 0)
		return;
	--currentpage;
	loadResults();
}
