#include "BranchFactory.h"
#include "widgets/MultibranchWidgets/BarcodeFilterSelectionSubbranch.h"
#include "widgets/MultibranchWidgets/PagedSearchWidget.h"
#include "widgets/MultibranchWidgets/FilterSelectWidget.h"
#include "widgets/MultibranchWidgets/SelectItemFromListWidget.h"
#include "widgets/MultibranchWidgets/PlaceSelectionWidget.h"
#include "widgets/MultibranchWidgets/NormalScaningWidget.h"
#include "widgets/MultibranchWidgets/ParentDocumentWidget.h"
#include "widgets/MultibranchWidgets/ReceiptParametersWidget.h"
#include "widgets/MultibranchWidgets/IdDependentSelectWidget.h"
#include "widgets/MultibranchWidgets/DocResultsWidget.h"
#include "widgets/MultibranchWidgets/InventoryParamsWidget.h"
#include "widgets/MultibranchWidgets/PrintingScaningWidget.h"
#include "AdjustableBranch.h"
#include "StaticBranch.h"
#include "widgets/Branches/SwitchSubbranch.h"
#include "widgets/Branches/SenderNode.h"


AbsBranch* BranchFactory::createNWBranch(QString description, QWidget* parent, independent_nodes::nodelist branchType)
{
	using namespace independent_nodes;
	BranchDescription root = BranchDescriptionParser::parse(description);
	if (root->type == independent_nodes::NotANode)
		return Q_NULLPTR;
	else
	{
		switch (branchType)
		{
		case Subbranch:
			return new AdjustableBranch(root, parent);
		case StaticSubbranch:
			return new StaticBranch(root, parent);
		default:
			return Q_NULLPTR;
		}
		
	}
}

IndependentBranchNode* BranchFactory::createAdjustableBranchAsNode(BranchDescription e, QWidget* parent)
{
	if (e->type != independent_nodes::Subbranch)
		return Q_NULLPTR;
	else
		return new AdjustableBranch(e, parent);
}

using namespace independent_nodes;
using namespace QueryTemplates;
namespace BranchNodeFactory {
	IndependentBranchNode* createNode(const BranchDescription e, QWidget* parent)
	{
		IndependentBranchNode* element;
		switch (e->type)
		{
		case DocResults:
			element = new DocResultsWidget(parent);
			break;
		case FilterSelect:
			element = new FilterSelectWidget(parent);
			break;
		case IdDependentSelect:
			element = new IdDependentSelectWidget(
				((e->entity.isNull()) ? RecEntity(new SupplierEntity()) : e->entity),
				parent
			);
			break;
		case InventoryParameters:
			element = new InventoryParamsWidget(parent);
			break;
		case NormalScaning:
			element = new NormalScaningWidget(parent);
			break;
		case PagedSearch:
			element = new PagedSearchWidget(
				((e->entity.isNull()) ? RecEntity(new ShortItemEntity()) : e->entity),
				parent
			); break;
		case independent_nodes::PlaceSelect:
			element = new PlaceSelectionWidget(parent);
			break;
		case independent_nodes::ReceiptParameters:
			element = new ReceiptParametersWidget(parent);
			break;
		case independent_nodes::SelectItemFromList:
			element = new SelectItemFromListWidget(parent,
				((e->entity.isNull()) ? RecEntity(new SupplierEntity()) : e->entity));
			break;
		case ParentDocument:
			element = new ParentDocumentWidget(
				((e->entity.isNull()) ? RecEntity(new LesserDocumentEntity()) : e->entity),
				parent);
			break;
		case independent_nodes::BarcodeFilterSelect:
			element = new BarcodeFilterSelectionSubbranch(parent,
				((e->count() > 0) ? createNode(e->first(), Q_NULLPTR) : Q_NULLPTR),
				((e->count() > 1) ? createNode(e->at(1), Q_NULLPTR) : Q_NULLPTR),
				((e->count() > 2) ? createNode(e->at(2), Q_NULLPTR) : Q_NULLPTR)
			);
			break;
		case independent_nodes::Subbranch:
			element = BranchFactory::createAdjustableBranchAsNode(e, parent);
			break;
		case PrintingScaning:
			element = new PrintingScaningWidget(parent, 
				((e->isEmpty()) ?  Q_NULLPTR : createNode(e->first(), Q_NULLPTR)));
			break;
		case Switch:
			element = new SwitchSubbranch(e, parent);
			break;
		case Sender:
			element = new SenderNode(parent);
			break;
		case StaticSubbranch:
			element = new StaticBranch(e, parent);
			break;
		default:
			return Q_NULLPTR;
		}
		element->backtracking() = e->backtracking;
		element->setOverloads(e->oqs);
		return element;
	}
}
