#include "MainPageWidget.h"



void MainPageWidget::show_login_widget(QString & log)
{
	current->hide();
	current = manualLogin;
	manualLogin->set_login(log);
	current->show();
}

MainPageWidget::MainPageWidget(GlobalAppSettings& go, QWidget* parent)
	: inframedWidget(parent), globalSettings(go), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	topPanelLayout(new QHBoxLayout(innerWidget)), bottomPanelLayout(new QHBoxLayout(innerWidget)),
	versionLabel(new QLabel(innerWidget)), hostLabel(new QLabel(innerWidget)),
	scrArea(new QScrollArea(innerWidget)),
	userHelpLabel(new QLabel(innerWidget)), loginsStorageWidget(new LoginSelectWidget(profiles, scrArea)), userIdInfo(new QLabel(innerWidget)),
	exitButton(new QPushButton(innerWidget)), settingsButton(new QPushButton(innerWidget)),
	userid(new QLineEdit(innerWidget)), manualLogin(new LoginWidget(go, this)), 
	settingsScreen(new MainSettingsWidget(go, this)),  current(innerWidget)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	mainLayout->addWidget(settingsScreen);
	mainLayout->addWidget(manualLogin);
	manualLogin->hide();
	settingsScreen->hide();
	mainLayout->setMargin(0); 
	mainLayout->setSpacing(0);		//	spacing removed to avoid space loss
	mainLayout->setContentsMargins(0, 0, 0, 0);

	innerLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	
	innerWidget->setLayout(innerLayout);
	innerLayout->addLayout(topPanelLayout);
	innerLayout->addWidget(userHelpLabel);


	scrArea->setWidgetResizable(true);
	innerLayout->addWidget(scrArea);
	innerLayout->addWidget(userIdInfo);

	innerLayout->addLayout(bottomPanelLayout);

	topPanelLayout->addWidget(versionLabel);
	topPanelLayout->addStretch();
	topPanelLayout->addWidget(hostLabel);

	bottomPanelLayout->addWidget(exitButton);
	bottomPanelLayout->addWidget(userid);
	bottomPanelLayout->addWidget(settingsButton);
	
	versionLabel->setText(QString::number(VERSION));
	hostLabel->setText(globalSettings.HttpUrl.section("/", 2, 2));

	userHelpLabel->setText(tr("main_page_select_profile_tip"));
	userIdInfo->setText(tr("main_page_enter_profile_id_tip"));
	userHelpLabel->setAlignment(Qt::AlignCenter);
	userIdInfo->setAlignment(Qt::AlignCenter);

	exitButton->setText(tr("main_page_exit_button"));
	settingsButton->setText(tr("main_page_settings_button"));


	loadUsers();

#ifdef QT_VERSION5X
	QObject::connect(settingsButton, &QPushButton::clicked, this, &MainPageWidget::settinsPressed);
	QObject::connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
	QObject::connect(manualLogin, &LoginWidget::loginConfirmed, this, &MainPageWidget::userIdOk);
	QObject::connect(manualLogin, &LoginWidget::backRequired, this, &MainPageWidget::hideCurrent);
	QObject::connect(settingsScreen, &MainSettingsWidget::backRequired, this, &MainPageWidget::hideCurrent);
	QObject::connect(settingsScreen, &MainSettingsWidget::languageChanged, this, &MainPageWidget::languageChanged);
	QObject::connect(loginsStorageWidget, &LoginSelectWidget::profilePicked, this, &MainPageWidget::userPicked);
#else
    QObject::connect(settingsButton, SIGNAL(clicked()), this, SLOT(settinsPressed()));
    QObject::connect(exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
    QObject::connect(manualLogin, SIGNAL(loginConfirmed(QString,QString)), this, SLOT(userIdOk(QString,QString)));
    QObject::connect(manualLogin, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
    QObject::connect(settingsScreen, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
    QObject::connect(settingsScreen, SIGNAL(languageChanged()), this, SLOT(languageChanged()));
    QObject::connect(loginsStorageWidget, SIGNAL(profilePicked(UserProfile)), this, SLOT(userPicked(UserProfile)));
#endif
	scrArea->setWidget(loginsStorageWidget);

}

void MainPageWidget::settinsPressed()
{
	current->hide();
	current = settingsScreen;
	current->show();
}


void MainPageWidget::userIdOk(const QString log,const  QString pass)
{
	hideCurrent();
	emit loggedIn();
}


void MainPageWidget::userPicked(UserProfile up)
{
	show_login_widget(up.login);
}

void MainPageWidget::hideCurrent()
{
	if (current == innerWidget)
	{
		emit backRequired();
	}
	else
	{
		current->hide();
		innerWidget->show();
		current = innerWidget;
	}
}

void MainPageWidget::languageChanged()
{
	userHelpLabel->setText(tr("main_page_select_profile_tip"));
	userIdInfo->setText(tr("main_page_enter_profile_id_tip"));
	exitButton->setText(tr("main_page_exit_button"));
	settingsButton->setText(tr("main_page_settings_button"));
}

void MainPageWidget::loadUsers()
{
	RequestAwaiter awaiter;
	globalSettings.networkingEngine->userUpdateList(&awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	while (awaiter.isAwaiting())
	{
		qApp->processEvents();
	}
	if (awaiter.wasTimeout())
	{
		detrace_METHEXPL("Timeout found")
			show_login_widget(QString(""));
	}
	else
	{
		parse_uniresults_functions::UserProfilesResult temp = RequestParser::interpretAsLogin(awaiter.restext, awaiter.errtext);
		if (temp.manually)
		{
			detrace_METHEXPL("manually parsed from request")
				show_login_widget(QString(""));
		}
		else
		{
			profiles = temp.profiles;
			loginsStorageWidget->reload();
		}
	}
}
