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
#include "widgets/utils/EventsAndFilters.h"
#include "widgets/ElementWidgets/BigButtonsSpinbox.h"

class AbstractScaningWidget : public inframedWidget
{
	Q_OBJECT
protected:
	GlobalAppSettings& globalSettings;

	Document document;

	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QLabel* userInfo;
	QLabel* stateInfo;
	QLineEdit* barcodeField;
	QTextEdit* mainTextView;
	BigButtonsSpinbox* quantitySpin;
	QHBoxLayout* buttonPanel;
	QPushButton* backButton;
	QPushButton* submitButton;
	QPushButton* searchButton;

	filters::NoKeyEvents* keyFilter;

	QHash<QString, QString> itemSuppliedValues;
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

public slots:
	virtual void setDocument(parsedOrder) = 0;
};
