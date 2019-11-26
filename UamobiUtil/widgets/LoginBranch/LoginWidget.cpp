#include "LoginWidget.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
void LoginWidget::login_confirmed()
{
	using parse_uniresults_functions::TypicalResponse;

	if (awaiter.isAwaiting()) return;
	if (loginField->text().isEmpty() || passwordField->text().isEmpty())
	{
		info->setText(tr("login_widget_no_password!"));
		return;
	}
	showProcessingOverlay();
	globalSettings.networkingEngine->userLogIn(loginField->text(), passwordField->text(), &awaiter, RECEIVER_SLOT_NAME, VERSION);
	awaiter.run();
}

void LoginWidget::was_timeout()
{
	hideProcessingOverlay();
	info->setText(tr("login_widget_connection_timeout:") + QString::number(globalSettings.timeoutInt));
}

void LoginWidget::checkResponse()
{
	using namespace parse_uniresults_functions;
	if (awaiter.wasTimeout())
		return;
	TypicalResponse resp = RequestParser::interpretAsLoginResponse(awaiter.restext, awaiter.errtext);
	if (resp.resp == true)
	{
		emit loginConfirmed(loginField->text(), passwordField->text());
	}
	else
	{
		info->setText(resp.errors);
	}
	hideProcessingOverlay();
}

LoginWidget::LoginWidget(GlobalAppSettings& go, QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)), loginInfo(new QLabel(this)),
	passwordInfo(new QLabel(this)), info(new QLabel(this)), loginField(new QLineEdit(this)),
	passwordField(new QLineEdit(this)), buttonPanel(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this)), globalSettings(go), awaiter(go.timeoutInt, this)
{
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

#ifdef DEBUG
	detrace_DCONSTR("LoginWidget");
	detrace_METHEXPL("size: " << this->size().height());
#endif
	this->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
	loginInfo->setText(tr("login_widget_login_tip"));

	QFont scf = makeFont(0.04);
	loginInfo->setFont(scf);
	passwordInfo->setFont(scf);
	passwordInfo->setText(tr("login_widget_password_tip"));
	okButton->setText(tr("login_widget_ok_button"));
	backButton->setText(tr("login_widget_back_button"));
	info->setStyleSheet(ERROR_TEXT_STYLESHEET);
	info->setAlignment(Qt::AlignCenter);
	info->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum));
	passwordField->setEchoMode(QLineEdit::Password);
	loginField->setFont(scf);
	passwordField->setFont(scf);
	okButton->setIcon(QIcon(":/res/submit.png"));
	backButton->setIcon(QIcon(":/res/back.png"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
#ifdef QT_VERSION5X
	QObject::connect(okButton, &QPushButton::clicked, this, &LoginWidget::login_confirmed);
	QObject::connect(backButton, &QPushButton::clicked, this, &LoginWidget::backRequired);
	QObject::connect(loginField, &QLineEdit::returnPressed, passwordField, QOverload<>::of(&QLineEdit::setFocus));
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

void LoginWidget::set_login(const QString str)
{
	loginField->setText(str);
	passwordField->clear();
}