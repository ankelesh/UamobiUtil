#include "DocResultsWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"

#ifdef DEBUG
#include "debugtrace.h"
#endif

DocResultsWidget::DocResultsWidget(GlobalAppSettings& go, QWidget* parent)
	: inframedWidget(true, parent), globalSettings(go), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	toolPanel(new QHBoxLayout(innerWidget)), deleteAllButton(new MegaIconButton(innerWidget)),
	deleteSelectedButton(new MegaIconButton(innerWidget)),
	userInfo(new QLabel(innerWidget)), listHeaderLayout(new QHBoxLayout(innerWidget)),
	previousButton(new MegaIconButton(innerWidget)), indexationInfo(new QLabel(innerWidget)),
	nextButton(new MegaIconButton(innerWidget)), itemInfoStorage(new QListWidget(innerWidget)),
	footerLayout(new QHBoxLayout(innerWidget)), backButton(new MegaIconButton(innerWidget)),
	saveButton(new MegaIconButton(innerWidget)), items(), pagenumber(0), awaiter(go.timeoutInt + 20000, this)
{
	untouchable = innerWidget;
	main = this;
	current = innerWidget;
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(userInfo);
	innerLayout->addLayout(toolPanel);
	toolPanel->addWidget(deleteAllButton);
	toolPanel->addWidget(deleteSelectedButton);
	innerLayout->addLayout(listHeaderLayout);
	listHeaderLayout->addWidget(previousButton);
	listHeaderLayout->addWidget(indexationInfo);
	listHeaderLayout->addWidget(nextButton);
	innerLayout->addWidget(itemInfoStorage);
	innerLayout->addLayout(footerLayout);
	footerLayout->addWidget(backButton);
	footerLayout->addWidget(saveButton);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
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
    itemInfoStorage->setWordWrap(true);
    itemInfoStorage->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//TODO: icons

	deleteAllButton->setFont(scf);
	deleteAllButton->setText(tr("delete all"));
	deleteAllButton->setIcon(QIcon(":/res/data.png"));
	deleteAllButton->setStyleSheet(DELETE_BUTTONS_STYLESHEET);
	deleteAllButton->hide();
	
	deleteSelectedButton->setFont(scf);
	deleteSelectedButton->setText(tr("delete selected"));
	deleteSelectedButton->setIcon(QIcon(":/res/deleteData.png"));
	deleteSelectedButton->setStyleSheet(CANCEL_BUTTONS_STYLESHEET);
	deleteSelectedButton->hide();

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
	QObject::connect(deleteAllButton, &MegaIconButton::clicked, this, &DocResultsWidget::deleteAll);
	QObject::connect(deleteSelectedButton, &MegaIconButton::clicked, this, &DocResultsWidget::deleteCurrent);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &DocResultsWidget::backRequired);
	QObject::connect(saveButton, &MegaIconButton::clicked, this, &DocResultsWidget::saveDocument);
	QObject::connect(nextButton, &MegaIconButton::clicked, this, &DocResultsWidget::nextPage);
	QObject::connect(previousButton, &MegaIconButton::clicked, this, &DocResultsWidget::previousPage);
	QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &DocResultsWidget::was_timeout);
#else
    QObject::connect(deleteAllButton, SIGNAL(clicked()), this, SLOT(deleteAll()));
    QObject::connect(deleteSelectedButton, SIGNAL(clicked()), this, SLOT(deleteCurrent()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(saveDocument()));
	QObject::connect(nextButton, SIGNAL(clicked()), this, SLOT(nextPage()));
	QObject::connect(previousButton, SIGNAL(clicked()), this, SLOT(previousPage()));
	QObject::connect(&awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
#endif
}

void DocResultsWidget::loadItems()
{
	if (awaiter.isAwaiting())
		return;
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &DocResultsWidget::items_response);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(items_response()));
#endif
	globalSettings.networkingEngine->docGetResults(pagenumber, &awaiter, RECEIVER_SLOT_NAME);
	deleteAllButton->hide();
	deleteSelectedButton->hide();
	awaiter.run();
	showProcessingOverlay();
}

void DocResultsWidget::refresh()
{
	itemInfoStorage->clear();
	indexationInfo->setText(items.from + " -- " + items.to);
	QVector<parsedItem>::iterator start = items.values.begin();
	while (start != items.values.end())
	{

#ifdef DEBUG
		detrace_METHEXPL("inserting item: " << start->description());
#endif

		itemInfoStorage->addItem(start++->title);
	}
	nextButton->setDisabled(items.last);
	previousButton->setDisabled(pagenumber == 0);
	saveButton->setDisabled(items.values.isEmpty());
	bool ok = items.optionals == "true";
	if (ok)
	{
		deleteAllButton->show();
		deleteSelectedButton->show();
	}
	else
	{
		deleteAllButton->hide();
		deleteSelectedButton->hide();
	}
}

void DocResultsWidget::show()
{
	pagenumber = 0;
	loadItems();
	inframedWidget::show();
}

void DocResultsWidget::clear()
{
	itemInfoStorage->clear();
	indexationInfo->setText(" -- ");
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
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &DocResultsWidget::save_response);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(save_response()));
#endif
	globalSettings.networkingEngine->docUnlock(true, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	showProcessingOverlay();
}

void DocResultsWidget::items_response()
{
	items = RequestParser::interpretAsListedDocument(awaiter.restext, awaiter.errtext);
	
	refresh();
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void DocResultsWidget::save_response()
{
	if (awaiter.restext.contains("_"))
		emit documentSaved();
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void DocResultsWidget::was_timeout()
{
	userInfo->setText(tr("doc_results_timeout") + QString::number(globalSettings.timeoutInt));
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void DocResultsWidget::deleteAll()
{
	if (awaiter.isAwaiting())
		return;
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &DocResultsWidget::items_response);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(items_response()));
#endif
	globalSettings.networkingEngine->docDeleteAll(&awaiter, RECEIVER_SLOT_NAME);
	deleteAllButton->hide();
	deleteSelectedButton->hide();
	awaiter.run();
	showProcessingOverlay();
}

void DocResultsWidget::deleteCurrent()
{
	if (awaiter.isAwaiting())
		return;
	if (itemInfoStorage->currentItem() == Q_NULLPTR)
		return;
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &DocResultsWidget::items_response);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(items_response()));
#endif
	globalSettings.networkingEngine->docDeleteByBarcode(items.values.at(itemInfoStorage->currentRow()).code, 
		"&qty=" + QString::number(items.values.at(itemInfoStorage->currentRow()).qty), &awaiter, RECEIVER_SLOT_NAME);
	deleteAllButton->hide();
	deleteSelectedButton->hide();
	awaiter.run();
	showProcessingOverlay();
}
