#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X

#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFrame>

#else
 // Qt 4 only imports

#include <QtGui/QBoxLayout>
#include <QtGui/QDateEdit>
#include <QtGui/QTextEdit>
#include <QtGui/QLabel>
#include <QtGui/QFrame>

#endif

// widgets imports
#include "widgets/parents/IndependentBranchNode.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
//networking imports
#include "networking/RequestAwaiter.h"
#include "networking/Parsers/RequestParser.h"

/*
	This widget is used for showing receipt parameters. It has a lot of rarely used buttons which are hidden.
	This widget is info-only, and is used mostly to check data and go to next or previous step.
*/

class ReceiptParametersWidget : public IndependentBranchNode
{
	Q_OBJECT
protected:

	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QFrame* innerFrame;
	QVBoxLayout* frameLayout;
	QLabel* stateInfo;
	MegaIconButton* closedButton;
	MegaIconButton* cancelledButton;
	QDateEdit* dateField;
	QTextEdit* mainTextView;
	QHBoxLayout* inspectLayout;
	MegaIconButton* inspectButton;
	QHBoxLayout* continueLayout;
	MegaIconButton* continueButton;
	MegaIconButton* backButton;

	RecEntity dependency;

	// Inherited via IndependentBranchNode
	virtual void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
public:
	ReceiptParametersWidget(QWidget* parent);
	void setMainView(const QString&);		//	uses provided string for mainView content
	virtual void setFocus() override;
protected slots:
	void closedClicked();		//	button reactions
	void cancelledClicked();
	void inspectClicked();
	void continueClicked();

};
