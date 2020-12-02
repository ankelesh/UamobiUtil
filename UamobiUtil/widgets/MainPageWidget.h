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
#include "networking/things.h"
#include "widgets/LoginBranch/LoginWidget.h"
#include "widgets/SettingsBranches/MainSettingsWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
#include "networking/RequestAwaiter.h"
/*
	This widget is shown on application launch. It's main functional is to allow user profile selection if
	server returns any user profiles. In worst case, this widget automatically shows manual login widget.
	Also this widget contains Settings widget, which is affecting GlobalAppSettings.


	This widget can not be created via node factory, it is remaining from old app.
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
	virtual void _numberReaction(int) override;

private slots:
	void settinsPressed();					//	activated on button press
	void userIdOk(const QString  log, const QString  pass);	//	activated when user logged in
	void userPicked(RecEntity);					//	activated when item was selected on LoginSelectWidget
	void hideCurrent();							//	hides current
	void languageChanged();						//	activated when language changed - retranslates text
	void userIdSearch();						//	searches and activates item in model by name or id
	void parseUsers();							// receives response and inserts it in the model
	void wasTimeout();							
	void settingsSaved();						// reloads user and refreshes labels
public slots:
	void loadUsers();							//	sends request to server, fills LoginSelectWidget
signals:
	void loggedIn();							//	emitted when this user logged in and this branch can be closed
};
