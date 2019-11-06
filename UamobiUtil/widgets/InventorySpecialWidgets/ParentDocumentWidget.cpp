#include "ParentDocumentWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "networking/Parsers/RequestParser.h"
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

int specwidgets::documentSelectionWidget::countElems()
{
	return alldocs.count();
}

void specwidgets::documentSelectionWidget::itemSelectedFromList(QListWidgetItem* it)
{
	emit docSelected(alldocs.at(currentRow()));
}

QString specwidgets::documentSelectionWidget::elemToString(int index)
{
	return alldocs.at(index).title;
}

specwidgets::documentSelectionWidget::documentSelectionWidget(QVector<parsedDocument>& docs, QWidget* parent)
	: AbstractListSelectionWidget(parent), alldocs(docs)
{
	init();
}

ParentDocumentWidget::ParentDocumentWidget(GlobalAppSettings& go, QWidget* parent)
	: inframedWidget(parent), abstractNode(), alldocs(),
	globalSettings(go), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	userInfo(new QLabel(innerWidget)), filterButton(new MegaIconButton(innerWidget)),
	docSelection(new specwidgets::documentSelectionWidget(alldocs, innerWidget)),
	footerLayout(new QHBoxLayout(innerWidget)), backButton(new MegaIconButton(innerWidget)),
	selectButton(new MegaIconButton(innerWidget)), filterSelect(new FilterSelectWidget(go, this)),
	awaiter(go.timeoutInt, this)
{
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
	QFont scf = makeFont(0.04);
	userInfo->setText(tr("parent_doc_search_info"));
	userInfo->setFont(scf);

	filterButton->setIcon(QIcon(":/res/filter.png"));
	filterButton->setText(tr("parent_doc_filter"));
	filterButton->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/res/back.png"));
	selectButton->setText(tr("ok"));
	selectButton->setIcon(QIcon(":/res/submit.png"));
	selectButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
#ifdef QT_VERSION5X
	QObject::connect(filterButton, &MegaIconButton::clicked, this, &ParentDocumentWidget::filterDocuments);
	QObject::connect(backButton, &QPushButton::clicked, this, &ParentDocumentWidget::backRequired);
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &ParentDocumentWidget::load_documents_response);
	QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &ParentDocumentWidget::was_timeout);
	QObject::connect(docSelection, &specwidgets::documentSelectionWidget::docSelected, this, &ParentDocumentWidget::docSelected);
	QObject::connect(filterSelect, &FilterSelectWidget::backRequired, this, &ParentDocumentWidget::hideCurrent);
	QObject::connect(filterSelect, &FilterSelectWidget::filterApplied, this, &ParentDocumentWidget::filterReady);
#else
	QObject::connect(filterButton, SIGNAL(clicked()), this, SLOT(filterDocuments()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(load_documents_response()));
	QObject::connect(&awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
	QObject::connect(docSelection, SIGNAL(docSelected(parsedDocument)), this, SIGNAL(docSelected(parsedDocument)));
	QObject::connect(filterSelect, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(filterSelect, SIGNAL(filterApplied()), this, SLOT(filterReady()));
#endif
}

void ParentDocumentWidget::loadDocuments()
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
	globalSettings.networkingEngine->invGetParentDocsList(&awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
}

void ParentDocumentWidget::filterDocuments()
{
	_hideAny(filterSelect);
	filterSelect->loadFilters();
}

void ParentDocumentWidget::load_documents_response()
{
#ifdef DEBUG
	detrace_METHEXPL("got " << awaiter.restext);
#endif
	alldocs = RequestParser::interpretAsDocumentsList(awaiter.restext, awaiter.errtext);
	docSelection->reload();
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