#include "EditableDocResultsWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include <QMessageBox>

void EditableDocResultsWidget::handleDelete()
{
	if (awaiter->isAwaiting())
		return;
	if (!itemInfoStorage->currentIndex().isValid())
		return;
	RecEntity item = itemInfoStorage->currentIndex().data(DataEntityListModel::DirectAccess
	).value<RecEntity>();
	switch (item->myType())
	{
	case UniformXmlObject::Invoice:
		return;
		break;
	case UniformXmlObject::Item:
	{
		FullItem c_item = item.staticCast<FullItemEntity>();
		int res = QMessageBox::question(this, tr("confirm delete"), tr("Do you want to delete ") + QString::number(c_item->qty) + tr(" of ") +
			c_item->title, QMessageBox::Ok | QMessageBox::Abort);
		if (res == QMessageBox::Ok)
		{
			if (!localCache.contains(docDeleteByBarcode))
			{
				AppNetwork->execQueryByTemplate(docDeleteByBarcode, c_item->getId(), QString::number(c_item->qty), awaiter);
			}
			else
			{
				AppNetwork->execQueryByTemplate(localCache[docDeleteByBarcode],
					c_item->getId(), QString::number(c_item->qty), awaiter);
			}
		}
	}
	break;
	case UniformXmlObject::Separator:
		return;
	}

}

void EditableDocResultsWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	using namespace QueryTemplates;
	if (overloads.count() >= 7)
	{
		localCache.insert(editItemById, overloads.at(6).assertedAndMappedCopy(
			editItemById));
	}
	if (overloads.count() >= 8)
	{
		localCache.insert(editInvoiceById, overloads.at(7).assertedAndMappedCopy(
			editInvoiceById));
	}
	DocResultsWidget::_makeOverloads(overloads);
}

void EditableDocResultsWidget::attachedControlsDone()
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
	
	switch (currentItem->myType())
	{
	case UniformXmlObject::Item:
		if (!localCache.contains(editItemById))
		{
			AppNetwork->execQueryByTemplate(editItemById, currentItem->getId(), attachedControls->mappedControls(), awaiter);
		}
		else
		{
			AppNetwork->execQueryByTemplate(localCache[editItemById],
				currentItem->getId(), attachedControls->mappedControls(), awaiter);
		};
		break;
	case UniformXmlObject::Invoice:
		if (!localCache.contains(editInvoiceById))
		{
			AppNetwork->execQueryByTemplate(editInvoiceById, currentItem->getId(), attachedControls->mappedControls(), awaiter);
		}
		else
		{
			AppNetwork->execQueryByTemplate(localCache[editInvoiceById],
				currentItem->getId(), attachedControls->mappedControls(), awaiter);
		}
		break;
	default:
		break;
	}
	_hideAny(innerWidget);
	hideProcessingOverlay();
}

EditableDocResultsWidget::EditableDocResultsWidget(QWidget* parent)
	: DocResultsWidget(parent), editItem(new MegaIconButton(innerWidget))
{
	toolPanel->addWidget(editItem);
	editItem->setIcon(QIcon(":/resources/pencil"));
	editItem->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);
	editItem->setText(tr("edit"));
	editItem->setMinimumWidth(calculateAdaptiveWidth(0.25));

	editItem->setMaximumHeight(calculateAdaptiveHeight(0.08));
	editItem->setFont(GENERAL_FONT);
	
#ifdef QT_VERSION5X
	QObject::connect(editItem, &MegaIconButton::clicked, this, &EditableDocResultsWidget::editCurrent);
#else
	QObject::connect(editItem, SIGNAL(clicked()), this, SLOT(editCurrent()));
#endif
}
void EditableDocResultsWidget::editCurrent()
{
	if (awaiter->isAwaiting())
		return;
	getAttachedControls();
}
