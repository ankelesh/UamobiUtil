#include "FilterSelectWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "networking/Parsers/RequestParser.h"
// Qt 5 only imports
#include "widgets/ExtendedDelegates/CheckableDelegate.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif

void FilterSelectWidget::_handleRecord(RecEntity)
{

}

FilterSelectWidget::FilterSelectWidget(QWidget* parent)
	: IndependentBranchNode(independent_nodes::FilterSelect, parent),
	doctypes(new DataEntityListModel(this)),
	mainLayout(new QVBoxLayout(this)),
	title(new QLabel(this)), topPanelLayout(new QHBoxLayout(this)),
	allonButton(new MegaIconButton(this)), alloffButton(new MegaIconButton(this)),
	typesel(new QListView(this)),
	footerLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this)), awaiter(new RequestAwaiter(AppSettings->timeoutInt, this)),
	localCache()
{
#ifdef DEBUG
	detrace_DCONSTR("FilterSelectWidget");
#endif
	this->setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(title);
	mainLayout->addLayout(topPanelLayout);
    topPanelLayout->setContentsMargins(0,0,0,0);
	topPanelLayout->setSpacing(0);
	topPanelLayout->addWidget(allonButton);
	topPanelLayout->addWidget(alloffButton);
	mainLayout->addWidget(typesel);
	typesel->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	mainLayout->addLayout(footerLayout);
    footerLayout->setContentsMargins(0,0,0,0);
	footerLayout->setSpacing(0);
	footerLayout->addWidget(backButton);
	footerLayout->addWidget(okButton);
	title->setFont(GENERAL_FONT);
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
	typesel->setModel(doctypes);
	setTabOrder(typesel, okButton);
	okButton->setDefault(true);
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
    QObject::connect(typesel, SIGNAL(clicked(QModelIndex)), this, SLOT(changeState(QModelIndex)));
#endif
}

void FilterSelectWidget::setFocus()
{
	typesel->setFocus();
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
	if (localCache.contains(documentGetAllowedTypes))
		AppNetwork->execQueryByTemplate(localCache[documentGetAllowedTypes], awaiter);
	else
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
#ifdef QT_VERSION5X
void FilterSelectWidget::changeState(const QModelIndex & index)
{
	if (index.isValid())
	{
		DocType doctype = upcastRecord<DocTypeEntity>(
			index.data(DataEntityListModel::DirectAccess).value<RecEntity>());
		if (doctype.isNull())
			return;
		else
		{
			doctype->isSelected = !doctype->isSelected;
			typesel->update(index);
		}
	}
}
#else
void FilterSelectWidget::changeState(QModelIndex index)
{
    if (index.isValid())
    {
        DocType doctype = upcastRecord<DocTypeEntity>(
            index.data(DataEntityListModel::DirectAccess).value<RecEntity>());
        if (doctype.isNull())
            return;
        else
        {
            doctype->isSelected = !doctype->isSelected;
            typesel->update(index);
        }
    }
}
#endif

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
			buffer += record->getId() + ",";
		}
	}
	buffer.chop(1);
	if (localCache.contains(documentSetFilter))
		AppNetwork->execQueryByTemplate(localCache[documentSetFilter], buffer, awaiter);
	else
		DocTypeEntity::sendFilterList(buffer, awaiter);
}

void FilterSelectWidget::was_timeout()
{
	title->setText(tr("timeout:") + QString::number(awaiter->getInterval()));
    QObject::disconnect(awaiter, SIGNAL(requestReceived()), Q_NULLPTR, Q_NULLPTR);
	hideProcessingOverlay();
}

void FilterSelectWidget::parse_doctype_list_response()
{
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	PolyResponse response = RequestParser::parseResponse(parser, RecEntity(new DocTypeEntity()));
	if (response.isError)
	{
		title->setText(response.errtext); 
#ifdef DEBUG
			detrace_NRESPERR(response.errtext);
#endif
	}
	else
	{
        doctypes->insertData(response.objects);
		typesel->setCurrentIndex(doctypes->index(0));
	}
    QObject::disconnect(awaiter, SIGNAL(requestReceived()), Q_NULLPTR, Q_NULLPTR);
	hideProcessingOverlay();
}

void FilterSelectWidget::parse_doctype_selection_response()
{
	ResponseParser parser(new SimpliestResponceParser(awaiter->restext, awaiter->errtext));
	if (parser->isSuccessfull())
	{
		emit done(RecEntity());
	}
	else
	{
		title->setText(parser->getErrors()); 
#ifdef DEBUG
			detrace_NRESPERR(parser->getErrors());
#endif
	}
    QObject::disconnect(awaiter, SIGNAL(requestReceived()), Q_NULLPTR, Q_NULLPTR);
	hideProcessingOverlay();
}

void FilterSelectWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	switch (
		((overloads.count() > 2) ? 2 : overloads.count())
		)
	{
	case 2:
    {
		localCache.insert(documentGetAllowedTypes, overloads.at(1).assertedAndMappedCopy(
			documentGetAllowedTypes
		));
    }
        Q_FALLTHROUGH();
	case 1:
    {
        QStringList t;
        t<<"ids";
        localCache.insert(documentSetFilter, overloads.at(0).assertedAndMappedCopy(
            documentGetAllowedTypes, t,t
		));
    }
        Q_FALLTHROUGH();
	default:
		return;
	}
}

void FilterSelectWidget::_arrowReaction(int arrow)
{
	if (!typesel->hasFocus())
	{
		typesel->setCurrentIndex(doctypes->moveByArrow(arrow, typesel->currentIndex()));
	}
}

void FilterSelectWidget::_returnReaction()
{
	changeState(typesel->currentIndex());
}

void FilterSelectWidget::_sendDataRequest()
{
	loadFilters();
}
