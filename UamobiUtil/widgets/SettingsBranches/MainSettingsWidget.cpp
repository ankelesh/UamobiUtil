#include "MainSettingsWidget.h"

MainSettingsWidget::MainSettingsWidget(GlobalAppSettings& go, QWidget* parent)
	:inframedWidget(parent), globalSettings(go),
	mainLayout(new QVBoxLayout(this)), innerWidget(new QTabWidget(this)),
	wrkflTab(new QWidget(innerWidget)), wrkflinnLayout(new QVBoxLayout(wrkflTab)),
	wrkflScrollArea(new QScrollArea(innerWidget)),
	wrkflContents(new QWidget(wrkflScrollArea)), wrkflContLayout(new QVBoxLayout(wrkflContents)),
	scanModeSelector(new QComboBox(wrkflContents)),
	scanModeInfo(new QLabel(wrkflContents)),
	sysTab(new QWidget(innerWidget)), sysinnLayout(new QVBoxLayout(sysTab)),
	sysScrollArea(new QScrollArea(sysTab)), sysContents(new QWidget(sysScrollArea)),
	sysContLayout(new QVBoxLayout(sysContents)), topExplLabel(new QLabel(sysContents)),
	dataengGroup(new QGroupBox(sysContents)), dataengLayout(new QVBoxLayout(dataengGroup)),
	dataengInfo(new QLabel(dataengGroup)),
	httpCheckBox(new QCheckBox(dataengGroup)), memcheckbox(new QCheckBox(dataengGroup)),
	connectionGroup(new QGroupBox(sysContents)), connectionLayout(new QVBoxLayout(connectionGroup)),
	connectionInfo(new QLabel(connectionGroup)), addressField(new QComboBox(connectionGroup)),
	langGroup(new QGroupBox(sysContents)), langLayout(new QVBoxLayout(langGroup)),
	langInfo(new QLabel(langGroup)), langField(new QComboBox(langGroup)),
	saveButton(new QPushButton(this)), backButton(new QPushButton(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	mainLayout->addWidget(saveButton);
	mainLayout->addWidget(backButton);
	innerWidget->addTab(wrkflTab, tr("settings_workflow_tab_title"));
	innerWidget->addTab(sysTab, tr("settings_system_tab_title"));
	mainLayout->setSpacing(0);		//	spacing removed to avoid space loss
	mainLayout->setContentsMargins(0, 0, 0, 0);

	wrkflTab->setLayout(wrkflinnLayout);
	wrkflinnLayout->addWidget(wrkflScrollArea);
	wrkflContents->setLayout(wrkflContLayout);
	wrkflContLayout->addWidget(scanModeInfo);
	wrkflContLayout->addWidget(scanModeSelector);

	sysTab->setLayout(sysinnLayout);
	sysinnLayout->addWidget(sysScrollArea);
	sysContents->setLayout(sysContLayout);
	sysContLayout->addWidget(topExplLabel);
	sysContLayout->addWidget(dataengGroup);
	sysContLayout->addWidget(connectionGroup);
	sysContLayout->addWidget(langGroup);

	dataengGroup->setLayout(dataengLayout);
	dataengLayout->addWidget(dataengInfo);
	dataengLayout->addWidget(httpCheckBox);
	dataengLayout->addWidget(memcheckbox);

	connectionGroup->setLayout(connectionLayout);
	connectionLayout->addWidget(connectionInfo);
	connectionLayout->addWidget(addressField);

	langGroup->setLayout(langLayout);
	langLayout->addWidget(langInfo);
	langLayout->addWidget(langField);

	scanModeInfo->setText(tr("settings_scan_mode_info"));
#ifdef QT_VERSION5X
	scanModeSelector->addItems(QStringList({ tr("settings_scmode_one"), tr("settings_scmode_autos"), tr("settings_scmode_simple") }));
#else
	QStringList temp;
	temp << tr("settings_scmode_one") << tr("settings_scmode_autos") << tr("settings_scmode_simple");
	scanModeSelector->addItems(temp);
#endif
	topExplLabel->setText(tr("settings_system_title"));
	dataengInfo->setText(tr("settings_system_dataupd_eng"));
	httpCheckBox->setText(tr("settings_system_http_mode"));
	memcheckbox->setText(tr("settings_system_inmemory"));
	connectionInfo->setText(tr("settings_select_address_tip"));
	addressField->addItem(go.HttpUrl);
	langInfo->setText(tr("settings_system_select_lang_tip"));
#ifdef QT_VERSION5X
	langField->addItems(QStringList({ "Russian", "Romanian", "English" }));
#else
	temp.clear();
	temp << "Russian" << "Romanian" << "English";
	langField->addItems(temp);
#endif
	saveButton->setText(tr("settings_save_button"));
	backButton->setText(tr("settings_back_button"));

	addressField->setEditable(true);
	addressField->setInsertPolicy(QComboBox::InsertAtTop);
	langField->setEditable(false);
	langField->setInsertPolicy(QComboBox::NoInsert);
#ifdef QT_VERSION5X
	langField->setCurrentText(go.language);
#else
	langField->setCurrentIndex(temp.indexOf(go.language));
#endif
	scanModeSelector->setEditable(false);
	scanModeSelector->setInsertPolicy(QComboBox::NoInsert);

	sysScrollArea->setWidget(sysContents);
	wrkflScrollArea->setWidget(wrkflContents);

#ifdef QT_VERSION5X
	QObject::connect(saveButton, &QPushButton::clicked, this, &MainSettingsWidget::saveClicked);
	QObject::connect(backButton, &QPushButton::clicked, this, &MainSettingsWidget::backRequired);
	QObject::connect(langField, QOverload<const QString&>::of(&QComboBox::activated), this, &MainSettingsWidget::langSelected);
#else
	QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(langField, SIGNAL(activated(QString)), this, SLOT(langSelected(QString)));
#endif
}

void MainSettingsWidget::saveClicked()
{
	globalSettings.HttpUrl = (addressField->currentText().isEmpty()) ? globalSettings.HttpUrl : addressField->currentText();
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
	dataengInfo->setText(tr("settings_system_dataupd_eng"));
	httpCheckBox->setText(tr("settings_system_http_mode"));
	memcheckbox->setText(tr("settings_system_inmemory"));
	connectionInfo->setText(tr("settings_select_address_tip"));
	langInfo->setText(tr("settings_system_select_lang_tip"));
	saveButton->setText(tr("settings_save_button"));
	backButton->setText(tr("settings_back_button"));
}