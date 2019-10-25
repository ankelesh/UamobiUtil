#pragma once
// Qt 5 only imports
#include "widgets/utils/GlobalAppSettings.h"
#ifdef QT_VERSION5X
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#else
 // Qt 4 only imports
#include <QtGui/QBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#endif

#include "widgets/parents/inframedWidget.h"
#include "networking/Parsers/RequestParser.h"
#include "networking/RequestAwaiter.h"
#include "widgets/utils/EventsAndFilters.h"
#include"widgets/ControlsMiniwidgets/QuantityControl.h"
#include "widgets/ElementWidgets/MegaIconButton.h"

class AbstractScaningWidget : public inframedWidget
{
	Q_OBJECT
protected:
	GlobalAppSettings& globalSettings;

	Document document;

	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QHBoxLayout* topPanelLayout;
	QLabel* userInfo;
	QLabel* stateInfo;
	QLineEdit* barcodeField;
	QTextEdit* mainTextView;
	QHBoxLayout* buttonPanel;
	MegaIconButton* quitButton;
	MegaIconButton* backButton;
	MegaIconButton* submitButton;
	MegaIconButton* searchButton;


	QHash<QString, QString> itemSuppliedValues;
	QHash<QString, abs_control*> controlsList;

	RequestAwaiter awaiter;
    void keyReleaseEvent(QKeyEvent *kev) override;
public:
	AbstractScaningWidget(GlobalAppSettings& go, QWidget* parent);

protected slots:
	virtual void processBarcode(QString) = 0;
	virtual void submitPressed() = 0;
	virtual void barcodeConfirmed() = 0;
	virtual void processNumber(QString) = 0;
	virtual void searchRequired() = 0;
	virtual void backNeeded() = 0;
	virtual void useControls() = 0;
	virtual void was_timeout();
public slots:
	virtual void setDocument(parsedOrder) = 0;

};
