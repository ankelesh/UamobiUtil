#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QListWidget>
#else
 // Qt 4 only imports
#include <QtGui/QListWidget>
#endif

/*
	This widget has similar interface with AbstractVariantSelection, but is based on QListWidget instead of QPushButtons spam.
	You must inherit this and implement adapter virtual methods to prepare this to work.

	Update:
		added Zebra delegate

*/
class AbstractListSelectionWidget : public QListWidget
{
	Q_OBJECT
protected:
	virtual QString elemToString(int) = 0;
	virtual int countElems() = 0;
	void init();
public:
	AbstractListSelectionWidget(QWidget* parent);
	void reload();
protected slots:
	virtual void itemSelectedFromList(QListWidgetItem*) = 0;
};
