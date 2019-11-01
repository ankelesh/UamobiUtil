#pragma once
#include "AbstractVariantSelectionWidget.h"




class AbstractCheckboxSelection : public AbstractVariantSelectionWidget
{
	Q_OBJECT
protected:
	QVector<bool> & states;
	QIcon isT;
	QIcon nonT;
	virtual void indexSelected(int Index) override;
	virtual void init() override;
public:
	AbstractCheckboxSelection(QVector<bool> & States, QWidget * parent = Q_NULLPTR);
};