#include "DocResultsWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "widgets/ExtendedDelegates/ZebraListItemDelegate.h"
#include "widgets/ExtendedDelegates/CountingDelegate.h"
#include <QMessageBox>
#ifdef DEBUG
#include "debugtrace.h"
#endif
#if defined(Q_OS_ANDROID) and defined(QT_VERSION5X)
#include <qscroller.h>
#endif
#include "widgets/ElementWidgets/ExtendedDialogs.h"
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
		previousButton->setDisabled(pagenumber <= 0);
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
	saveButton(new MegaIconButton(innerWidget)), 
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
#if defined(Q_OS_WINCE) || defined(Q_OS_ANDROID)
	previousButton->setMaximumHeight(calculateAdaptiveButtonHeight(0.08));
#endif

	nextButton->setSizePolicy(mi);
	nextButton->setIcon(QIcon(":/res/nextpage.png"));
	nextButton->setDisabled(true);
	nextButton->setStyleSheet(NAVIGATE_BUTTONS_STYLESHEET);
	nextButton->setMinimumWidth(calculateAdaptiveWidth(0.2));
#if defined(Q_OS_WINCE) || defined(Q_OS_ANDROID)
	nextButton->setMaximumHeight(calculateAdaptiveButtonHeight(0.08));
#endif


	indexationInfo->setSizePolicy(ma);
	indexationInfo->setFont(GENERAL_FONT);
	indexationInfo->setAlignment(Qt::AlignCenter);
#if defined(Q_OS_WINCE) || defined(Q_OS_ANDROID)
	indexationInfo->setMaximumHeight(calculateAdaptiveButtonHeight(0.08));
#endif
    itemInfoStorage->setWordWrap(true);
    itemInfoStorage->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//TODO: icons

	deleteAllButton->setFont(GENERAL_FONT);
	deleteAllButton->setText(tr("delete all"));
	deleteAllButton->setIcon(QIcon(":/res/data.png"));
	deleteAllButton->setStyleSheet(DELETE_BUTTONS_STYLESHEET);
	deleteAllButton->setMaximumHeight(calculateAdaptiveButtonHeight(0.08));
	deleteAllButton->hide();
	
	deleteSelectedButton->setFont(GENERAL_FONT);
	deleteSelectedButton->setText(tr("delete selected"));
	deleteSelectedButton->setIcon(QIcon(":/res/deleteData.png"));
	deleteSelectedButton->setStyleSheet(CANCEL_BUTTONS_STYLESHEET);
	deleteSelectedButton->hide();
	deleteSelectedButton->setMaximumHeight(calculateAdaptiveButtonHeight(0.08));

	backButton->setText(tr("doc_results_back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	saveButton->setText(tr("doc_results_text"));
	saveButton->setIcon(QIcon(":/res/netUpload.png"));
	saveButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);



	userInfo->setText(tr("doc_results_userinfo"));
	userInfo->setFont(GENERAL_FONT);
	userInfo->setAlignment(Qt::AlignCenter);

	itemInfoStorage->setFont(AppFonts->makeCustomFont(0.03));
	itemInfoStorage->setModel(items);
	itemInfoStorage->setItemDelegate(new CountingItemDelegate(this));
#if defined(QT_VERSION5X) && defined(Q_OS_ANDROID)
	QScroller::grabGesture(itemInfoStorage, QScroller::TouchGesture);
	itemInfoStorage->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
#endif
#ifdef QT_VERSION5X
	QObject::connect(deleteAllButton, &MegaIconButton::clicked, this, &DocResultsWidget::deleteAll);
	QObject::connect(deleteSelectedButton, &MegaIconButton::clicked, this, &DocResultsWidget::handleDelete);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &DocResultsWidget::backRequired);
	QObject::connect(saveButton, &MegaIconButton::clicked, this, &DocResultsWidget::saveDocument);
	QObject::connect(nextButton, &MegaIconButton::clicked, this, &DocResultsWidget::nextPage);
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
	if (!assertAndShowError(this, parser, tr("Error!")))
	{
		PolyResponse invoices = RequestParser::parseResponse(response, RecEntity(new InvoiceEntity()));
		setIndexation(invoices.additionalObjects);
		saveButton->setDisabled(response.objects.isEmpty());
		QVector<int> heights;
		heights.reserve(invoices.objects.count() + response.objects.count());
		QVector<RecEntity> total;
		total.reserve(invoices.objects.count() + response.objects.count());
		int height  = 0;
		QFontMetrics fm = itemInfoStorage->fontMetrics();
		{
			QVector<RecEntity>::iterator pos = invoices.objects.begin();
			Invoice fitem;
			Invoice prototype(new InvoiceEntity());
			while (pos != invoices.objects.end())
			{
				fitem = upcastRecord(*pos, prototype);
				if (!fitem.isNull())
				{
					fitem->name = FontAdapter::breakStringToFitScreen(
						fitem->name.replace("\n", " "),fm,  0.75, &height);
					total << *pos;
					heights << height;
				}
				++pos;
			}
		}
		{
			QVector<RecEntity>::iterator pos = response.objects.begin();
			FullItem fitem;
			FullItem prototype(new FullItemEntity());
			while (pos != response.objects.end())
			{
				fitem = upcastRecord(*pos, prototype);
				if (!fitem.isNull())
				{
					fitem->title = FontAdapter::breakStringToFitScreen(
						fitem->title.replace("\n", " "),fm,  0.75, &height);
					total << *pos;
					heights << height;
				}
				++pos;
			}
		}
        items->insertData(total, heights);
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

	if (localCache.contains(unlockDocument))
	{
		if (localCache[unlockDocument].isNull())
		{
			emit done(RecEntity());
			return;
		}
		AppNetwork->execQueryByTemplate(localCache[unlockDocument], "true", awaiter);
	}
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
		FullItem castedItem = upcastRecord<FullItemEntity>(currentItem);
		int type;
		if (castedItem.isNull())
			type = InputControlEntity::Int;
		else
			type = castedItem->controlType;
		attachedControls->emplaceControl(
			InputControl(
				new InputControlEntity(
					currentItem->getTitle(), "Label", currentItem->getTitle())));
		attachedControls->emplaceControl(
			InputControl(
				new InputControlEntity(
					"qty", type, "0")));
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
	else
	{
		QDomDocument doc;
		doc.setContent(awaiter->restext);
		if (isError(doc))
		{
			QPair<QString, QString> edata = makeError(doc);
			ErrorMessageDialog::showErrorInfo(this, tr("Error!"), edata.first, false, edata.second);
		}
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
	if (!assertAndShowError(this, parser, tr("Error!")))
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
	int result =  QMessageBox::question(this, tr("Delete all"), tr("Delete all data?"), QMessageBox::Ok | QMessageBox::Cancel);
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
