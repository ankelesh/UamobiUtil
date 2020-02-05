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
#include "widgets/parents/IndependentBranchNode.h"
#include "widgets/parents/AbstractVariantSelectionWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
#include "widgets/parents/SelectItemFromListWidget.h"
#include "networking/things.h"
/*
	This widget branch is allowing user to select barcode filter that will be sent to server.

	__ASSOCIATED_DB_FUNCTION__	:	void	select_filter_item(filter, value)

*/

class BarcodeFilterSelectionSubbranch : public IndependentBranchNode, abstractNode
{
	Q_OBJECT
protected:

	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QLabel * currentFilterInfo;
	MegaIconButton* stillageButton;
	MegaIconButton* suppliersButton;
	MegaIconButton* groupButton;
	MegaIconButton* nofilterButton;
	MegaIconButton* passButton;
	MegaIconButton* backButton;

	SelectItemFromListWidget* supplierWidget;
	SelectItemFromListWidget* stillageWidget;
	SelectItemFromListWidget* groupWidget;
	RequestAwaiter* awaiter;

	RecEntity doc;
	virtual void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
public:
	BarcodeFilterSelectionSubbranch(QWidget* parent = Q_NULLPTR);
	void assertAndShow(QString& pdoc);
	void applyFilter(QString param, QString type);
protected slots:
	void supplierSelectRequired();
	void stillageSelectRequired();
	void groupSelectRequired();
	void noFilterRequired();
	void filterSelected(RecEntity);
	void got_response();
	void hideCurrent();

};