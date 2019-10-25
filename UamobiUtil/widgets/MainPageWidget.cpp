#include "MainPageWidget.h"
#ifdef QT_VERSION5X
#include <QtWidgets/QScroller>
#else
#include "legacy/qtCompatibility/scrollgrabber.h"
#endif
#include "widgets/utils/ElementsStyles.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif


void MainPageWidget::show_login_widget(QString& log)
{
	manualLogin->set_login(log);
	_hideAny(manualLogin);
}

void MainPageWidget::resizeEvent(QResizeEvent* rev)
{
	inframedWidget::resizeEvent(rev);
}

MainPageWidget::MainPageWidget(GlobalAppSettings& go, QWidget* parent)
	: inframedWidget(parent), abstractNode(), globalSettings(go), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	topPanelLayout(new QHBoxLayout(innerWidget)), bottomPanelLayout(new QHBoxLayout(innerWidget)),
	versionLabel(new QLabel(innerWidget)), hostLabel(new QLabel(innerWidget)),
	scrArea(new QScrollArea(innerWidget)),
	userHelpLabel(new QLabel(innerWidget)), loginsStorageWidget(new LoginSelectWidget(profiles, scrArea)), userIdInfo(new QLabel(innerWidget)),
    exitButton(new MegaIconButton(innerWidget)), settingsButton(new MegaIconButton(innerWidget)), refreshButton(new MegaIconButton(innerWidget)),
	userid(new QLineEdit(innerWidget)),  manualLogin(new LoginWidget(go, this)),
    settingsScreen(new MainSettingsWidget(go, this)), awaiter(go.timeoutInt, this)
{
	current = innerWidget;
	untouchable = innerWidget;
	main = this;
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
	innerLayout->addWidget(userid);

	innerLayout->addLayout(bottomPanelLayout);

	topPanelLayout->addWidget(versionLabel);
	topPanelLayout->addStretch();
	topPanelLayout->addWidget(hostLabel);
	bottomPanelLayout->addWidget(exitButton);
	bottomPanelLayout->addWidget(settingsButton);
    bottomPanelLayout->addWidget(refreshButton);

    QFont scf = makeFont(0.04);
	versionLabel->setText(QString::number(VERSION) + " " + SUFFIX);
    versionLabel->setFont(scf);
	hostLabel->setText(globalSettings.HttpUrl.section("/", 4, 4));
    hostLabel->setFont(scf);

	userHelpLabel->setText(tr("main_page_select_profile_tip"));
    userHelpLabel->setFont(scf);
	userIdInfo->setText(tr("main_page_enter_profile_id_tip"));
    userIdInfo->setFont(scf);
	userHelpLabel->setAlignment(Qt::AlignCenter);
	userIdInfo->setAlignment(Qt::AlignCenter);

	exitButton->setText(tr("main_page_exit_button"));
	exitButton->setIcon(QIcon(":/res/exit.png"));
    exitButton->setStyleSheet(CANCEL_BUTTONS_STYLESHEET);
    exitButton->setFont(scf);
	settingsButton->setText(tr("main_page_settings_button"));
	settingsButton->setIcon(QIcon(":/res/settings.png"));
	settingsButton->setStyleSheet(SETTINGS_BUTTONS_STYLESHEET);
    settingsButton->setFont(scf);
    refreshButton->setIcon(QIcon(":/res/refresh.png"));
    refreshButton->setText(tr("refresh"));
    refreshButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);
    refreshButton->setFont(scf);
	QScroller::grabGesture(scrArea, QScroller::ScrollerGestureType::LeftMouseButtonGesture);
    userid->setFont(scf);

	innerWidget->installEventFilter(keyfilter);

