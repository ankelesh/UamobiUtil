#pragma once
// qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QGroupBox>
#else
// Qt 4 only imports
#include <QtGui/QLabel>
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
#include "widgets/ElementWidgets/MegaIconButton.h"
/*
	This widgets is used to alter GlobalAppSettings object. Generally it allows to change
	scanning mode, adress, type of storage and language.

	// update:
	now this widget is reworked to match classic mobile architecture

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

	QComboBox* scanModeSelector;
	QLabel* scanModeInfo;

	// systab without contents
	QWidget* sysTab;
	QVBoxLayout* sysinnLayout;
	QLabel* topExplLabel;

	// group of connection changes
	QLabel* connectionInfo;
	QComboBox* addressField;

	// group of language changes
	QLabel* langInfo;
	QComboBox* langField;

	// buttons in widget footer
	QHBoxLayout* footerLayout;
	MegaIconButton* saveButton;
	MegaIconButton* backButton;

public:
	MainSettingsWidget(GlobalAppSettings& go, QWidget* parent = Q_NULLPTR);
	virtual void show() override;
private slots:
	void saveClicked();			//	saves data to GlobalAppSettings widget
	void langSelected(const QString&);	//	alters GAS, then retranslates this widget
	void langChanged();				//	retranslates this widget
	void AddressSelected(const QString& activated);
signals:
	void languageChanged();			//	is emitted when language is changed
	void saveConfirmed();
};
