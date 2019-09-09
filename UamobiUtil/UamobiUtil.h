#pragma once
// Qt 5 only headers
#ifdef QT_VERSION5X
#include <QtWidgets/QWidget>
#include <QtWidgets/QBoxLayout>
// Qt 4 only headers
#else
#include <QtGui/QWidget>
#include <QtGui/QBoxLayout>
#endif
// Qt headers
#include <QtCore/QPointer>
// widgets headers
#include "widgets/parents/inframedWidget.h"
#include "widgets/MainPageWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/ModeSelectionBranch/ModeSelectionWidget.h"
#include "widgets/SuppliersSelectionBranch/SuppliersSelectWidget.h"
/*
	This is main widget of the project. It is based on QWidget and has no gui representation. Main goal
	of this widget - is providing root functionality in widget tree. It is responsible for dynamical allocation of widgets,
	hiding branches, opening branches.
		Slots:
			Slots of this widget are concentrated around switching branches. All slots with this functionality must be 
			named with syntax goto<WidgetName>. If branch does not requires initialization, it's okay to connect it to gotoBranch,
			which needs only value of enum 'branches'.


*/

class UamobiUtil : public QWidget
{
	Q_OBJECT
private:
	GlobalAppSettings & globalSettings;   // Link to global settings object


	QVBoxLayout* mainLayout;			//	These widgets are permanent frame with buttons.

	QPointer<inframedWidget> mainPage;			//	Login page. Remember - user must be logged in to perform complex ops.
	QPointer<inframedWidget> modeSelectionBranch;	//	Mode selection. Usually this branch must be shown after logging in
	QPointer<inframedWidget> suppliersSelectionBranch; // suppliers selection.

	QPointer<inframedWidget>* current;		//	This pointer is holding currently shown widget or branch

public:
	enum branches { login, modeselect};

    UamobiUtil(GlobalAppSettings & go, QWidget* parent = 0);	//	Constructor is similar to most other constructors

private slots:
	void gotoModeSelection();			//	Switch to modeSelectionBranch
	void gotoSupplierSelection();

	void interpretMode(QHash<QString, QString>);	//	interprets mode settings obtained through "select mode". NOT IMPLEMENTED
public slots:
	void hideCurrent();					//	hides current branch if it is not main page
	void gotoBranch(branches branch);	//	opens branch by it's enum type. Does not supports branches with any initialization.
signals:
	void requestSettings();				//	Deprecated, but may be usefull if app will be reassembled 
};
