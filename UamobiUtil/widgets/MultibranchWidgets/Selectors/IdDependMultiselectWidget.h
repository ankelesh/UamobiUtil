#pragma once
#include "widgets/MultibranchWidgets/IdDependentSelectWidget.h"
#include "datacore/EntityListEntity.h"


class IdDependMultiselectWidget : public IdDependentSelectWidget
{
	Q_OBJECT
protected:
	bool _askSelectedListConfirmation(ERecordList & list);
	virtual void pickClicked() override;
	virtual void itemSelected(RecEntity) override;
	virtual void parse_get_response() override;
	virtual void parse_select_response() override;
public:
	IdDependMultiselectWidget(RecEntity proto, QWidget* parent = Q_NULLPTR);
};