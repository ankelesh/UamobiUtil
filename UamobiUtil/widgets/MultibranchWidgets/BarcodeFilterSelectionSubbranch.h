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



class StillageSelectWidget;
class GroupSelectWidget;

class BarcodeFilterSelectionSubbranch : public inframedWidget, abstractNode
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	MegaIconButton* stillageButton;
	MegaIconButton* suppliersButton;
	MegaIconButton* groupButton;
	MegaIconButton* nofilterButton;
	MegaIconButton* backButton;

	SuppliersSelectWidget* supplierWidget;
	StillageSelectWidget* stillageWidget;
	GroupSelectWidget* groupWidget;

public:
	BarcodeFilterSelectionSubbranch(QWidget* parent = Q_NULLPTR);

protected slots:
	void continuePressed();
	void supplierSelectRequired();
	void stillageSelectRequired();
	void groupSelectRequired();
	void selectionHappened(QString);

};