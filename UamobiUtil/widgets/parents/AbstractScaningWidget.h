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
#include "widgets/parents/IndependentBranchNode.h"
#include "widgets/parents/inframedWidget.h"
#include "networking/Parsers/RequestParser.h"
#include "networking/RequestAwaiter.h"
#include "widgets/utils/EventsAndFilters.h"
#include "widgets/ControlsMiniwidgets/ControlManager.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
#include "ScaningCore/NormalCapturer.h"
#include "networking/things.h"

class AbstractScaningWidget : public IndependentBranchNode
{
	Q_OBJECT
protected:

	FullDocument document;

	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QHBoxLayout* topPanelLayout;
	QLabel* userInfo;
	QLineEdit* barcodeField;
	QTextEdit* mainTextView;
	QHBoxLayout* buttonPanel;
	MegaIconButton* quitButton;
	MegaIconButton* switchFocus;
	MegaIconButton* backButton;
	MegaIconButton* submitButton;
	MegaIconButton* searchButton;

	QHash<QString, QString> itemSuppliedValues;

	Control first_control;
	Control second_control;

	int controlsAvailable;
	bool controlsRequired;

	QString modename;
	RequestAwaiter* awaiter;
	virtual void useControls(QVector<QSharedPointer<InputControlEntity> > &);
	virtual void refreshControls();
	virtual bool checkControls();
	virtual void focusControl(int);
	virtual bool isControlFocused();
	virtual void _postClear() =0;
public:
	AbstractScaningWidget(int id, QWidget* parent);
	void clear();
	void setModeName(QString& name);
protected slots:
	virtual void processBarcode(QString) = 0;
	virtual void submitPressed() = 0;
	virtual void barcodeConfirmed() = 0;
	virtual void processNumber(QString) = 0;
	virtual void searchRequired() = 0;
	virtual void backNeeded() = 0;
	virtual void was_timeout();
	virtual void switchedFocus() = 0;
	virtual void syncControlAndBuffer(QString v) = 0;
    virtual void quitNoSave();
public slots:
	virtual void setDocument(Order) = 0;
signals:
	void saveSuccess();
};
QVector<QPair<QString, QString> > make_control_list(QHash<QString, QString>&);
