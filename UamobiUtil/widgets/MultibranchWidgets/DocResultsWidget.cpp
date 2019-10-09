#include "DocResultsWidget.h"

DocResultsWidget::DocResultsWidget(GlobalAppSettings & go, QWidget* parent)
	: inframedWidget(parent), globalSettings(go), mainLayout(new QVBoxLayout(this)),
	userInfo(new QLabel(this)), listHeaderLayout(new QHBoxLayout(this)),
	previousButton(new QPushButton(this)), indexationInfo(new QLabel(this)),
	nextButton(new QPushButton(this)), itemInfoStorage(new QListWidget(this)),
	footerLayout(new QHBoxLayout(this)), backButton(new QPushButton(this)),
	saveButton(new QPushButton(this)), items(), pagenumber(0)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(userInfo);
	mainLayout->addLayout(listHeaderLayout);
	listHeaderLayout->addWidget(previousButton);
	listHeaderLayout->addWidget(indexationInfo);
	listHeaderLayout->addWidget(nextButton);
	mainLayout->addWidget(itemInfoStorage);
	mainLayout->addLayout(footerLayout);
	footerLayout->addWidget(backButton);
	footerLayout->addStretch();
	footerLayout->addWidget(saveButton);

	QSizePolicy mi(QSizePolicy::Maximum, QSizePolicy::Maximum);
	QSizePolicy ma(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

	previousButton->setSizePolicy(mi);
	nextButton->setSizePolicy(mi);
	indexationInfo->setSizePolicy(ma);

	//TODO: icons

	backButton->setText(tr("doc_results_back"));
	saveButton->setText(tr("doc_results_text"));
	userInfo->setText(tr("doc_results_userinfo"));
#ifdef QT_VERSION5X

	QObject::connect(backButton, &QPushButton::clicked, this, &DocResultsWidget::backRequired);
	QObject::connect(saveButton, &QPushButton::clicked, this, &DocResultsWidget::saveDocument);
	QObject::connect(nextButton, &QPushButton::clicked, this, &DocResultsWidget::nextPage);
	QObject::connect(previousButton, &QPushButton::clicked, this, &DocResultsWidget::previousPage);
#else
	throw;
#endif

}

void DocResultsWidget::loadItems()
{
	RequestAwaiter awaiter;
	globalSettings.networkingEngine->docGetResults(pagenumber, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	while (awaiter.isAwaiting())
	{
		qApp->processEvents();
	}
	if (awaiter.wasTimeout())
	{
		userInfo->setText(tr("doc_results_timeout"));
	}
	else
	{
		items = RequestParser::interpretAsListedDocument(awaiter.restext, awaiter.errtext);
		refresh();
	}
}

void DocResultsWidget::refresh()
{
	itemInfoStorage->clear();
	indexationInfo->setText(items.from + " -- " + items.to);
	QVector<parsedItem>::iterator start = items.values.begin();
	while (start != items.values.end())
	{
		itemInfoStorage->addItem(start++->title);
	}
	nextButton->setDisabled(items.last);
	previousButton->setDisabled(pagenumber = 0);
}

void DocResultsWidget::show()
{
	pagenumber = 0;
	loadItems();
	inframedWidget::show();
}

void DocResultsWidget::previousPage()
{
	if (pagenumber == 0)
		return;
	--pagenumber;
	loadItems();
}

void DocResultsWidget::nextPage()
{
	if (items.last)
		return;
	++pagenumber;
	loadItems();
}

void DocResultsWidget::saveDocument()
{
	RequestAwaiter awaiter;
	globalSettings.networkingEngine->docUnlock(true, &awaiter, RECEIVER_SLOT_NAME);
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
		if (awaiter.restext.contains("_"))
			emit documentSaved();
	}
}
