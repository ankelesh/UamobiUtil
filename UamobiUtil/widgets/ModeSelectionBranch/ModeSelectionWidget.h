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
#include <qtableview.h>
#include <qheaderview.h>
#include "datacore/PseudotableEntityModel.h"
// widgets imports
#include "submodules/UNAQtCommons/widgets/parents/inframedWidget.h"
#include "widgets/MultibranchWidgets/PlaceSelectionWidget.h"
#include "submodules/UNAQtCommons/widgets/parents/abstractNodeInterface.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
// networking imports
#include "networking/RequestAwaiter.h"
#include "networking/Parsers/RequestParser.h"
// utils imports
#include "widgets/utils/GlobalAppSettings.h"
#include "networking/things.h"

/*
	This file contains ModeSelectionWidget - which is non-branched widget remaining from old style 
	application. It does it's work, but it is really similar with SelectItemFromList.

*/

class ModeSelectionWidget : public inframedWidget	//	root of modeSelection branch
{
	Q_OBJECT
protected:
	// modes list
	PseudotableEntityModel* innerModel;

	QVBoxLayout* mainLayout;
	QHBoxLayout* buttonLayout;
	QLabel* modesTip;
	QTableView* modeSelection;
	QLabel* placeTip;
	MegaIconButton* logoutButton;

	// this list is used to store data obtained after select_mode web request. it will be usefull later
	QHash<QString, QString> settings;
	//	here upcasted mode awaits end of request
	Mode selected;

	RequestAwaiter awaiter;
public:
	ModeSelectionWidget(QWidget* parent = Q_NULLPTR);
	virtual void _numberReaction(int) override;
	virtual void _arrowReaction(int) override;
	virtual void _returnReaction() override;
	virtual void show();
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