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
#include "submodules/UNAQtCommons/widgets/parents/inframedWidget.h"
#include "networking/Parsers/RequestParser.h"
#include "networking/RequestAwaiter.h"
#include "widgets/utils/EventsAndFilters.h"
#include "widgets/ControlsMiniwidgets/ControlManager.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include "networking/things.h"
#include "submodules/UNAQtCommons/widgets/ControlsWidgets/abs_control.h"


#ifdef Q_OS_ANDROID
class SwitchableIMDecorator;
#endif
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
#ifdef Q_OS_ANDROID
	QHBoxLayout* barcodeLayout;
	MegaIconButton* switchKeyboardTypeButton;
	SwitchableIMDecorator* switchDecorator;
#endif
	QTextEdit* mainTextView;
    QVBoxLayout* controlPanel;
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

	virtual void _backReaction() override;
    virtual void useControls(const QVector<QSharedPointer<InputControlEntity> > &);
	virtual void wipe();
	virtual void setDocument(RecEntity e);
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
};
QVector<QPair<QString, QString> > make_control_list(QHash<QString, QString>&);
