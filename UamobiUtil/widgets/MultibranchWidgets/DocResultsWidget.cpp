#include "DocResultsWidget.h"
#include "widgets/utils/ElementsStyles.h"

DocResultsWidget::DocResultsWidget(GlobalAppSettings & go, QWidget* parent)
	: inframedWidget(parent), globalSettings(go), mainLayout(new QVBoxLayout(this)),
	userInfo(new QLabel(this)), listHeaderLayout(new QHBoxLayout(this)),
	previousButton(new MegaIconButton(this)), indexationInfo(new QLabel(this)),
	nextButton(new MegaIconButton(this)), itemInfoStorage(new QListWidget(this)),
	footerLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	saveButton(new MegaIconButton(this)), items(), pagenumber(0), awaiter(go.timeoutInt, this)
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
	footerLayout->addWidget(saveButton);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);

	QSizePolicy mi(QSizePolicy::Maximum, QSizePolicy::Maximum);
	QSizePolicy ma(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
	QFont scf = makeFont(0.04);

	previousButton->setSizePolicy(mi);
	previousButton->setIcon(QIcon(":/res/prevpage.png"));
	previousButton->setDisabled(true);
	previousButton->setStyleSheet(NAVIGATE_BUTTONS_STYLESHEET);
	previousButton->setMinimumWidth(calculateAdaptiveWidth(0.2));

	nextButton->setSizePolicy(mi);
	nextButton->setIcon(QIcon(":/res/nextpage.png"));
	nextButton->setDisabled(true);
	nextButton->setStyleSheet(NAVIGATE_BUTTONS_STYLESHEET);
	nextButton->setMinimumWidth(calculateAdaptiveWidth(0.2));

	indexationInfo->setSizePolicy(ma);
	indexationInfo->setFont(scf);
	indexationInfo->setAlignment(Qt::AlignCenter);

	//TODO: icons

	backButton->setText(tr("doc_results_back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	saveButton->setText(tr("doc_results_text"));
	saveButton->setIcon(QIcon(":/res/netUpload.png"));
	saveButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);

	userInfo->setText(tr("doc_results_userinfo"));
	userInfo->setFont(scf);
	userInfo->setAlignment(Qt::AlignCenter);

	itemInfoStorage->setFont(scf);
#ifdef QT_VERSION5X

	QObject::connect(backButton, &MegaIconButton::clicked, this, &DocResultsWidget::backRequired);
	QObject::connect(saveButton, &MegaIconButton::clicked, this, &DocResultsWidget::saveDocument);
	QObject::connect(nextButton, &MegaIconButton::clicked, this, &DocResultsWidget::nextPage);
	QObject::connect(previousButton, &MegaIconButton::clicked, this, &DocResultsWidget::previousPage);
	QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &DocResultsWidget::was_timeout);
#else
    QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
    QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(saveDocument()));
    QObject::connect(nextButton, SIGNAL(clicked()), this, SLOT(nextPage()));
    QObject::connect(previousButton, SIGNAL(clicked()), this, SLOT(previousPage()));
#endif

}

void DocResultsWidget::loadItems()
{
	if (awaiter.isAwaiting())
		return;
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &DocResultsWidget::items_response);
	globalSettings.networkingEngine->docGetResults(pagenumber, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();

	
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
	previousButton->setDisabled(pagenumber == 0);
	saveButton->setDisabled(items.values.isEmpty());
}

void DocResultsWidget::show()
{
	pagenumber = 0;
	loadItems();
	inframedWidget::show();
}

void DocResultsWidget::previousPage()
{
	if (awaiter.isAwaiting())
		return;
	if (pagenumber == 0)
		return;
	--pagenumber;
	loadItems();
}

void DocResultsWidget::nextPage()
{
	if (awaiter.isAwaiting())
		return;
	if (items.last)
		return;
	++pagenumber;
	loadItems();
}

void DocResultsWidget::saveDocument()
{
	if (awaiter.isAwaiting())
		return;
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &DocResultsWidget::save_response);
	globalSettings.networkingEngine->docUnlock(true, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();

}

void DocResultsWidget::items_response()
{
	items = RequestParser::interpretAsListedDocument(awaiter.restext, awaiter.errtext);
	refresh();
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(items_response()));
}

void DocResultsWidget::save_response()
{
	if (awaiter.restext.contains("_"))
		emit documentSaved();
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(save_response()));
}

void DocResultsWidget::was_timeout()
{
	userInfo->setText(tr("doc_results_timeout") + QString::number(globalSettings.timeoutInt));
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(save_response()));
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(items_response()));
}
