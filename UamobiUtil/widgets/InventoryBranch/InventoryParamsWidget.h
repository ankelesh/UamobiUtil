#pragma once
#include "widgets/parents/inframedWidget.h"
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
#include "widgets/ElementWidgets/MegaIconButton.h"
#include "networking/things.h"
#include "widgets/utils/GlobalAppSettings.h"


class InventoryParamsWidget : public inframedWidget
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
	Document document;
public:
	InventoryParamsWidget(QWidget* parent = Q_NULLPTR);

	void setDocument(Document);
private slots:
	void selectDocPressed();
	void okPressed();
signals:
	void documentMustBeSelected(int);
	void documentConfirmed(Document);
};
