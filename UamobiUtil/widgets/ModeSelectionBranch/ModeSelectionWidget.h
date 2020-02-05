#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#else
// Qt 4 only imports
#include <QtGui/QVBoxLayout>
#include <QtGui/QScrollArea>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#endif

// widgets imports
#include "widgets/parents/inframedWidget.h"
#include "widgets/parents/AbstractVariantSelectionWidget.h"
#include "widgets/ModeSelectionBranch/PlaceSelectionWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
// networking imports
#include "networking/RequestAwaiter.h"
#include "networking/Parsers/RequestParser.h"
// utils imports
#include "widgets/utils/GlobalAppSettings.h"
#include "networking/things.h"

/*
	This file contains ModeSelectionWidget - which is root of ModeSelection branch. It loads
	Modes list from web, then allows user to choose one. PlaceSelection is part of this branch, because it must be chosed before
	other operations will continue.

	__ASSOCIATED_DATABASE_FUNCTION__  :  P'modesResponse' list_modes()
	__ASSOCIATED_DATABASE_FUNCTION__  :  P'PositionalResponse' select_mode (mode, submode)

*/

class ModeSelectionWidget : public inframedWidget	//	root of modeSelection branch
{
	Q_OBJECT
protected:
	// modes list
	DataEntityListModel* innerModel;

	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QHBoxLayout* buttonLayout;
	QLabel* userTip;
	QLabel* modesTip;
	QListView* modeSelection;
	MegaIconButton* logoutButton;

	// this list is used to store data obtained after select_mode web request. it will be usefull later
	QHash<QString, QString> settings;
	Mode selected;
	RequestAwaiter awaiter;
public:
	ModeSelectionWidget(QWidget* parent = Q_NULLPTR);
	virtual bool back() override;	//	has top widget
	virtual bool isExpectingControl(int) override;
	virtual void show() override;
protected slots:
	void logoutPressed();		//	this slot sends log_out and only then calls backRequired
	void modeSelected(RecEntity);	//	these slots react to changes
	virtual void hideCurrent();
	void parse_modes();				//	network reactions
	virtual void mode_select_response();
	void was_timeout();
public slots:
	void loadModes();				//	loads modes list from web
signals:
	void modeAcquired(QHash<QString, QString>, Mode pm);		//	is emitted when this branch is done
};
class ModeBranchRootWidget : public ModeSelectionWidget, abstractNode
{
	Q_OBJECT
protected:
	// child nodes
	PlaceSelectionWidget* placeSelection;
public:
	ModeBranchRootWidget( QWidget* parent = Q_NULLPTR);
protected slots:
	void placeAcquired(Place);
	virtual void hideCurrent() override;
	virtual void mode_select_response() override;
};