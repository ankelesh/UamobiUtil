#pragma once

// include here everything qt-5 only
#ifdef QT_VERSION5X
#include <QtWidgets/QLabel>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
// include here legacy-versions of top ones
#else
#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>
#include <QtGui/QListView>
#include <QtGui/QLineEdit>
#endif
// include here version-independents
#include <QtCore/QVector>
#include <QtCore/QPointer>

// fixed includes
#include "networking/things.h"
#include "widgets/LoginBranch/LoginWidget.h"
#include "widgets/SettingsBranches/MainSettingsWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
#include "widgets/parents/AbstractVariantSelectionWidget.h"
#include "networking/RequestAwaiter.h"
#include "networking/Parsers/RequestParser.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
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
	DataEntityListModel* innerModel;

	// has own view while is root
	QVBoxLayout* mainLayout;
	QPointer<inframedWidget> innerWidget;
	QVBoxLayout* innerLayout;
	QHBoxLayout* topPanelLayout;
	QHBoxLayout* bottomPanelLayout;
	QLabel* versionLabel;
	QLabel* hostLabel;
	QLabel* userHelpLabel;
	QListView* loginsView;
	QLabel* userIdInfo;
	MegaIconButton* exitButton;
	MegaIconButton* settingsButton;
	MegaIconButton* refreshButton;
	QLineEdit* userid;
	// Child widgets
	LoginWidget* manualLogin;
	MainSettingsWidget* settingsScreen;



	RequestAwaiter awaiter;

	void show_login_widget(User user); // Utility: hides inner, shows login widget, sets it up.
public:
	MainPageWidget( QWidget* parent);
	virtual bool isExpectingControl(int) override;

private slots:
	void settinsPressed();					//	activated on button press
	void userIdOk(const QString  log, const QString  pass);	//	activated when user logged in
	void userPicked(RecEntity);					//	activated when item was selected on LoginSelectWidget
	void hideCurrent();							//	hides current
	void languageChanged();						//	activated when language changed - retranslates text
	void userIdSearch();
	void parseUsers();
	void wasTimeout();
	void settingsSaved();
public slots:
	void loadUsers();							//	sends request to server, fills LoginSelectWidget
signals:
	void loggedIn();							//	emitted when this user logged in and this branch can be closed
};
