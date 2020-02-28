#pragma once
#include "widgets/MultibranchWidgets/DocResultsWidget.h"

class EditableDocResultsWidget : public DocResultsWidget
{
	Q_OBJECT
private:
	MegaIconButton* editItem;
public:
	EditableDocResultsWidget(QWidget* parent);
private slots:
	void deleteCurrent();
};