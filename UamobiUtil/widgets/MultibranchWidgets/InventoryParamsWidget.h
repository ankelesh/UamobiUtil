#pragma once
#include "submodules/UNAQtCommons/widgets/parents/inframedWidget.h"
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QLabel>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QTextEdit>
#else
 // Qt 4 only imports
#include <QtGui/QLabel>
#include <QtGui/QDateTimeEdit>
#include <QtGui/QBoxLayout>
#include <QtGui/QTextEdit>

#endif
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include "networking/things.h"
#include "widgets/parents/IndependentBranchNode.h"
/*
	This widget is used for checking inventory document before starting completing it. 
	It has no additional functions except changing document credintials.
	TODO: dateedit is unused now. Make it usefull.


*/

class InventoryParamsWidget : public IndependentBranchNode
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	QLabel* title;
	QLabel* dateInfo;
	QDateEdit* dateField;
	MegaIconButton* selectDocumentButton;
	QLabel* commentInfo;
	QTextEdit* commentField;
	QLabel* statusInfo;
	QHBoxLayout* compcancelPanel;
	MegaIconButton* completedButton;
	MegaIconButton* cancelledButton;
	QHBoxLayout* footerLayout;
	MegaIconButton* backButton;
	MegaIconButton* okButton;
	FullDocument document;

	// Inherited via IndependentBranchNode
	virtual void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
public:
	InventoryParamsWidget(QWidget* parent = Q_NULLPTR);

	// renundant -> sets inner data to correspond provided document
	virtual void setFocus() override;
	void setDocument(FullDocument);
private slots:
	// emits backRequired - now in dynamicBranches is not possible to go to special one
	void selectDocPressed();
	// emits done and fills it with FullDocument created from provided data
	void okPressed();

};
