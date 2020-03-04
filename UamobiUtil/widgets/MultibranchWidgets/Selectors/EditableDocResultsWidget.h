#pragma once
#include "widgets/MultibranchWidgets/DocResultsWidget.h"

class EditableDocResultsWidget : public DocResultsWidget
{
	Q_OBJECT
private:
	MegaIconButton* editItem;
	virtual void handleDelete() override;
public:
	EditableDocResultsWidget(QWidget* parent);
private slots:
	void editCurrent();
};