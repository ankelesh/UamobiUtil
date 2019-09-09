#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#else
// Qt 4 only imports
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QGridLayout>
#endif

// widgets imports
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/GlobalAppSettings.h"
// networking imports
#include "networking/things.h"
#include "networking/RequestAwaiter.h"
#include "networking/Parsers/RequestParser.h"
// util imports
#include "debugtrace.h"
#include "widgets/utils/ElementsStyles.h"

/*
	This widget is the simpliest one-screen form for logging in. It sends to server obtained result
	and if it was right - loginConfirmed is emitted.
		set_login is used to setup loginField before widget is shown

*/


class LoginWidget : public inframedWidget
{
	Q_OBJECT
private:
	QGridLayout* mainLayout;
	QLabel* loginInfo;
	QLabel* passwordInfo;
	QLabel* info;
	QLineEdit* loginField;
	QLineEdit* passwordField;
	QPushButton* backButton;
	QPushButton* okButton;
	
	GlobalAppSettings & globalSettings;
public:
	LoginWidget(GlobalAppSettings& go, QWidget* parent);
private slots:
	void login_confirmed();
public slots:
	void set_login(const QString);
signals:
	void loginConfirmed(const QString, const QString);
};