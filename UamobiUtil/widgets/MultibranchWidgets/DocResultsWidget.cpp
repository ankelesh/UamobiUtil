#include "DocResultsWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "widgets/ExtendedDelegates/ZebraListItemDelegate.h"
#include <QMessageBox>
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
		if (settings.count() > 1 && settings.at(1)->hasField("allowdelete"))
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
		if (settings.first()->value("from") == "0")
		{
			pagenumber = 0;
		}
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
	saveButton(new MegaIconButton(innerWidget)), quitButton(new MegaIconButton(innerWidget)),
	items(new DataEntityListModel(this)), pagenumber(0), 
	awaiter(new RequestAwaiter(AppSettings->timeoutInt + 20000, this)), localCache(),
	attachedControls(new ControlListWidget(this))
{
#ifdef DEBUG
	detrace_DCONSTR("DocResults");
#endif
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
	footerLayout->addWidget(quitButton);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
	mainLayout->addWidget(attachedControls);
	attachedControls->hide();

	QSizePolicy mi(QSizePolicy::Maximum, QSizePolicy::Maximum);
	QSizePolicy ma(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

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
	indexationInfo->setFont(GENERAL_FONT);
	indexationInfo->setAlignment(Qt::AlignCenter);
    itemInfoStorage->setWordWrap(true);
    itemInfoStorage->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//TODO: icons

	deleteAllButton->setFont(GENERAL_FONT);
	deleteAllButton->setText(tr("delete all"));
	deleteAllButton->setIcon(QIcon(":/res/data.png"));
	deleteAllButton->setStyleSheet(DELETE_BUTTONS_STYLESHEET);
	deleteAllButton->hide();
	
	deleteSelectedButton->setFont(GENERAL_FONT);
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


	quitButton->setText(tr("quit"));
	quitButton->setIcon(QIcon(":/res/data.png"));
	quitButton->setStyleSheet(CANCEL_BUTTONS_STYLESHEET);

	userInfo->setText(tr("doc_results_userinfo"));
	userInfo->setFont(GENERAL_FONT);
	userInfo->setAlignment(Qt::AlignCenter);

	itemInfoStorage->setFont(GENERAL_FONT);

	itemInfoStorage->setModel(items);
	itemInfoStorage->setItemDelegate(new ZebraItemDelegate(this));
#ifdef QT_VERSION5X
	QObject::connect(deleteAllButton, &MegaIconButton::clicked, this, &DocResultsWidget::deleteAll);
	QObject::connect(deleteSelectedButton, &MegaIconButton::clicked, this, &DocResultsWidget::handleDelete);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &DocResultsWidget::backRequired);
	QObject::connect(saveButton, &MegaIconButton::clicked, this, &DocResultsWidget::saveDocument);
	QObject::connect(nextButton, &MegaIconButton::clicked, this, &DocResultsWidget::nextPage);
	QObject::connect(quitButton, &MegaIconButton::clicked, this, &DocResultsWidget::saveDocument);
	QObject::connect(attachedControls, &ControlListWidget::controlsConfirmed, this, &DocResultsWidget::attachedControlsDone);
	QObject::connect(attachedControls, &ControlListWidget::backRequired, this, &DocResultsWidget::hideCurrent);
	QObject::connect(previousButton, &MegaIconButton::clicked, this, &DocResultsWidget::previousPage);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &DocResultsWidget::was_timeout);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &DocResultsWidget::items_response);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &DocResultsWidget::save_response);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &DocResultsWidget::get_attached_response);
#else
    QObject::connect(deleteAllButton, SIGNAL(clicked()), this, SLOT(deleteAll()));
    QObject::connect(deleteSelectedButton, SIGNAL(clicked()), this, SLOT(handleDelete()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(saveDocument()));
	QObject::connect(nextButton, SIGNAL(clicked()), this, SLOT(nextPage()));
	QObject::connect(attachedControls, SIGNAL(controlsConfirmed()), this, SLOT(attachedControlsDone()));
	QObject::connect(attachedControls, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(previousButton, SIGNAL(clicked()), this, SLOT(previousPage()));
    QObject::connect(awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(items_response()));
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(save_response()));
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(get_attached_response()));
#endif
}

void DocResultsWidget::loadItems()
{
	if (awaiter->isAwaiting())
		return;

	if (localCache.contains(documentGetResults))
	{
		AppNetwork->execQueryByTemplate(localCache[documentGetResults], QString::number(pagenumber), awaiter);
	}
	else
		FullItemEntity::sendGetRequest(pagenumber, awaiter);
	awaiter->deliverResultTo(documentGetResults);
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
		QMessageBox::critical(this, tr("Error!"), response.errtext, QMessageBox::Ok);
#ifdef DEBUG
		detrace_NRESPERR(response.errtext);
#endif
	}
	else
	{
		PolyResponse invoices = RequestParser::parseResponse(response, RecEntity(new InvoiceEntity()));
		setIndexation(invoices.additionalObjects);
		saveButton->setDisabled(response.objects.isEmpty());
        items->insertData(invoices.objects + response.objects);
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
	if (sender() == quitButton)
	{
		QMessageBox::StandardButton response = QMessageBox::question(this, tr("QuitWithoutSave?"), 
			tr("quit_without_save_info?"),
			QMessageBox::Ok | QMessageBox::Cancel);
		if (response == QMessageBox::Ok)
			emit done(RecEntity());
		else
			return;
	}
	if (awaiter->isAwaiting())
		return;

	if (localCache.contains(unlockDocument))
		AppNetwork->execQueryByTemplate(localCache[unlockDocument],"true", awaiter);
	else
		AppNetwork->execQueryByTemplate(QueryTemplates::unlockDocument, "true", awaiter);
	awaiter->deliverResultTo(unlockDocument);
	showProcessingOverlay();
}

