#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#else
 // Qt 4 only imports
#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QBoxLayout>
#include <QtGui/QLabel>

#endif
// widgets imports
#include "widgets/parents/inframedWidget.h"
#include "widgets/parents/AbstractListSelectionWidget.h"
#include "OrderSelectionWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
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

	__ASSOCIATED_DATABASE_FUNCTION__  :   P'suppliersResponse' list_suppliers(part_of_name, withorders)
	__OPTIONAL_REPLACEMENT_DBFUNCTION__ : P'suppliersResponse' list_warehouses(part_of_name, withorders)
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
		_SupplierSelectionWidget(QVector<parsedSupplier>& supplist, QWidget* parent = Q_NULLPTR);
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

	// pointers
	interpretsPointers::interpretAsSupplierLike interpreter;
	SuppliersLikeMP listSuppliers;

	parsedSupplier confirmedSupplier;

	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QHBoxLayout* headerLayout;
	QHBoxLayout* footerLayout;
	QLabel* userHelp;
	QLineEdit* userinputField;
	MegaIconButton* searchButton;
	MegaIconButton* ordfilterButton;
	specwidgets::_SupplierSelectionWidget* supplierSelection;
	MegaIconButton* backButton;

	QIcon withOrd;
	QIcon withoutOrd;
	RequestAwaiter awaiter;

public:
	SuppliersSelectWidget(GlobalAppSettings& go, QWidget* parent,
		SuppliersLikeMP meth = &DataUpdateEngine::recListSuppliers,
		interpretsPointers::interpretAsSupplierLike inter = &RequestParser::interpretAsSupplierList);

	virtual void show() override;
	virtual bool isExpectingControl(int) override;
protected slots:
	void searchPrimed();
	void ordFilterSwitched(bool);
	virtual void supplierPicked(parsedSupplier);
	void parse_response();
	void was_timeout();
public slots:
	void loadSuppliers();
signals:
	void supplierAcquired(parsedSupplier);
};

class SuppliersSelectionBranch : public SuppliersSelectWidget, abstractNode
{
	Q_OBJECT
protected:

	OrderSelectionWidget* orderSelection;

public:
	SuppliersSelectionBranch(GlobalAppSettings& go, QWidget* parent,
		SuppliersLikeMP meth = &DataUpdateEngine::recListSuppliers,
		interpretsPointers::interpretAsSupplierLike inter = &RequestParser::interpretAsSupplierList);
	virtual void show() override;
	
protected slots:
	virtual void hideCurrent();
	virtual void supplierPicked(parsedSupplier) override;
	virtual void orderAcquired(parsedOrder);
};
