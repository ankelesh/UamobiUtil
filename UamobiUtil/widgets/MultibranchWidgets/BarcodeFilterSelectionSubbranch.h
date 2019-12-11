#pragma once
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
#include "widgets/parents/inframedWidget.h"
#include "widgets/parents/AbstractVariantSelectionWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
#include "widgets/SuppliersSelectionBranch/SuppliersSelectWidget.h"
#include "widgets/MultibranchWidgets/GroupSelectionWidget.h"
#include "widgets/MultibranchWidgets/StillageSelectionWidget.h"

/*
	This widget branch is allowing user to select barcode filter that will be sent to server.

	__ASSOCIATED_DB_FUNCTION__	:	void	select_filter_item(filter, value)

*/


class BarcodeFilterSelectionSubbranch : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
	GlobalAppSettings& globalSettings;

	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	MegaIconButton* stillageButton;
	MegaIconButton* suppliersButton;
	MegaIconButton* groupButton;
	MegaIconButton* nofilterButton;
	MegaIconButton* backButton;

	SuppliersSelectWidget* supplierWidget;
	StillageSelectionWidget* stillageWidget;
	GroupSelectionWidget* groupWidget;

public:
	BarcodeFilterSelectionSubbranch(GlobalAppSettings & go, QWidget* parent = Q_NULLPTR);

protected slots:
	void supplierSelectRequired();
	void stillageSelectRequired();
	void groupSelectRequired();
	void supplierProxyWrapper(parsedSupplier);
	void noFilterRequired();
	void applyFilter(QString param, QString type);
	void hideCurrent();
signals:
	void selectionHappened();
};