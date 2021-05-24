#include "ParentDocumentWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "networking/Parsers/RequestParser.h"
#include "widgets/ExtendedDelegates/ZebraListItemDelegate.h"
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
#if defined(Q_OS_ANDROID) && defined QT_VERSION5X
#include <QScroller>
#endif

ParentDocumentWidget::ParentDocumentWidget(RecEntity proto, QWidget* parent
	, IndependentBranchNode* filterSelection)
	: IndependentBranchNode(independent_nodes::ParentDocument, parent), abstractNode(),
	entityModel(new DataEntityListModel(this)),
	prototype(proto), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	userInfo(new QLabel(innerWidget)), filterButton(new MegaIconButton(innerWidget)),
	docSelection(new QListView(this)),
	footerLayout(new QHBoxLayout(innerWidget)), backButton(new MegaIconButton(innerWidget)),
	selectButton(new MegaIconButton(innerWidget)), filterSelect(filterSelection),
    loadDocumentQuery(-2, ping),awaiter( new RequestAwaiter(AppSettings->timeoutInt, this))
{

#ifdef DEBUG
	detrace_DCONSTR("ParentDocumentWidget");
#endif
	if (filterSelect == Q_NULLPTR)
		filterSelect = new FilterSelectWidget(this);
	else
		filterSelect->setParent(this);
	current = innerWidget;
	untouchable = innerWidget;
	main = this;
	this->setLayout(mainLayout);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	innerLayout->addWidget(userInfo);
	innerLayout->addWidget(filterButton);
	innerLayout->addWidget(docSelection);
	innerLayout->addLayout(footerLayout);
	footerLayout->setSpacing(0);
	footerLayout->setContentsMargins(0, 0, 0, 0);
	footerLayout->addWidget(backButton);
	footerLayout->addWidget(selectButton);

	mainLayout->addWidget(filterSelect);
	filterSelect->hide();
	userInfo->setText(tr("parent_doc_search_info"));
	userInfo->setFont(GENERAL_FONT);

	filterButton->setIcon(QIcon(":/res/filter.png"));
	filterButton->setText(tr("parent_doc_filter"));
	filterButton->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/resources/back"));
	selectButton->setText(tr("ok"));
	selectButton->setIcon(QIcon(":/resources/submit"));
	selectButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	docSelection->setModel(entityModel);
	docSelection->setItemDelegate(new ZebraItemDelegate(this));
#if defined(Q_OS_ANDROID) && defined(QT_VERSION5X)
	QScroller::grabGesture(docSelection, QScroller::TouchGesture);
#endif
	docSelection->setFont(AppFonts->makeFont(0.7));
	setTabOrder(docSelection, filterButton);
	filterButton->setDefault(true);
	backButton->setFocusPolicy(Qt::NoFocus);


#ifdef QT_VERSION5X
	QObject::connect(filterButton, &MegaIconButton::clicked, this, &ParentDocumentWidget::filterDocuments);
	QObject::connect(backButton, &QPushButton::clicked, this, &ParentDocumentWidget::backRequired);
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &ParentDocumentWidget::load_documents_response);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &ParentDocumentWidget::was_timeout);
#ifdef Q_OS_ANDROID
	QObject::connect(docSelection, &QListView::doubleClicked, entityModel, &DataEntityListModel::mapClickToEntity);
#else
	QObject::connect(docSelection, &QListView::clicked, entityModel, &DataEntityListModel::mapClickToEntity);
#endif
	QObject::connect(entityModel, &DataEntityListModel::dataEntityClicked, this, &IndependentBranchNode::done);
	QObject::connect(filterSelect, &IndependentBranchNode::backRequired, this, &ParentDocumentWidget::hideCurrent);
	QObject::connect(filterSelect, &IndependentBranchNode::done, this, &ParentDocumentWidget::filterReady);
	QObject::connect(selectButton, &MegaIconButton::clicked, this, &ParentDocumentWidget::onItemConfirmed);
#else
	QObject::connect(filterButton, SIGNAL(clicked()), this, SLOT(filterDocuments()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(load_documents_response()));
	QObject::connect(awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
	QObject::connect(entityModel, SIGNAL(dataEntityClicked(RecEntity)), this, SIGNAL(done(RecEntity)));
    QObject::connect(docSelection, SIGNAL(clicked(QModelIndex)), entityModel, SLOT(mapClickToEntity(QModelIndex)));
	QObject::connect(filterSelect, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
    QObject::connect(filterSelect, SIGNAL(done(RecEntity)), this, SLOT(filterReady()));
	QObject::connect(selectButton, SIGNAL(clicked()), this, SLOT(onItemConfirmed()));
#endif
}

void ParentDocumentWidget::setFocus()
{
	docSelection->setFocus();
}

void ParentDocumentWidget::loadDocuments()
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
	if (loadDocumentQuery.isDefault())
		prototype->sendAssociatedGetRequest(QStringList(), awaiter);
	else
		AppNetwork->execQueryByTemplate(loadDocumentQuery, awaiter);
}

void ParentDocumentWidget::filterDocuments()
{
	_hideAny(filterSelect);
	filterSelect->loadData();
}

void ParentDocumentWidget::load_documents_response()
{
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	PolyResponse response(RequestParser::parseResponse(parser, prototype));
	if (response.isError)
	{
		userInfo->setText(response.errtext);
#ifdef DEBUG
		detrace_NRESPERR(response.errtext);
#endif
	}
	else
	{
		if (response.objects.isEmpty())
			userInfo->setText(tr("no data received during request"));
        entityModel->insertData(response.objects);
		docSelection->setCurrentIndex(entityModel->index(0));
	}
	hideProcessingOverlay();
}

void ParentDocumentWidget::was_timeout()
{
	userInfo->setText("timeout");
	hideProcessingOverlay();
}

void ParentDocumentWidget::hideCurrent()
{
	_hideCurrent(innerWidget);
}

void ParentDocumentWidget::filterReady()
{
	_hideCurrent(innerWidget);
	loadDocuments();
}

void ParentDocumentWidget::onItemConfirmed()
{
	if (docSelection->currentIndex().isValid())
	{
		emit done(entityModel->getItemDirectly(docSelection->currentIndex()));
	}
}

void ParentDocumentWidget::_handleRecord(RecEntity)
{
}

void ParentDocumentWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	if (overloads.isEmpty())
		return;
	else
		loadDocumentQuery = overloads.first().assertedAndMappedCopy(inventoryListParentDocs);
}

void ParentDocumentWidget::_returnReaction()
{
	entityModel->mapClickToEntity(docSelection->currentIndex());
}

void ParentDocumentWidget::_arrowReaction(int arrow)
{
	if (!docSelection->hasFocus())
	{
		docSelection->setCurrentIndex(entityModel->moveByArrow(arrow, docSelection->currentIndex()));
	}
}

void ParentDocumentWidget::_sendDataRequest()
{
	loadDocuments();
}
