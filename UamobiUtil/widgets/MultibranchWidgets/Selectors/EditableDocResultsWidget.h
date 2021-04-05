#pragma once
#include "widgets/MultibranchWidgets/DocResultsWidget.h"

class EditableDocResultsWidget : public DocResultsWidget
{
	Q_OBJECT
private:
	MegaIconButton* editItem;
	virtual void handleDelete() override;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
	virtual void attachedControlsDone() override; 
public:
	EditableDocResultsWidget(QWidget* parent);
private slots:
	void editCurrent();
};