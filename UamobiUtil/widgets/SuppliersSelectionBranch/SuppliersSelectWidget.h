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
#include "widgets/parents/AbstractSearchAndPickWidget.h"
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

class SuppliersSelectWidget : public AbstractSearchAndPickWidget
{
	Q_OBJECT
protected:
	QVector<parsedSupplier> allsuppliers;
	interpretsPointers::interpretAsSupplierLike interpreter;

	// Inherited via AbstractSearchAndPickWidget
	virtual void interpretResult() override;
public:
	SuppliersSelectWidget(GlobalAppSettings& go, QWidget* parent,
		QString itemName,
		interpretsPointers::interpretAsSupplierLike inter = &RequestParser::interpretAsSupplierList, bool isOrdNeeded = true);

	virtual bool isExpectingControl(int) override;
protected slots:
	virtual void supplierPicked(parsedSupplier);
signals:
	void supplierAcquired(parsedSupplier);

};

class SuppliersSelectionBranch : public SuppliersSelectWidget, abstractNode
{
	Q_OBJECT
protected:

	parsedSupplier confirmedSupplier;
	OrderSelectionWidget* orderSelection;

public:
	SuppliersSelectionBranch(GlobalAppSettings& go, QWidget* parent,
		QString item,
		interpretsPointers::interpretAsSupplierLike inter = &RequestParser::interpretAsSupplierList);
	virtual void show() override;

protected slots:
	virtual void hideCurrent();
	virtual void supplierPicked(parsedSupplier) override;
	virtual void orderAcquired(parsedOrder);
};
