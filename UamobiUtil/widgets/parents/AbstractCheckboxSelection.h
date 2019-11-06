#pragma once
#include "AbstractVariantSelectionWidget.h"


/*
	This subtype of variant selection is used for replacing normal checkbox-forms with toggle-able list of buttons.
	It requires list of bool values meaning states.


*/



class AbstractCheckboxSelection : public AbstractVariantSelectionWidget
{
	Q_OBJECT
protected:
	QVector<bool>& states;
	QIcon isT;
	QIcon nonT;
	virtual void indexSelected(int Index) override;
	virtual void init() override;
public:
	AbstractCheckboxSelection(QVector<bool>& States, QWidget* parent = Q_NULLPTR);
};