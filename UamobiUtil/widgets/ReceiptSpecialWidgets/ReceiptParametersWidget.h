#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X

#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QFrame>

#else
 // Qt 4 only imports

#include <QtGui/qpushbutton.h>
#include <QtGui/QBoxLayout>
#include <QtGui/QDateEdit>
#include <QtGui/QTextEdit>
#include <QtGui/QLabel>
#include <QtGui/QFrame>

#endif

// widgets imports
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/parents/abstractNodeInterface.h"
//networking imports
#include "networking/RequestAwaiter.h"
#include "networking/Parsers/RequestParser.h"

class ReceiptParametersWidget : public inframedWidget
{
	Q_OBJECT
protected:
	GlobalAppSettings& globalSettings;

	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QLabel* userInfo;
	QFrame* innerFrame;
	QVBoxLayout* frameLayout;
	QLabel* stateInfo;
	QPushButton* closedButton;
	QPushButton* cancelledButton;
	QDateEdit* dateField;
	QHBoxLayout* selectOrderLayout;
	QPushButton* selectOrderButton;
	QTextEdit* mainTextView;
	QHBoxLayout* inspectLayout;
	QPushButton* inspectButton;
	QHBoxLayout* continueLayout;
	QPushButton* continueButton;
	QPushButton* backButton;

public:
	ReceiptParametersWidget(GlobalAppSettings& go, QWidget* parent);

protected slots:
	void closedClicked();
	void cancelledClicked();
	void inspectClicked();
	void continueClicked();

public slots:
	void setMainView(const QString&);
signals:
	void dataConfirmed();
};
