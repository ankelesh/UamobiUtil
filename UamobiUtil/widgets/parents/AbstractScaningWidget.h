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
	virtual void wipe();
public:
	AbstractScaningWidget(int id, QWidget* parent);
	void clear();
	void setModeName(QString& name);

protected slots:
	virtual void submitPressed() = 0;
	virtual void barcodeConfirmed() = 0;
	virtual void searchRequired() = 0;
	virtual void backNeeded() = 0;
	virtual void was_timeout();
    virtual void quitNoSave();
public slots:
	virtual void setDocument(Order) = 0;
};
QVector<QPair<QString, QString> > make_control_list(QHash<QString, QString>&);
