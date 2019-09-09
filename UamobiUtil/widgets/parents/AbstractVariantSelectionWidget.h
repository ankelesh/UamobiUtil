#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QWidget>
#include <QtWidgets/QBoxLayout>
#else
// Qt 4 only imports
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#endif

// widgets imports
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/SpecializedWidgets.h"

/*
	This widget provides an abstract selection widget based on virtuals method which will provide required info for 
	construction buttons:
		String representation of elements
		Number of elements
		Item selected emitting

*/


class AbstractVariantSelectionWidget : public inframedWidget
{
	Q_OBJECT
private:
	QVBoxLayout* mainLayout;			//	layout where buttons will be placed
	QVector<specwidgets::indexedButton* > userButtons;	//	list with buttons
protected:
	virtual QString elemAsString(int index) = 0;	//	must return string which will be displayed on button
	virtual int countElems() = 0;					//	must return size of structure, where indexing works
	void init();									//	abstract initialization of button list
public:
	AbstractVariantSelectionWidget(QWidget* parent = Q_NULLPTR, bool test = false);	//	"test" value adds empty button with "TEST'
	void reload();									//	deletes all buttons, then shows them again
protected slots:
	virtual void indexSelected(int Index) = 0;		//	must emit any kind of itemSelected()
}; 