#ifdef QT_VERSION5X
	QObject::connect(settingsButton, &QPushButton::clicked, this, &MainPageWidget::settinsPressed);
	QObject::connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
	QObject::connect(manualLogin, &LoginWidget::loginConfirmed, this, &MainPageWidget::userIdOk);
	QObject::connect(manualLogin, &LoginWidget::backRequired, this, &MainPageWidget::hideCurrent);
	QObject::connect(settingsScreen, &MainSettingsWidget::backRequired, this, &MainPageWidget::hideCurrent);
	QObject::connect(settingsScreen, &MainSettingsWidget::languageChanged, this, &MainPageWidget::languageChanged);
	QObject::connect(settingsScreen, &MainSettingsWidget::saveConfirmed, this, &MainPageWidget::settingsSaved);
	QObject::connect(loginsStorageWidget, &LoginSelectWidget::profilePicked, this, &MainPageWidget::userPicked);
	QObject::connect(userid, &QLineEdit::returnPressed, this, &MainPageWidget::userIdSearch);
	QObject::connect(this, &MainPageWidget::backRequired, qApp, &QApplication::quit);
	QObject::connect(loginsStorageWidget, &LoginSelectWidget::backRequired, qApp, &QApplication::quit);
	QObject::connect(refreshButton, &MegaIconButton::clicked, this, &MainPageWidget::loadUsers);
	QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &MainPageWidget::wasTimeout);
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &MainPageWidget::parseUsers);
#else
	QObject::connect(settingsButton, SIGNAL(clicked()), this, SLOT(settinsPressed()));
	QObject::connect(exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
	QObject::connect(manualLogin, SIGNAL(loginConfirmed(QString, QString)), this, SLOT(userIdOk(QString, QString)));
	QObject::connect(manualLogin, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(settingsScreen, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(settingsScreen, SIGNAL(languageChanged()), this, SLOT(languageChanged()));
    QObject::connect(settingsScreen, SIGNAL(saveConfirmed()), this, SLOT(settingsSaved()));
	QObject::connect(loginsStorageWidget, SIGNAL(profilePicked(UserProfile)), this, SLOT(userPicked(UserProfile)));
    QObject::connect(userid, SIGNAL(returnPressed()), this, SLOT(userIdSearch()));
    QObject::connect(this, SIGNAL(backRequired()), qApp, SLOT(quit()));
    QObject::connect(loginsStorageWidget, SIGNAL(backRequired()), qApp, SLOT(quit()));
    QObject::connect(refreshButton, SIGNAL(clicked()), this, SLOT(loadUsers()));
    QObject::connect(&awaiter, SIGNAL(requestTimeout()), this, SLOT(wasTimeout()));
    QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(parseUsers()));
#endif
	scrArea->setWidget(loginsStorageWidget);
}

bool MainPageWidget::isExpectingControl(int val)
{
#ifdef DEBUG
	detrace_METHEXPL("control! " << val << " aw: ");
#endif
	if (val < 9 && val >= -1)
	{
		if (val == -1)
		{
			qApp->quit();
			return true;
		}
		else
		{
            if (val < profiles.count() -1)
			{
				userPicked(profiles.at(val));
				return true;
			}
		}

	}
	return false;
}

void MainPageWidget::settinsPressed()
{
    _hideAny(settingsScreen);
}

void MainPageWidget::userIdOk(const QString log, const  QString pass)
{
	hideCurrent();
	emit loggedIn();
}

void MainPageWidget::userPicked(UserProfile up)
{
	if (awaiter.isAwaiting())
		return;
	show_login_widget(up.login);
}

void MainPageWidget::hideCurrent()
{
	if (!_hideCurrent(innerWidget))
	{
		emit backRequired();
	}
}

void MainPageWidget::languageChanged()
{
	userHelpLabel->setText(tr("main_page_select_profile_tip"));
	userIdInfo->setText(tr("main_page_enter_profile_id_tip"));
	exitButton->setText(tr("main_page_exit_button"));
	settingsButton->setText(tr("main_page_settings_button"));
}

void MainPageWidget::userIdSearch()
{
	if (awaiter.isAwaiting())
		return;
	QVector<UserProfile>::iterator start = profiles.begin();
	while (start != profiles.end())
	{
		if (start->name.contains(userid->text()))
		{
			userPicked(*start);
			return;
		}
		++start;
    }
}

void MainPageWidget::parseUsers()
{
	hideProcessingOverlay();
    parse_uniresults_functions::UserProfilesResult temp = RequestParser::interpretAsLogin(awaiter.restext, awaiter.errtext);
    if (temp.manually)
    {
#ifdef DEBUG
        detrace_METHEXPL("manually parsed from request");
#endif
        QString temp;
        show_login_widget(temp);
    }
    else
    {
        profiles = temp.profiles;
        loginsStorageWidget->reload();
    }
}

void MainPageWidget::wasTimeout()
{
	hideProcessingOverlay();
	userHelpLabel->setText(tr("timeout_with_delay:") + QString::number(globalSettings.timeoutInt));
}

void MainPageWidget::settingsSaved()
{
	hostLabel->setText(globalSettings.HttpUrl.section("/", 4, 4));
	loadUsers();
}

void MainPageWidget::loadUsers()
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
	globalSettings.networkingEngine->userUpdateList(&awaiter, RECEIVER_SLOT_NAME);
    awaiter.run();
}
