#include "LoginWidget.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "widgets/utils/ElementsStyles.h"

void LoginWidget::login_confirmed()
{
	using parse_uniresults_functions::TypicalResponse;

	if (awaiter.isAwaiting()) return;
	if (loginField->text().isEmpty() || passwordField->text().isEmpty())
	{
		info->setText(tr("login_widget_no_password!"));
		return;
	}
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
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)), loginInfo(new QLabel(this)),
	passwordInfo(new QLabel(this)), info(new QLabel(this)), loginField(new QLineEdit(this)),
	passwordField(new QLineEdit(this)), buttonPanel(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this)), globalSettings(go)
{
	this->setLayout(mainLayout);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(loginInfo);
	mainLayout->addWidget(loginField);
	mainLayout->addWidget(info);

	mainLayout->addWidget(passwordInfo);
	mainLayout->addWidget(passwordField);
	mainLayout->addLayout(buttonPanel);
	buttonPanel->addWidget(okButton);
	buttonPanel->addWidget(backButton);

#ifdef DEBUG
	detrace_DCONSTR("LoginWidget");
	detrace_METHEXPL("size: " << this->size().height());
#endif
	this->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	loginInfo->setText(tr("login_widget_login_tip"));
	loginInfo->setStyleSheet(countAdaptiveFont(0.05));
	passwordInfo->setStyleSheet(countAdaptiveFont(0.05));
	passwordInfo->setText(tr("login_widget_password_tip"));
	okButton->setText(tr("login_widget_ok_button"));
	backButton->setText(tr("login_widget_back_button"));
	info->setStyleSheet(ERROR_TEXT_STYLESHEET);
	info->setAlignment(Qt::AlignCenter);
	info->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum));
	passwordField->setEchoMode(QLineEdit::Password);
	loginField->setStyleSheet(countAdaptiveFont(0.04));
	passwordField->setStyleSheet(countAdaptiveFont(0.04));
	okButton->setIcon(QIcon(":/res/submit.png"));
	backButton->setIcon(QIcon(":/res/back.png"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
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