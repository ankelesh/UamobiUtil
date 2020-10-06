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
#include <QFormLayout>
#include <QSpinBox>
// widgets imports
#include "widgets/parents/inframedWidget.h"
#include "widgets/ElementWidgets/MegaIconButton.h"

/*
	This widgets is used to alter GlobalAppSettings object. Generally it allows to change
	any variable in app settings, but separates them into different tabs. Dumps settings after
	pressing ok.

*/

class MainSettingsWidget : public inframedWidget
{
	Q_OBJECT
private:

	QVBoxLayout* mainLayout;
	QTabWidget* innerWidget;

	// workflow tab with contents
	QWidget* wrkflTab;
	QFormLayout* wrkflinnLayout;

	QComboBox* scanModeSelector;
	QSpinBox* fontMin;
	QSpinBox* fontMax;
	QSpinBox* fontDec;
	// systab without contents
	QWidget* sysTab;
	QFormLayout* sysinnLayout;
	QLabel* topExplLabel;

	// group of connection changes
	QComboBox* addressField;

	// group of language changes
	QComboBox* langField;
	MegaIconButton* createSTableButton;

	QWidget* scaningTab;
	QFormLayout* scaninnlayout;

	QSpinBox* prefix;
	QSpinBox* suffix;
	
	// printing tab
	QWidget* printTab;
	QFormLayout* printinnLayout;
	QLabel* buildState;
	QComboBox* portDesignation;
	QSpinBox* portNumber;
	QComboBox* portType;
	QLineEdit* btDeviceName;
	// buttons in widget footer
	QHBoxLayout* footerLayout;
	MegaIconButton* saveButton;
	MegaIconButton* backButton;

public:
	MainSettingsWidget(QWidget* parent = Q_NULLPTR);
private slots:
	void saveClicked();			//	saves data to settings.ini
	void langSelected(const QString&);	//	alters GAS, then retranslates this widget
	void langChanged();				//	retranslates this widget
	void AddressSelected(const QString& activated); // changes url without reconnection
	void setPSLabels(int);			//	updates prefix and suffix
	void applyFonts();				//	applies fonts to FontAdapter. but results will be only after reboot
	void createStyleTable();
signals:
	void languageChanged();			//	is emitted when language is changed
	void saveConfirmed();			//	emitted when user confirms data
};
