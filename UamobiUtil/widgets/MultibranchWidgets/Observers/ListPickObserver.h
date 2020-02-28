#pragma once
#include "widgets/parents/IndependentBranchNode.h"
#include "widgets/MultibranchWidgets/IdDependentSelectWidget.h"




class ListPickObserver : public IdDependentSelectWidget
{
	Q_OBJECT
protected:
	virtual void itemSelected(RecEntity) override;
	virtual void parse_get_response() override;
	virtual void _handleRecord(RecEntity) override;
public:
	ListPickObserver(RecEntity proto, QWidget* parent = Q_NULLPTR);
};
