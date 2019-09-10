#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#else
 // Qt 4 only imports
throw;
#endif
// widgets imports
#include "widgets/parents/inframedWidget.h"
#include "widgets/parents/AbstractListSelectionWidget.h"
#include "OrderSelectionWidget.h"
// networking imports
#include "networking/RequestAwaiter.h"
#include "networking/Parsers/RequestParser.h"
#include "networking/things.h"
// util imports
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/ElementsStyles.h"

/*
	This widget is suited for selecting a supplier. It is designed and as root element, and as top widget.
	
	Update: 
		Added branching to OrderSelection - possible renundancy
	TODO:
		More abstraction possible.


*/


namespace specwidgets
{
	class _SupplierSelectionWidget : public AbstractListSelectionWidget
	{
		Q_OBJECT
	protected:
		QVector<parsedSupplier>& suppliers;

		virtual QString elemToString(int) override;
		virtual int countElems() override;
	public:
		_SupplierSelectionWidget(QVector<parsedSupplier> & supplist, QWidget* parent = Q_NULLPTR);
	protected slots:
		virtual void itemSelectedFromList(QListWidgetItem*) override;
	signals:
		void supplierPicked(parsedSupplier);
	};
}



class SuppliersSelectWidget : public inframedWidget
{
	Q_OBJECT
protected:
	GlobalAppSettings& globalSettings;
	QVector<parsedSupplier> allsuppliers;

	parsedSupplier confirmedSupplier;

	QVBoxLayout* mainLayout;
	inframedWidget * innerWidget;
	QVBoxLayout* innerLayout;
	QHBoxLayout* headerLayout;
	QHBoxLayout* footerLayout;
	QLabel* userHelp;
	QLineEdit* userinputField;
	QPushButton* searchButton;
	QPushButton* ordfilterButton;
	specwidgets::_SupplierSelectionWidget* supplierSelection;
	QPushButton* backButton;

	OrderSelectionWidget * orderSelectBranch;

	inframedWidget* current;

public:
	SuppliersSelectWidget(GlobalAppSettings& go, QWidget* parent);

private slots:
	void searchPrimed();
	void ordFilterSwitched(bool);
	void supplierPicked(parsedSupplier);
public slots:
	void hideCurrent();

	void loadSuppliers();
};