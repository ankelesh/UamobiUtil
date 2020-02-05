#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QBoxLayout>
#else
// Qt 4 only imports
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>
#endif

// widgets imports
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
// networking imports
#include "networking/things.h"
#include "networking/RequestAwaiter.h"
#include "networking/Parsers/RequestParser.h"
// util imports

/*
	This widget is the simpliest one-screen form for logging in. It sends to server obtained result
	and if it was right - loginConfirmed is emitted.
		set_login is used to setup loginField before widget is shown
		loginConfirmed is emitted after logging in confirmed

		__ASSOCIATED_DATABASE_FUNCTION__  :  P'LoginResponse'  log_in(login, password)
*/

class LoginWidget : public inframedWidget
{
	Q_OBJECT
private:
	QVBoxLayout* mainLayout;
	QLabel* loginInfo;
	QLabel* passwordInfo;
	QLabel* info;
	QLineEdit* loginField;
	QLineEdit* passwordField;
	QHBoxLayout* buttonPanel;
	MegaIconButton* backButton;
	MegaIconButton* okButton;

	RequestAwaiter awaiter;
	
	User userToSend;
public:
	LoginWidget(QWidget* parent);

	virtual void show() override;
	void langCh();
private slots:
	void login_confirmed();
	void was_timeout();
	void checkResponse();
public slots:
	void set_user(const User);
signals:
	void loginConfirmed(const QString, const QString);
};