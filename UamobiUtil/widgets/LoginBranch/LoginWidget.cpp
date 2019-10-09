#include "LoginWidget.h"

void LoginWidget::login_confirmed()
{
	using parse_uniresults_functions::TypicalResponse;
	if (loginField->text().isEmpty() || passwordField->text().isEmpty())
	{
		info->setText(tr("login_widget_no_password!"));
		return;
	}
	RequestAwaiter awaiter;
	globalSettings.networkingEngine->userLogIn(loginField->text(), passwordField->text(), &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	while (awaiter.isAwaiting()) { qApp->processEvents(); }
	if (awaiter.wasTimeout())
	{
		info->setText(tr("login_widget_connection_timeout"));
		return;
	}
	TypicalResponse resp = RequestParser::interpretAsLoginResponse(awaiter.restext, awaiter.errtext);
	if (resp.resp == true)
	{
		emit loginConfirmed(loginField->text(), passwordField->text());
	}
	else
	{
		info->setText(resp.errors);
	}
}

LoginWidget::LoginWidget(GlobalAppSettings& go, QWidget* parent)
	: inframedWidget(parent), mainLayout(new QGridLayout(this)), loginInfo(new QLabel(this)),
	passwordInfo(new QLabel(this)), info(new QLabel(this)), loginField(new QLineEdit(this)),
	passwordField(new QLineEdit(this)), backButton(new QPushButton(this)),
	okButton(new QPushButton(this)), globalSettings(go)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(loginInfo, 1, 0);
	mainLayout->addWidget(loginField, 1, 1, 1, -1);
	mainLayout->addWidget(info, 0, 0, 1, -1);

	mainLayout->addWidget(passwordInfo, 2, 0);
	mainLayout->addWidget(passwordField, 2, 1, 1, -1);
	mainLayout->addWidget(okButton, 5, 3);
	mainLayout->addWidget(backButton, 5, 0);

#ifdef DEBUG
	detrace_DCONSTR("LoginWidget");
	detrace_METHEXPL("size: " << this->size().height());
#endif
	this->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	loginInfo->setText(tr("login_widget_login_tip"));
	passwordInfo->setText(tr("login_widget_password_tip"));
	okButton->setText(tr("login_widget_ok_button"));
	backButton->setText(tr("login_widget_back_button"));
	info->setStyleSheet(ERROR_TEXT_STYLESHEET);
	info->setAlignment(Qt::AlignCenter);
	info->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum));
	passwordField->setEchoMode(QLineEdit::Password);

#ifdef QT_VERSION5X
	QObject::connect(okButton, &QPushButton::clicked, this, &LoginWidget::login_confirmed);
	QObject::connect(backButton, &QPushButton::clicked, this, &LoginWidget::backRequired);
	QObject::connect(loginField, &QLineEdit::returnPressed, passwordField, QOverload<>::of(&QLineEdit::setFocus));
	QObject::connect(passwordField, &QLineEdit::returnPressed, this, &LoginWidget::login_confirmed);
#else
	QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(login_confirmed()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(loginField, SIGNAL(returnPressed()), passwordField, SLOT(setFocus()));
	QObject::connect(passwordField, SIGNAL(returnPressed()), this, SLOT(login_confirmed()));
#endif
}

void LoginWidget::set_login(const QString str)
{
	loginField->setText(str);
	passwordField->clear();
}