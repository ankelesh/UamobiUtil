#include "PagedSearchWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "networking/Parsers/RequestParser.h"
#include "widgets/ExtendedDelegates/ZebraListItemDelegate.h"
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
#if defined(QT_VERSION5X) && defined(Q_OS_ANDROID)
#include <QScroller>
#include <QApplication>
#include <QInputEvent>
#endif

void PagedSearchWidget::_handleRecord(RecEntity)
{
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
		nextButton->setDisabled(settings.first()->value("last") == "true");
		previousButton->setDisabled(currentpage == 0);
	}
}

PagedSearchWidget::PagedSearchWidget(RecEntity proto, QWidget* parent)
	: IndependentBranchNode(independent_nodes::PagedSearch, parent),
	entityModel(new DataEntityListModel(this)),
	prototype(proto), mainLayout(new QVBoxLayout(this)),
	searchPanel(new QHBoxLayout(this)), searchInput(new QLineEdit(this)),
	searchButton(new MegaIconButton(this)), listHeaderLayout(new QHBoxLayout(this)),
	previousButton(new MegaIconButton(this)), nextButton(new MegaIconButton(this)),
	indexationInfo(new QLabel(this)), itemList(new QListView(this)),
	footerLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	currentpage(0), toSearch(), awaiter(new RequestAwaiter(AppSettings->timeoutInt, this)),
	loadDataQuery(-2, ping)
{
#ifdef DEBUG
	detrace_DCONSTR("PagedSearchWidget");
#endif
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
    mainLayout->setContentsMargins(0,0,0, 0);
	mainLayout->setSpacing(0);

	searchButton->setText(tr("item_search_search"));
	searchButton->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);
	searchButton->setIcon(QIcon(":/resources/search"));
	searchButton->setMinimumWidth(calculateAdaptiveWidth(0.2));
	searchButton->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum));

	indexationInfo->setText("0 " + QString(QChar(0x2014)) + " 0");
	indexationInfo->setFont(GENERAL_FONT);
	indexationInfo->setAlignment(Qt::AlignCenter);

	backButton->setText(tr("item_search_back"));
	backButton->setIcon(QIcon(":/resources/back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	nextButton->setIcon(QIcon(":/res/nextpage.png"));
	nextButton->setStyleSheet(NAVIGATE_BUTTONS_STYLESHEET);
	nextButton->setDisabled(true);

	previousButton->setIcon(QIcon(":/res/prevpage.png"));
	previousButton->setStyleSheet(NAVIGATE_BUTTONS_STYLESHEET);
	previousButton->setDisabled(true);

	searchInput->setFont(GENERAL_FONT);
	searchInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	searchInput->setMinimumWidth(calculateAdaptiveWidth(0.6));
	searchInput->setMaximumWidth(calculateAdaptiveWidth(0.8));
	searchInput->setMaximumHeight(calculateAdaptiveHeight());

	itemList->setFont(GENERAL_FONT);

	itemList->setModel(entityModel);
	itemList->setItemDelegate(new ZebraItemDelegate(this));
#if defined(QT_VERSION5X) && defined(Q_OS_ANDROID)
	QScroller::grabGesture(itemList, QScroller::LeftMouseButtonGesture);
	itemList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
#endif
#ifdef QT_VERSION5X

	QObject::connect(searchButton, &MegaIconButton::clicked, this, &PagedSearchWidget::doSearch);
	QObject::connect(nextButton, &MegaIconButton::clicked, this, &PagedSearchWidget::nextPage);
	QObject::connect(previousButton, &MegaIconButton::clicked, this, &PagedSearchWidget::previousPage); 
#ifdef Q_OS_ANDROID
		QObject::connect(itemList, &QListView::doubleClicked, entityModel, &DataEntityListModel::mapClickToEntity);
#else
	QObject::connect(itemList, &QListView::clicked, entityModel, &DataEntityListModel::mapClickToEntity);
#endif
	QObject::connect(entityModel, &DataEntityListModel::dataEntityClicked, this, &PagedSearchWidget::done);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &PagedSearchWidget::backRequired);
	QObject::connect(searchInput, &QLineEdit::returnPressed, this, &PagedSearchWidget::doSearch);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &PagedSearchWidget::was_timeout);
#else
	QObject::connect(searchButton, SIGNAL(clicked()), this, SLOT(doSearch()));
	QObject::connect(nextButton, SIGNAL(clicked()), this, SLOT(nextPage()));
	QObject::connect(previousButton, SIGNAL(clicked()), this, SLOT(previousPage()));
	QObject::connect(entityModel, SIGNAL(dataEntityClicked(RecEntity)), this, SIGNAL(done(RecEntity)));
    QObject::connect(itemList, SIGNAL(clicked(QModelIndex)), entityModel, SLOT(mapClickToEntity(QModelIndex)));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(searchInput, SIGNAL(returnPressed()), this, SLOT(doSearch()));
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
#ifdef DEBUG
			detrace_NRESPERR(response.errtext);
#endif
	}
	else
	{
		setIndexation(response.additionalObjects);
        entityModel->insertData(response.objects);
		itemList->setCurrentIndex(entityModel->index(0));
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
	if (loadDataQuery.isDefault())
    {
        QStringList t;
        t << toSearch << QString::number(currentpage);
        prototype->sendAssociatedGetRequest(t, awaiter);
    }
	else
		AppNetwork->execQueryByTemplate(loadDataQuery, toSearch, QString::number(currentpage), awaiter);
	showProcessingOverlay();
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
    QObject::disconnect(awaiter, SIGNAL(requestReceived()),Q_NULLPTR, Q_NULLPTR);
	hideProcessingOverlay();
}

void PagedSearchWidget::was_timeout()
{
	indexationInfo->setText(tr("item_search_timeout: ") + QString::number(awaiter->getInterval()));
    QObject::disconnect(awaiter, SIGNAL(requestReceived()),Q_NULLPTR, Q_NULLPTR);
	hideProcessingOverlay();
}

void PagedSearchWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	if (overloads.isEmpty())
		return;
    QStringList t;
    t << "text" << "page";
	loadDataQuery = overloads.first().assertedAndMappedCopy(
		documentSearchItems,
        t,t
	);
}

void PagedSearchWidget::focusInEvent(QFocusEvent* f)
{
	inframedWidget::focusInEvent(f);
	searchInput->setFocus();
#ifdef Q_OS_ANDROID
	qApp->inputMethod()->show();
#endif
}

void PagedSearchWidget::_numberReaction(int num)
{
	if (!searchInput->hasFocus())
		entityModel->mapClickToEntity(entityModel->index(num));
}

void PagedSearchWidget::_arrowReaction(int arrow)
{
	itemList->setCurrentIndex(entityModel->moveByArrow(arrow, itemList->currentIndex()));
}

void PagedSearchWidget::_returnReaction()
{
	if (!searchInput->hasFocus())
	{
		entityModel->mapClickToEntity(itemList->currentIndex());
	}
}

void PagedSearchWidget::setFocus()
{
	searchInput->setFocus();
}

void PagedSearchWidget::_sendDataRequest()
{
	loadResults();
}
