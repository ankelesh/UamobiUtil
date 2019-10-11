#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QLabel>
#else
// Qt 4 only imports
#include <QtGui/QVBoxLayout>
#include <QtGui/QScrollArea>
#include <QtGui/QLabel>
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

/*
	This file contains ModeSelectionWidget - which is root of ModeSelection branch. It loads
	Modes list from web, then allows user to choose one. PlaceSelection is part of this branch, because it must be chosed before
	other operations will continue.
	
	__ASSOCIATED_DATABASE_FUNCTION__  :  P'modesResponse' list_modes()
	__ASSOCIATED_DATABASE_FUNCTION__  :  P'PositionalResponse' select_mode (mode, submode)

*/

using parse_uniresults_functions::modesResponse;
namespace specwidgets {
	// This namespace must contain transformed base widgets with minor changes and specializations of abstract widgets
	class _modeSelectionWidget : public AbstractVariantSelectionWidget	//	This class wraps Modes vector
	{
		Q_OBJECT
	private:
		modesResponse& modes;
	protected:
		// Inherited via AbstractVariantSelectionWidget
		virtual QString elemAsString(int index) override;
		virtual int countElems() override;
	public:
		_modeSelectionWidget(modesResponse& Modes, QWidget* parent);
	private slots:
		virtual void indexSelected(int Index);
	signals:
		void modeSelected(parsedMode);
	};
}

class ModeSelectionWidget : public inframedWidget	//	root of modeSelection branch
{
	Q_OBJECT
protected:
	// uses GAS
	const GlobalAppSettings& globalSettings;
	// modes list
	modesResponse allmodes;

	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QHBoxLayout* buttonLayout;
	QScrollArea* scrArea;
	QLabel* userTip;
	QLabel* modesTip;
	specwidgets::_modeSelectionWidget* modeSelection;
	MegaIconButton* logoutButton;

	// this list is used to store data obtained after select_mode web request. it will be usefull later
	QHash<QString, QString> settings;
	parsedMode selected;
	RequestAwaiter awaiter;
public:
	ModeSelectionWidget(const GlobalAppSettings& go, QWidget* parent = Q_NULLPTR);
	virtual bool back() override;	//	has top widget
	virtual bool isExpectingControl(int) override;
	virtual void show() override;
protected slots:
	void logoutPressed();		//	this slot sends log_out and only then calls backRequired
	virtual void modeSelected(parsedMode);	//	these slots react to changes
	virtual void hideCurrent();
public slots:
	void loadModes();				//	loads modes list from web
signals:
	void modeAcquired(QHash<QString, QString>, parsedMode pm);		//	is emitted when this branch is done
};
class ModeBranchRootWidget : public ModeSelectionWidget, abstractNode
{
	Q_OBJECT
protected:
	// child nodes
	PlaceSelectionWidget* placeSelection;
public:
	ModeBranchRootWidget(const GlobalAppSettings& go, QWidget* parent = Q_NULLPTR);
protected slots:
	virtual void modeSelected(parsedMode) override;
	void placeAcquired(parsedPlace);
	virtual void hideCurrent() override;
};