#pragma once
// Qt 5 only headers
#ifdef QT_VERSION5X
#include <QtWidgets/QWidget>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QMainWindow>
// Qt 4 only headers
#else
#include <QtGui/QWidget>
#include <QtGui/QBoxLayout>
#include <QtGui/QMainWindow>
#endif
// Qt headers
#include <QtCore/QPointer>
// widgets headers
#include "widgets/parents/inframedWidget.h"
#include "widgets/MainPageWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/ModeSelectionBranch/ModeSelectionWidget.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "widgets/BranchingTools/AbsBranch.h"
#include "widgets/BranchingTools/BranchFactory.h"
/*
	This is main widget of the project. It is based on QWidget and has no gui representation. Main goal
	of this widget - is providing root functionality in widget tree. It is responsible for dynamical allocation of widgets,
	hiding branches, opening branches.
		Slots:
			Slots of this widget are concentrated around switching branches. All slots with this functionality must be
			named with syntax goto<WidgetName>. If branch does not requires initialization, it's okay to connect it to gotoBranch,
			which needs only value of enum 'branches'.

*/

class UamobiUtil : public QWidget, abstractDynamicNode
{
	Q_OBJECT
private:		
	ProcessingOverlay* overlay;
	void resizeEvent(QResizeEvent* rev) override;
public:

    UamobiUtil( QWidget* parent = Q_NULLPTR);	//	Constructor is similar to most other constructors

private slots:
	void gotoModeSelection();			//	Switch to modeSelectionBranch
	void interpretMode(QHash<QString, QString>, Mode mode);	
public slots:
	void hideCurrent();					//	hides current branch if it is not main page
	void closeBranch(RecEntity e);
signals:
	void requestSettings();				//	Deprecated, but may be usefull if app will be reassembled
};

