#include "DocResultsWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "widgets/ExtendedDelegates/ZebraListItemDelegate.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif

void DocResultsWidget::_handleRecord(RecEntity)
{
}

void DocResultsWidget::setIndexation(XmlObjects& settings)
{
	if (settings.isEmpty())
		userInfo->setText("No indexation in data");
	else
	{
		indexationInfo->setText(
			settings.first()->value("from")
			+ " -- " +
			settings.first()->value("to")
		);
		bool ok = settings.first()->value("last") == "true";
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
		nextButton->setDisabled(ok);
		previousButton->setDisabled(pagenumber >= 0);
	}
}

DocResultsWidget::DocResultsWidget( QWidget* parent)
	: IndependentBranchNode(independent_nodes::DocResults, true, parent), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	toolPanel(new QHBoxLayout(innerWidget)), deleteAllButton(new MegaIconButton(innerWidget)),
	deleteSelectedButton(new MegaIconButton(innerWidget)),
	userInfo(new QLabel(innerWidget)), listHeaderLayout(new QHBoxLayout(innerWidget)),
	previousButton(new MegaIconButton(innerWidget)), indexationInfo(new QLabel(innerWidget)),
	nextButton(new MegaIconButton(innerWidget)), itemInfoStorage(new QListView(innerWidget)),
	footerLayout(new QHBoxLayout(innerWidget)), backButton(new MegaIconButton(innerWidget)),
	saveButton(new MegaIconButton(innerWidget)), items(new DataEntityListModel(this)), pagenumber(0), 
	awaiter(new RequestAwaiter(AppSettings->timeoutInt + 20000, this))
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

	itemInfoStorage->setModel(items);
	itemInfoStorage->setItemDelegate(new ZebraItemDelegate(this));
#ifdef QT_VERSION5X
	QObject::connect(deleteAllButton, &MegaIconButton::clicked, this, &DocResultsWidget::deleteAll);
	QObject::connect(deleteSelectedButton, &MegaIconButton::clicked, this, &DocResultsWidget::deleteCurrent);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &DocResultsWidget::backRequired);
	QObject::connect(saveButton, &MegaIconButton::clicked, this, &DocResultsWidget::saveDocument);
	QObject::connect(nextButton, &MegaIconButton::clicked, this, &DocResultsWidget::nextPage);
	QObject::connect(previousButton, &MegaIconButton::clicked, this, &DocResultsWidget::previousPage);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &DocResultsWidget::was_timeout);
#else
    QObject::connect(deleteAllButton, SIGNAL(clicked()), this, SLOT(deleteAll()));
    QObject::connect(deleteSelectedButton, SIGNAL(clicked()), this, SLOT(deleteCurrent()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(saveDocument()));
	QObject::connect(nextButton, SIGNAL(clicked()), this, SLOT(nextPage()));
	QObject::connect(previousButton, SIGNAL(clicked()), this, SLOT(previousPage()));
    QObject::connect(awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
#endif
}

void DocResultsWidget::loadItems()
{
	if (awaiter->isAwaiting())
		return;
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &DocResultsWidget::items_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(items_response()));
#endif
	if (localCache.contains(documentGetResults))
	{
		AppNetwork->execQueryByTemplate(localCache[documentGetResults], QString::number(pagenumber), awaiter);
	}
	else
		FullItemEntity::sendGetRequest(pagenumber, awaiter);
	deleteAllButton->hide();
	deleteSelectedButton->hide();
	showProcessingOverlay();
}

void DocResultsWidget::refresh()
{
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	PolyResponse response = RequestParser::parseResponse(parser, RecEntity(new FullItemEntity()));
	if (response.isError)
	{
		userInfo->setText(response.errtext);
	}
	else
	{
		setIndexation(response.additionalObjects);
		saveButton->setDisabled(response.objects.isEmpty());
		items->setData(response.objects);
	}
}



void DocResultsWidget::previousPage()
{
	if (awaiter->isAwaiting())
		return;
	if (pagenumber == 0)
		return;
	--pagenumber;
	loadItems();
}

void DocResultsWidget::nextPage()
{
	if (awaiter->isAwaiting())
		return;
	++pagenumber;
	loadItems();
}

void DocResultsWidget::saveDocument()
{
	if (awaiter->isAwaiting())
		return;
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &DocResultsWidget::save_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(save_response()));
#endif
	if (localCache.contains(unlockDocument))
		AppNetwork->execQueryByTemplate(localCache[unlockDocument],"true", awaiter);
	else
		AppNetwork->execQueryByTemplate(QueryTemplates::unlockDocument, "true", awaiter);
	showProcessingOverlay();
}

void DocResultsWidget::items_response()
{
	refresh();
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void DocResultsWidget::save_response()
{
	if (awaiter->restext.contains("_"))
		emit done(RecEntity());
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void DocResultsWidget::was_timeout()
{
	userInfo->setText(tr("doc_results_timeout") + QString::number(AppSettings->timeoutInt));
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void DocResultsWidget::deleteAll()
{
	if (awaiter->isAwaiting())
		return;
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &DocResultsWidget::items_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(items_response()));
#endif
	if (localCache.contains(documentDeleteAll))
		AppNetwork->execQueryByTemplate(localCache[documentDeleteAll], awaiter);
	else
		AppNetwork->execQueryByTemplate(QueryTemplates::documentDeleteAll, awaiter);
	deleteAllButton->hide();
	deleteSelectedButton->hide();
	showProcessingOverlay();
}

void DocResultsWidget::deleteCurrent()
{
	if (awaiter->isAwaiting())
		return;
	if (!itemInfoStorage->currentIndex().isValid())
		return;
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &DocResultsWidget::items_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(items_response()));
#endif
	FullItem item = upcastRecord<FullItemEntity>(
		itemInfoStorage->currentIndex().data(
			DataEntityListModel::DirectAccess
		).value<RecEntity>());
	if (item.isNull())
		return;
	if (localCache.contains(docDeleteByBarcode))
		item->sendDeleteThisRequest(localCache[docDeleteByBarcode], awaiter);
	else
		item->sendDeleteThisRequest(awaiter);
	deleteAllButton->hide();
	deleteSelectedButton->hide();
	showProcessingOverlay();
}

void DocResultsWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	using namespace QueryTemplates;
	switch (
		((overloads.count()>4) ? 4 : overloads.count())
		)
	{
	case 4:
		localCache.insert(documentDeleteAll, overloads.at(3).assertedAndMappedCopy(
		documentDeleteAll));
	case 3:
    {
        QStringList t;
        t << "code" << "qty";
		localCache.insert(docDeleteByBarcode, overloads.at(2).assertedAndMappedCopy(
            docDeleteByBarcode,t,t
		));
    }
	case 2:
    {
        QStringList t;
        t << "save";
        localCache.insert(unlockDocument, overloads.at(1).assertedAndMappedCopy(
            unlockDocument, t,t
		));
    }
	case 1:
    {
        QStringList t;
        t << "page" << "opts";
        localCache.insert(documentGetResults, overloads.at(1).assertedAndMappedCopy(
            documentGetResults,t,t
		));
    }
	default:
		break;
	}
}

void DocResultsWidget::_sendDataRequest()
{
	loadItems();
}
