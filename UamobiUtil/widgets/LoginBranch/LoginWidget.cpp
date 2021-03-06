#include "LoginWidget.h"
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/ExtendedDialogs.h"

void LoginWidget::login_confirmed()
{
	if (awaiter.isAwaiting()) return;

	if (loginField->text().isEmpty() || passwordField->text().isEmpty())
	{
		info->setText(tr("login_widget_no_password!"));
		return;
	}
	showProcessingOverlay();
	userToSend = User(new UserEntity("", loginField->text()));
	userToSend->sendLoginRequest(passwordField->text(), &awaiter);
}

void LoginWidget::was_timeout()
{
	hideProcessingOverlay();
	info->setText(tr("login_widget_connection_timeout:") + QString::number(AppSettings->timeoutInt));
}

void LoginWidget::checkResponse()
{
	if (awaiter.wasTimeout())
		return;
	QString result = AppNetwork->setSession(awaiter.restext);
	if (AppNetwork->sessionReady())
	{
		emit loginConfirmed(userToSend->login, passwordField->text());
	}
	else
	{
		ErrorMessageDialog::showErrorInfo(tr("Error!"), result);
#ifdef DEBUG
		detrace_NRESPERR(result);
#endif
	}
	hideProcessingOverlay();
}

void LoginWidget::set_user(const User u)
{
	userToSend = User( upcastRecord<UserEntity>(u->clone()));
	loginField->setText(userToSend->login);
	passwordField->clear();
}

void LoginWidget::focusInEvent(QFocusEvent *ev)
{
    inframedWidget::focusInEvent(ev);
    loginField->setFocus();
#ifdef Q_OS_ANDROID
    qApp->inputMethod()->show();
#endif
}

LoginWidget::LoginWidget( QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)), loginInfo(new QLabel(this)),
	passwordInfo(new QLabel(this)), info(new QLabel(this)), loginField(new QLineEdit(this)),
	passwordField(new QLineEdit(this)), buttonPanel(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this)), awaiter(AppSettings->timeoutInt, this), userToSend(new UserEntity())
{

#ifdef DEBUG
	detrace_DCONSTR("LoginWidget");
#endif

	this->setLayout(mainLayout);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addStretch();
	mainLayout->addWidget(loginInfo);
	mainLayout->addWidget(loginField);
	mainLayout->addWidget(info);

	mainLayout->addWidget(passwordInfo);
	mainLayout->addWidget(passwordField);
	mainLayout->addStretch();
	mainLayout->addLayout(buttonPanel);
	buttonPanel->addWidget(okButton);
	buttonPanel->addWidget(backButton);


	this->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	loginInfo->setText(tr("login_widget_login_tip"));

	loginInfo->setFont(GENERAL_FONT);
	passwordInfo->setFont(GENERAL_FONT);
	passwordInfo->setText(tr("login_widget_password_tip"));
	okButton->setText(tr("login_widget_ok_button"));
	backButton->setText(tr("login_widget_back_button"));
	info->setStyleSheet(ERROR_TEXT_STYLESHEET);
	info->setAlignment(Qt::AlignCenter);
	info->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum));
	passwordField->setEchoMode(QLineEdit::Password);
	loginField->setFont(GENERAL_FONT);
	passwordField->setFont(GENERAL_FONT);
	okButton->setIcon(QIcon(":/resources/submit"));
	backButton->setIcon(QIcon(":/resources/back"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
#ifdef LINELLA
	passwordField->setInputMethodHints(Qt::ImhPreferNumbers);
	loginField->setInputMethodHints(Qt::ImhPreferNumbers);
#endif
#ifdef QT_VERSION5X
	QObject::connect(okButton, &QPushButton::clicked, this, &LoginWidget::login_confirmed);
	QObject::connect(backButton, &QPushButton::clicked, this, &LoginWidget::backRequired);
	QObject::connect(loginField, &QLineEdit::returnPressed, passwordField, QOverload<>::of(&QLineEdit::setFocus));
	QObject::connect(loginField, &QLineEdit::returnPressed, qApp->inputMethod(), &QInputMethod::show);
	QObject::connect(passwordField, &QLineEdit::returnPressed, this, &LoginWidget::login_confirmed);
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &LoginWidget::checkResponse);
	QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &LoginWidget::was_timeout);
#else
	QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(login_confirmed()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(loginField, SIGNAL(returnPressed()), passwordField, SLOT(setFocus()));
	QObject::connect(passwordField, SIGNAL(returnPressed()), this, SLOT(login_confirmed()));
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(checkResponse()));
	QObject::connect(&awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
#endif
}

void LoginWidget::show()
{
	inframedWidget::show();
	if (loginField->text().isEmpty())
	{
		loginField->setFocus();
	}
	else
	{
		passwordField->setFocus();
	}
}

void LoginWidget::langCh()
{
	loginInfo->setText(tr("login_widget_login_tip"));
	passwordInfo->setText(tr("login_widget_password_tip"));
	okButton->setText(tr("login_widget_ok_button"));
	backButton->setText(tr("login_widget_back_button"));
	
}
