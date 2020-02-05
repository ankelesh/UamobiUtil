#include "BranchNodeFactory.h"
#include "widgets/MultibranchWidgets/BarcodeFilterSelectionSubbranch.h"
#include "widgets/MultibranchWidgets/PagedSearchWidget.h"
#include "widgets/MultibranchWidgets/FilterSelectWidget.h"
#include "widgets/parents/SelectItemFromListWidget.h"
#include "widgets/ModeSelectionBranch/PlaceSelectionWidget.h"
#include "widgets/InventorySpecialWidgets/NormalScaningWidget.h"
#include "widgets/InventorySpecialWidgets/ParentDocumentWidget.h"
#include "widgets/ReceiptSpecialWidgets/ReceiptParametersWidget.h"
#include "widgets/SuppliersSelectionBranch/IdDependentSelectWidget.h"
#include "widgets/MultibranchWidgets/DocResultsWidget.h"
#include "widgets/InventoryBranch/InventoryParamsWidget.h"

using namespace independent_nodes;
using namespace QueryTemplates;
IndependentBranchNode* BranchNodeFactory::createNode(const nodelist id,
	const int backtracking, const QVector<OverloadableQuery>& overloads, 
	const RecEntity prototype, QWidget* parent)
{
	IndependentBranchNode* element;
	switch (id)
	{
	case DocResults:
		element = new DocResultsWidget(parent);
		break;
	case FilterSelect:
		element = new FilterSelectWidget(parent);
		break;
	case IdDependentSelect:
		element = new IdDependentSelectWidget(
			((prototype.isNull()) ? RecEntity(new SupplierEntity()) : prototype),
			parent
		);
		break;
	case InventoryParameters:
		element = new InventoryParamsWidget(parent);
		break;
	case NormalScaning:
		element = new NormalScaningWidget(
			parent,
			((overloads.count() > 0) ? overloads.at(0) : OverloadableQuery::defaultQuery()),
			((overloads.count() > 1) ? overloads.at(1) : OverloadableQuery::defaultQuery()),
			((overloads.count() > 2) ? overloads.at(2) : OverloadableQuery::defaultQuery())
				);
		break;
	case PagedSearch:
		element = new PagedSearchWidget(
			((prototype.isNull()) ? RecEntity(new ShortItemEntity()) : prototype),
			parent
		); break;
	case independent_nodes::PlaceSelect:
		element = new PlaceSelectionWidget(
			parent,
			((overloads.isEmpty()) ? placeList : overloads.first().getQID())
		); break;
	case independent_nodes::ReceiptParameters:
		element = new ReceiptParametersWidget(parent);
		break;
	case independent_nodes::SelectItemFromList:
		element = new SelectItemFromListWidget(parent,
			((prototype.isNull()) ? RecEntity(new SupplierEntity()) : prototype));
		if (!overloads.isEmpty())
		{
			SelectItemFromListWidget* temp = static_cast<SelectItemFromListWidget*>(element);
			temp->setOverload(overloads.first());
		}
		break;
	case ParentDocument:
		element = new ParentDocumentWidget(
			((prototype.isNull()) ? RecEntity(new LesserDocumentEntity()) : prototype),
			parent);
		break;
	case independent_nodes::BarcodeFilterSelect:
		element = new BarcodeFilterSelectionSubbranch(parent);
		break;
	default:
		return Q_NULLPTR;
	}
	element->backtracking() = backtracking;
	return element;
}
