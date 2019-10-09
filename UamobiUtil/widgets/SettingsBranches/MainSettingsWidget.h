#pragma once
// qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QGroupBox>
#else
// Qt 4 only imports
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>
#include <QtGui/QBoxLayout>
#include <QtGui/QComboBox>
#include <QtGui/QTabWidget>
#include <QtGui/QScrollArea>
#include <QtGui/QGroupBox>
#endif

// widgets imports
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/ElementsStyles.h"

/*
	This widgets is used to alter GlobalAppSettings object. Generally it allows to change
	scanning mode, adress, type of storage and language.

*/

class MainSettingsWidget : public inframedWidget
{
	Q_OBJECT
private:
	GlobalAppSettings& globalSettings;

	QVBoxLayout* mainLayout;
	QTabWidget* innerWidget;

	// workflow tab with contents
	QWidget* wrkflTab;
	QVBoxLayout* wrkflinnLayout;
	QScrollArea* wrkflScrollArea;
	QWidget* wrkflContents;
	QVBoxLayout* wrkflContLayout;
	QComboBox* scanModeSelector;
	QLabel* scanModeInfo;

	// systab without contents
	QWidget* sysTab;
	QVBoxLayout* sysinnLayout;
	QScrollArea* sysScrollArea;
	QWidget* sysContents;
	QVBoxLayout* sysContLayout;
	QLabel* topExplLabel;

	// group of dataengine changes
	QGroupBox* dataengGroup;
	QVBoxLayout* dataengLayout;
	QLabel* dataengInfo;
	QCheckBox* httpCheckBox;
	QCheckBox* memcheckbox;

	// group of connection changes
	QGroupBox* connectionGroup;
	QVBoxLayout* connectionLayout;
	QLabel* connectionInfo;
	QComboBox* addressField;

	// group of language changes
	QGroupBox* langGroup;
	QVBoxLayout* langLayout;
	QLabel* langInfo;
	QComboBox* langField;

	// buttons in widget footer
	QPushButton* saveButton;
	QPushButton* backButton;

public:
	MainSettingsWidget(GlobalAppSettings& go, QWidget* parent = Q_NULLPTR);

private slots:;
			 void saveClicked();			//	saves data to GlobalAppSettings widget
			 void langSelected(const QString&);	//	alters GAS, then retranslates this widget
			 void langChanged();				//	retranslates this widget
signals:
	void languageChanged();			//	is emitted when language is changed
};