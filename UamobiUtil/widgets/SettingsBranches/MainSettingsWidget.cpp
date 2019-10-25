#include "MainSettingsWidget.h"
#include "widgets/utils/ElementsStyles.h"
MainSettingsWidget::MainSettingsWidget(GlobalAppSettings& go, QWidget* parent)
	:inframedWidget(parent), globalSettings(go),
    mainLayout(new QVBoxLayout(this)),
    scanModeSelector(new QComboBox(this)),
    scanModeInfo(new QLabel(this)),
    topExplLabel(new QLabel(this)),
    connectionInfo(new QLabel(this)), addressField(new QComboBox(this)),
    langInfo(new QLabel(this)), langField(new QComboBox(this)),
	footerLayout(new QHBoxLayout(this)),
	saveButton(new MegaIconButton(this)), backButton(new MegaIconButton(this))
{
    this->setLayout(mainLayout);
#ifdef Q_OS_WINCE
    this->setFixedSize(calculateAdaptiveSize(1));
    this->setFixedHeight(calculateAdaptiveButtonHeight(0.9));
#endif
	mainLayout->setSpacing(0);		//	spacing removed to avoid space loss
    mainLayout->setContentsMargins(0, 0, 0, 0);

    mainLayout->addWidget(scanModeInfo);
    mainLayout->addWidget(scanModeSelector);
    mainLayout->addWidget(topExplLabel);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    mainLayout->addWidget(connectionInfo);
    mainLayout->addWidget(addressField);
    mainLayout->addWidget(langInfo);
    mainLayout->addWidget(langField);

	QFont scf = makeFont(0.04);

	scanModeInfo->setText(tr("settings_scan_mode_info"));
	scanModeInfo->setAlignment(Qt::AlignCenter);
	scanModeInfo->setFont(scf);
#ifdef QT_VERSION5X
	scanModeSelector->addItems(QStringList({ tr("settings_scmode_one"), tr("settings_scmode_autos"), tr("settings_scmode_simple") }));
#else
	QStringList temp;
	temp << tr("settings_scmode_one") << tr("settings_scmode_autos") << tr("settings_scmode_simple");
	scanModeSelector->addItems(temp);
#endif
	scanModeSelector->setFont(scf);
	scanModeSelector->setMinimumHeight(calculateAdaptiveButtonHeight());

	topExplLabel->setText(tr("settings_system_title"));
	topExplLabel->setAlignment(Qt::AlignCenter);
	topExplLabel->setFont(scf);

	connectionInfo->setText(tr("settings_select_address_tip"));
	connectionInfo->setAlignment(Qt::AlignCenter);
	connectionInfo->setFont(scf);

	addressField->addItem(go.HttpUrl);
    addressField->addItems(go.AlternativeAdresses);
    addressField->setCurrentIndex(0);
	addressField->setFont(scf);

	langInfo->setText(tr("settings_system_select_lang_tip"));
	langInfo->setAlignment(Qt::AlignCenter);
	langInfo->setFont(scf);
#ifdef QT_VERSION5X
	langField->addItems(QStringList({ "Russian", "Romanian", "English" }));
#else
	temp.clear();
	temp << "Russian" << "Romanian" << "English";
	langField->addItems(temp);
    if (globalSettings.language == "Russian")
        langField->setCurrentIndex(0);
    else if (globalSettings.language == "Romanian")
        langField->setCurrentIndex(1);
    else
        langField->setCurrentIndex(2);
#endif
	saveButton->setText(tr("settings_save_button"));
	saveButton->setIcon(QIcon(":/res/with.png"));
	saveButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);

	backButton->setText(tr("settings_back_button"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	addressField->setEditable(true);
	addressField->setInsertPolicy(QComboBox::InsertAtTop);
	langField->setEditable(false);
	langField->setFont(scf);
	langField->setInsertPolicy(QComboBox::NoInsert);
#ifdef QT_VERSION5X
	langField->setCurrentText(go.language);
#else
	langField->setCurrentIndex(temp.indexOf(go.language));
#endif
	scanModeSelector->setEditable(false);
	scanModeSelector->setInsertPolicy(QComboBox::NoInsert);
	scanModeSelector->setFont(scf);
    mainLayout->addLayout(footerLayout);
    footerLayout->addWidget(saveButton);
    footerLayout->addWidget(backButton);

#ifdef QT_VERSION5X
	QObject::connect(saveButton, &MegaIconButton::clicked, this, &MainSettingsWidget::saveClicked);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &MainSettingsWidget::backRequired);
	QObject::connect(langField, QOverload<const QString&>::of(&QComboBox::activated), this, &MainSettingsWidget::langSelected);
	QObject::connect(addressField, QOverload<const QString&>::of(&QComboBox::activated), this, &MainSettingsWidget::AddressSelected);
#else
	QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(langField, SIGNAL(activated(QString)), this, SLOT(langSelected(QString)));
    QObject::connect(addressField, SIGNAL(activated(QString)), this, SLOT(AddressSelected(QString)));
#endif
}

void MainSettingsWidget::show()
{

    inframedWidget::show();
}

void MainSettingsWidget::saveClicked()
{
	globalSettings.HttpUrl = (addressField->currentText().isEmpty()) ? globalSettings.HttpUrl : addressField->currentText();
	if (!globalSettings.AlternativeAdresses.contains(globalSettings.HttpUrl))
	{
		globalSettings.AlternativeAdresses.push_back(globalSettings.HttpUrl);
		addressField->addItem(globalSettings.HttpUrl);
		globalSettings.dump();
	}
	globalSettings.networkingEngine->setUrl(globalSettings.HttpUrl);
	emit saveConfirmed();
	emit backRequired();
}

void MainSettingsWidget::langSelected(const QString& lang)
{
	globalSettings.language = lang;
	langChanged();
	emit languageChanged();
}

void MainSettingsWidget::langChanged()
{
	globalSettings.setTranslator();
	innerWidget->setTabText(0, tr("settings_workflow_tab_title"));
	innerWidget->setTabText(1, tr("settings_system_tab_title"));
	scanModeInfo->setText(tr("settings_scan_mode_info"));
#ifdef QT_VERSION5X
	scanModeSelector->addItems(QStringList({ tr("settings_scmode_one"), tr("settings_scmode_autos"), tr("settings_scmode_simple") }));
#else
	QStringList temp;
	temp << tr("settings_scmode_one") << tr("settings_scmode_autos") << tr("settings_scmode_simple");
	scanModeSelector->addItems(temp);
#endif
	topExplLabel->setText(tr("settings_system_title"));
	connectionInfo->setText(tr("settings_select_address_tip"));
	langInfo->setText(tr("settings_system_select_lang_tip"));
	saveButton->setText(tr("settings_save_button"));
	backButton->setText(tr("settings_back_button"));
}


void MainSettingsWidget::AddressSelected(const QString& activated)
{
	globalSettings.HttpUrl = activated;
}
