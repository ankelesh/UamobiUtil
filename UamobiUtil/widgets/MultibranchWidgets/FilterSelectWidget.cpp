#include "FilterSelectWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "networking/Parsers/RequestParser.h"
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QScroller>
#else
 // Qt 4 only imports
#include "legacy/qtCompatibility/scrollgrabber.h"
#endif
#include "widgets/ExtendedDelegates/CheckableDelegate.h"

void FilterSelectWidget::_handleRecord(RecEntity)
{
	loadFilters();
}

FilterSelectWidget::FilterSelectWidget(QWidget* parent)
	: IndependentBranchNode(independent_nodes::FilterSelect, true, parent),
	doctypes(new DataEntityListModel(this)),
	mainLayout(new QVBoxLayout(this)),
	title(new QLabel(this)), topPanelLayout(new QHBoxLayout(this)),
	allonButton(new MegaIconButton(this)), alloffButton(new MegaIconButton(this)),
	typesel(new QListView(this)),
	footerLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this)), awaiter(new RequestAwaiter(AppSettings->timeoutInt, this))
{
	this->setLayout(mainLayout);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(title);
	mainLayout->addLayout(topPanelLayout);
	topPanelLayout->setContentsMargins(0, 0, 0, 0);
	topPanelLayout->setSpacing(0);
	topPanelLayout->addWidget(allonButton);
	topPanelLayout->addWidget(alloffButton);
	mainLayout->addWidget(typesel);
	typesel->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	mainLayout->addLayout(footerLayout);
	footerLayout->setContentsMargins(0, 0, 0, 0);
	footerLayout->setSpacing(0);
	footerLayout->addWidget(backButton);
	footerLayout->addWidget(okButton);
	QFont scf = makeFont(0.04);
	title->setFont(scf);
	title->setAlignment(Qt::AlignCenter);
	title->setText(tr("filter_widget_title"));
	allonButton->setText(tr("all_on_button"));
	allonButton->setIcon(QIcon(":/res/single.png"));
	allonButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);
	alloffButton->setStyleSheet(DELETE_BUTTONS_STYLESHEET);
	alloffButton->setIcon(QIcon(":/res/data.png"));
	alloffButton->setText(tr("all_off_button"));
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/res/back.png"));
	okButton->setText(tr("filter!"));
	okButton->setIcon(QIcon(":/res/filter.png"));
	okButton->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);
	typesel->setItemDelegate(new CheckableDelegate(QColor(210, 224, 146), QColor(245, 164, 188), this));
#ifdef QT_VERSION5X
	QObject::connect(allonButton, &MegaIconButton::clicked, this, &FilterSelectWidget::checkAll);
	QObject::connect(alloffButton, &MegaIconButton::clicked, this, &FilterSelectWidget::uncheckAll);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &FilterSelectWidget::backRequired);
	QObject::connect(okButton, &MegaIconButton::clicked, this, &FilterSelectWidget::okPressed);
	QObject::connect(typesel, &QListView::clicked, this, &FilterSelectWidget::changeState);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &FilterSelectWidget::was_timeout);

#else
	QObject::connect(allonButton, SIGNAL(clicked()), this, SLOT(checkAll()));
	QObject::connect(alloffButton, SIGNAL(clicked()), this, SLOT(uncheckAll()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
	QObject::connect(awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
	QObject::connect(typesel, SIGNAL(clicked(const QModelIndex&)), this, SLOT(changeState(const QModelIndex&)));
#endif
}

void FilterSelectWidget::loadFilters()
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &FilterSelectWidget::parse_doctype_list_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(parse_doctype_list_response()));
#endif
	DocTypeEntity::sendGetRequest(awaiter);
}

void FilterSelectWidget::checkAll()
{
	DocType doc(new DocTypeEntity());
	for (int i = 0; i < doctypes->rowCount(); ++i)
	{
		doc = upcastRecord(doctypes->data(
			doctypes->index(i),
			DataEntityListModel::DirectAccess).value<RecEntity>(), doc);
		if (doc.isNull())
			continue;
		doc->isSelected = true;
	}
	typesel->update();
}

void FilterSelectWidget::uncheckAll()
{
	DocType doc(new DocTypeEntity());
	for (int i = 0; i < doctypes->rowCount(); ++i)
	{
		doc = upcastRecord(doctypes->data(
			doctypes->index(i),
			DataEntityListModel::DirectAccess).value<RecEntity>(), doc);
		if (doc.isNull())
			continue;
		doc->isSelected = false;
	}
	typesel->update();
}

void FilterSelectWidget::changeState(const QModelIndex& index)
{
	if (index.isValid())
	{
		DocType doctype = upcastRecord<DocTypeEntity>(index.data().value<RecEntity>());
		if (doctype.isNull())
			return;
		else
		{
			doctype->isSelected = !doctype->isSelected;
			typesel->update(index);
		}
	}
}

void FilterSelectWidget::okPressed()
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &FilterSelectWidget::parse_doctype_selection_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(parse_doctype_selection_response()));
#endif
	QString buffer;
	RecEntity record;
	for (int i = 0; i < doctypes->rowCount(); ++i)
	{
		record = doctypes->data(
			doctypes->index(i),
			DataEntityListModel::DirectAccess
		).value<RecEntity>();
		if (record->getAttachedNumber() == 1)
		{
			buffer += QString::number(record->getId()) + ",";
		}
	}
	buffer.chop(1);
	DocTypeEntity::sendFilterList(buffer, awaiter);
}

void FilterSelectWidget::was_timeout()
{
	title->setText(tr("timeout:") + QString::number(awaiter->getInterval()));
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void FilterSelectWidget::parse_doctype_list_response()
{
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	PolyResponse response = RequestParser::parseResponse(parser, RecEntity(new DocTypeEntity()));
	if (response.isError)
	{
		title->setText(response.errtext);
	}
	else
	{
		doctypes->setData(response.objects);
	}
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void FilterSelectWidget::parse_doctype_selection_response()
{
	ResponseParser parser(new SimpliestResponceParser(awaiter->restext, awaiter->errtext));
	if (parser->isSuccessfull())
	{
		emit filterApplied();
	}
	else
	{
		title->setText(parser->getErrors());
	}
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void FilterSelectWidget::_sendDataRequest()
{
	loadFilters();
}
