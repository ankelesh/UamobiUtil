#pragma once

// include here everything qt-5 only
#ifdef QT_VERSION5X
#include <QtWidgets/QLabel>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
// include here legacy-versions of top ones
#else
#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#endif
// include here version-independents
#include <QtCore/QVector>
#include <QtCore/QPointer>

// fixed includes
#include "widgets/LoginBranch/LoginWidget.h"
#include "widgets/LoginBranch/LoginSelectWidget.h"
#include "widgets/SettingsBranches/MainSettingsWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "networking/RequestAwaiter.h"
#include "networking/Parsers/RequestParser.h"

/*
	This widget is shown on application launch. It's main functional is to allow user profile selection if
	server returns any user profiles. In worst case, this widget automatically shows manual login widget.
	Also this widget contains Settings widget, which is affecting GlobalAppSettings.

	__ASSOCIATED_DATABASE_FUNCTION__   : P'UserProfilesResult' list_users(void)

	Update:
		now more intellegent hiding - > returns to modeselect if user already logged in


*/

class MainPageWidget : public inframedWidget, abstractNode
{
	Q_OBJECT
private:
	// Uses global settings
	GlobalAppSettings& globalSettings;
	QVector<UserProfile> profiles;

	// has own view while is root
	QVBoxLayout* mainLayout;
	QPointer<inframedWidget> innerWidget;
	QVBoxLayout* innerLayout;
	QHBoxLayout* topPanelLayout;
	QHBoxLayout* bottomPanelLayout;
	QLabel* versionLabel;
	QLabel* hostLabel;
	QScrollArea* scrArea;
	QLabel* userHelpLabel;
	LoginSelectWidget* loginsStorageWidget;
	QLabel* userIdInfo;
	QPushButton* exitButton;
	QPushButton* settingsButton;
	QLineEdit* userid;

	// Child widgets
	LoginWidget* manualLogin;
	MainSettingsWidget* settingsScreen;

	void show_login_widget(QString& log); // Utility: hides inner, shows login widget, sets it up.

public:
	MainPageWidget(GlobalAppSettings& go, QWidget* parent);

private slots:
	void settinsPressed();					//	activated on button press
	void userIdOk(const QString  log, const QString  pass);	//	activated when user logged in
	void userPicked(UserProfile);					//	activated when item was selected on LoginSelectWidget
	void hideCurrent();							//	hides current
	void languageChanged();						//	activated when language changed - retranslates text
public slots:
	void loadUsers();							//	sends request to server, fills LoginSelectWidget
signals:
	void loggedIn();							//	emitted when this user logged in and this branch can be closed
};
