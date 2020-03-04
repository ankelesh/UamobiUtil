#include "EditableDocResultsWidget.h"
#include "widgets/utils/ElementsStyles.h"


void EditableDocResultsWidget::handleDelete()
{
	if (awaiter->isAwaiting())
		return;
	if (!itemInfoStorage->currentIndex().isValid())
		return;
	RecEntity currentItem = itemInfoStorage->currentIndex().data(DataEntityListModel::DirectAccess
	).value<RecEntity>();
	if (!(currentItem->myType() == UniformXmlObject::Item))
		return;
	FullItem item = upcastRecord<FullItemEntity>(currentItem);
	if (item.isNull())
		return;
	if (localCache.contains(docDeleteByBarcode))
		item->sendDeleteThisRequest(localCache[docDeleteByBarcode], awaiter);
	else
		item->sendDeleteThisRequest(awaiter);
	deleteAllButton->hide();
	deleteSelectedButton->hide();
}

EditableDocResultsWidget::EditableDocResultsWidget(QWidget* parent)
	: DocResultsWidget(parent), editItem(new MegaIconButton(innerWidget))
{
	toolPanel->addWidget(editItem);
	editItem->setIcon(QIcon(":/res/pencil.png"));
	editItem->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);
	editItem->setText(tr("edit"));
	editItem->setMinimumWidth(calculateAdaptiveWidth(0.25));
	editItem->setFont(GENERAL_FONT);
	editItem->hide();
	
}
void EditableDocResultsWidget::editCurrent()
{
	if (awaiter->isAwaiting())
		return;
	if (!itemInfoStorage->currentIndex().isValid())
		return;
	RecEntity currentItem = itemInfoStorage->currentIndex().data(DataEntityListModel::DirectAccess
	).value<RecEntity>();
	if (!(currentItem->myType() == UniformXmlObject::Item))
		return;
	FullItem item = upcastRecord<FullItemEntity>(currentItem);
	if (item.isNull())
		return;
	if (localCache.contains(docDeleteByBarcode))
		item->sendDeleteThisRequest(localCache[docDeleteByBarcode], awaiter);
	else
		item->sendDeleteThisRequest(awaiter);
	deleteAllButton->hide();
	deleteSelectedButton->hide();
}