void DocResultsWidget::getAttachedControls()
{
	if (awaiter->isAwaiting())
		return;
	if (!itemInfoStorage->currentIndex().isValid())
		return;
	RecEntity currentItem = itemInfoStorage->currentIndex().data(DataEntityListModel::DirectAccess
	).value<RecEntity>();
	if (!localCache.contains(documentResultGetBox))
	{
		attachedControls->clearControls();
		attachedControls->emplaceControl(
			InputControl(
				new InputControlEntity(
					"qty", "Int", "0")));
		_hideAny(attachedControls);
	}
	else
	{
		QStringList temp;
		temp << currentItem->getId() << currentItem->getTitle() << QString::number(currentItem->getAttachedNumber());
		AppNetwork->execQueryByTemplate(localCache[documentResultGetBox],3, temp, awaiter);
		awaiter->deliverResultTo(documentResultGetBox);
		showProcessingOverlay();
	}
}

void DocResultsWidget::items_response()
{
	if (awaiter->deliverHere(unlockDocument))
		return;
	refresh();
	hideProcessingOverlay();
}

void DocResultsWidget::save_response()
{
	if (!awaiter->deliverHere(unlockDocument))
		return;
	if (awaiter->restext.startsWith('_'))
	{
		if (awaiter->restext.count() > 1)
		{
#ifdef DEBUG
			debugtrace::getObject() << detr_supply::netErrorPossible <<
				" debug info came from server during document save operation |"
				<< awaiter->restext << '\n';
#endif
		}
		emit done(RecEntity());
	}
	QDomDocument doc;
	doc.setContent(awaiter->restext);
	if (isError(doc))
	{
		userInfo->setText(makeError(doc));
	}
	hideProcessingOverlay();
}

void DocResultsWidget::get_attached_response()
{
	if (!awaiter->deliverHere(documentResultGetBox))
		return;
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	NetRequestResponse<InputControlEntity> response 
		= RequestParser::parseResponse<InputControlEntity>(parser);
	if (response.isError)
	{
		QMessageBox::critical(this, tr("Error!"), response.errtext, QMessageBox::Ok);
#ifdef DEBUG
		detrace_NRESPERR(response.errtext);
#endif
	}
	else
	{
		attachedControls->clearControls();
		attachedControls->useControls(response.objects);
		_hideAny(attachedControls);
	}
	hideProcessingOverlay();
}

void DocResultsWidget::attachedControlsDone()
{
	if (awaiter->isAwaiting())
		return;
	if (!itemInfoStorage->currentIndex().isValid())
	{
		_hideAny(innerWidget);
		return;
	}
	RecEntity currentItem = itemInfoStorage->currentIndex().data(DataEntityListModel::DirectAccess
	).value<RecEntity>();
	if (!localCache.contains(docDeleteByBarcode))
	{
		AppNetwork->execQueryByTemplate(docDeleteByBarcode, currentItem->getId(), attachedControls->mappedControls(), awaiter);
	}
	else
	{
		AppNetwork->execQueryByTemplate(localCache[docDeleteByBarcode],
			currentItem->getId(), attachedControls->mappedControls(), awaiter);
	}
	_hideAny(innerWidget);
	hideProcessingOverlay();
}

void DocResultsWidget::was_timeout()
{
	userInfo->setText(tr("doc_results_timeout") + QString::number(AppSettings->timeoutInt));
	hideProcessingOverlay();
}

void DocResultsWidget::deleteAll()
{
	if (awaiter->isAwaiting())
		return;
	int result =  QMessageBox::question(this, tr("Delete all"), tr("Delete all data?"), QMessageBox::Ok, QMessageBox::Cancel);
	if (result != QMessageBox::Ok)
		return;
	if (localCache.contains(documentDeleteAll))
		AppNetwork->execQueryByTemplate(localCache[documentDeleteAll], awaiter);
	else
		AppNetwork->execQueryByTemplate(QueryTemplates::documentDeleteAll, awaiter);
	awaiter->deliverResultTo(documentDeleteAll);
	deleteAllButton->hide();
	deleteSelectedButton->hide();
	showProcessingOverlay();
}

void DocResultsWidget::handleDelete()
{
	getAttachedControls();
}



void DocResultsWidget::hideCurrent()
{
	_hideAny(innerWidget);
}

void DocResultsWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	using namespace QueryTemplates;
	switch (
		((overloads.count()>5) ? 5 : overloads.count())
		)
	{
	case 5:
		localCache.insert(documentDeleteAll, overloads.at(4).assertedAndMappedCopy(
		documentDeleteAll));
        Q_FALLTHROUGH();
	case 4:
		localCache.insert(documentResultGetBox, overloads.at(3));
        Q_FALLTHROUGH();
	case 3:
    {
        QStringList t;
        t << "code" << "qty";
		localCache.insert(docDeleteByBarcode, overloads.at(2).assertedAndMappedCopy(
            docDeleteByBarcode,t,t
		));
        Q_FALLTHROUGH();
    }
	case 2:
    {
        QStringList t;
        t << "save";
        localCache.insert(unlockDocument, overloads.at(1).assertedAndMappedCopy(
            unlockDocument, t,t
		));
        Q_FALLTHROUGH();
    }
	case 1:
    {
        QStringList t;
        t << "page" << "opts";
        localCache.insert(documentGetResults, overloads.at(1).assertedAndMappedCopy(
            documentGetResults,t,t
		));
        Q_FALLTHROUGH();
    }
	default:
		break;
	}
}

void DocResultsWidget::_sendDataRequest()
{
	loadItems();
}
