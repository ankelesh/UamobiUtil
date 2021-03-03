#include "IdDependMultiselectWidget.h"
#include "datacore/EntityListEntity.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "networking/Parsers/RequestParser.h"
#include "widgets/ExtendedDelegates/ZebraListItemDelegate.h"
#ifdef DEBUG 
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
#include "datacore/UniformXmlObject.h"


void IdDependMultiselectWidget::pickClicked()
{
	if (awaiter->isAwaiting())
		return;
	QModelIndexList currentItems = selectionView->selectionModel()->selectedIndexes();
	if (currentItems.isEmpty())
		return;
	ERecordList toAwait(new EntityListEntity());
	RecEntity currentItem;
	for (QModelIndexList::iterator ind = currentItems.begin(); ind != currentItems.end(); ++ind)
	{
		currentItem = entityModel->getItemDirectly(*ind);
		if (!currentItem.isNull())
			toAwait->list.push_back(currentItem);
	}
	showProcessingOverlay();
	awaitsConfirmation = toAwait;
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &IdDependMultiselectWidget::parse_select_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(parse_select_response()));
#endif
	if (localCache.contains(receiptGetOrderInfo))
		AppNetwork->execQueryByTemplate(localCache[receiptGetOrderInfo],
			toAwait->getId(), dependency->getId(), awaiter);
	else
		AppNetwork->execQueryByTemplate(QueryTemplates::receiptGetOrderInfo,
			toAwait->getId(),
			dependency->getId(), awaiter);
}

void IdDependMultiselectWidget::itemSelected(RecEntity)
{

}

void IdDependMultiselectWidget::parse_get_response()
{
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	PolyResponse response = RequestParser::parseResponse(parser, prototype);
	if (response.isError)
	{
		userInfo->setText(response.errtext);
#ifdef DEBUG
		detrace_NRESPERR(response.errtext);
#endif
	}
	else if (response.objects.isEmpty())
	{
		userInfo->setText(tr("Request received, but no data provided: ") + dependency->getId());
	}
	else
	{
		entityModel->insertData(response.objects);
		XmlObjects selection = response.filterAdditionals("selection");
		if (!selection.isEmpty())
		{
			QString sel_string = selection.first()->directFieldsAccess().value("selection");
			int index = 0;
			for (QString::iterator s_status = sel_string.begin(); s_status != sel_string.end(); ++s_status)
			{
				switch (s_status->toLatin1())
				{
				case '1':
					selectionView->selectionModel()->select(entityModel->index(index, 0), QItemSelectionModel::SelectionFlag::Current);
					Q_FALLTHROUGH();
				case '0':
					++index;
					break;
				default:
					break;
				}
			}
		}
	}
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), Q_NULLPTR, Q_NULLPTR);
	hideProcessingOverlay();
}

void IdDependMultiselectWidget::parse_select_response()
{
	if (awaiter->wasTimeout())
		return;
	RichtextResponseParser parser(awaiter->restext, awaiter->errtext);
	if (parser.isSuccessfull())
	{
		awaitsConfirmation->setTitle(parser.read().first()->value("richdata"));
		emit done(awaitsConfirmation);
	}
	else
	{
		userInfo->setText(parser.getErrors());
#ifdef DEBUG
		detrace_NRESPERR(parser.getErrors());
#endif
	}
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), Q_NULLPTR, Q_NULLPTR);
	hideProcessingOverlay();
}

IdDependMultiselectWidget::IdDependMultiselectWidget(RecEntity proto, QWidget* parent)
	: IdDependentSelectWidget(proto, parent)
{
	selectionView->setSelectionMode(QListView::MultiSelection);
#ifdef QT_VERSION5X
	QObject::disconnect(entityModel, &DataEntityListModel::dataEntityClicked, this, &IdDependMultiselectWidget::itemSelected);
#else
	QObject::disconnect(entityModel, SIGNAL(dataEntityClicked(RecEntity)), this, SLOT(itemSelected(RecEntity)));
#endif
}
