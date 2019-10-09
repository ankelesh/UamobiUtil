#include "AbstractScaningWidget.h"

AbstractScaningWidget::AbstractScaningWidget(GlobalAppSettings& go, QWidget* parent)
	: inframedWidget(parent), globalSettings(go),
	mainLayout(new QVBoxLayout(this)), innerWidget(new inframedWidget(this)),
	innerLayout(new QVBoxLayout(innerWidget)), userInfo(new QLabel(innerWidget)),
	stateInfo(new QLabel(innerWidget)), barcodeField(new QLineEdit(innerWidget)),
	mainTextView(new QTextEdit(innerWidget)), quantitySpin(new BigButtonsSpinbox(BigButtonsSpinbox::intspin, this, 0.125)),
	buttonPanel(new QHBoxLayout(innerWidget)),
	backButton(new QPushButton(innerWidget)),
	submitButton(new QPushButton(innerWidget)), searchButton(new QPushButton(innerWidget)),
	keyFilter(new filters::NoKeyEvents(this)), itemSuppliedValues()
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(userInfo);
	innerLayout->addWidget(stateInfo);
	innerLayout->addWidget(barcodeField);
	innerLayout->addWidget(mainTextView);
	innerLayout->addWidget(quantitySpin);
	innerLayout->addLayout(buttonPanel);
	buttonPanel->addWidget(backButton);
	buttonPanel->addWidget(searchButton);
	buttonPanel->addWidget(submitButton);

	userInfo->setText(tr("scaning_widget_user_info"));
	stateInfo->setText(tr("scaning_widget_state_ready"));
	mainTextView->setText(tr("scaning_widget_filler_text"));
	backButton->setText(tr("scaning_widget_back"));
	submitButton->setText(tr("scaining_widget_submit"));
	searchButton->setText(tr("scaning_widget_search"));

	innerWidget->installEventFilter(keyFilter);
	barcodeField->setFocus();
	quantitySpin->hide();

#ifdef QT_VERSION5X

	QObject::connect(backButton, &QPushButton::clicked, this, &AbstractScaningWidget::backNeeded);
	QObject::connect(submitButton, &QPushButton::clicked, this, &AbstractScaningWidget::submitPressed);
	QObject::connect(keyFilter, &filters::NoKeyEvents::backRequired, this, &AbstractScaningWidget::backRequired);
	QObject::connect(keyFilter, &filters::NoKeyEvents::numberObtained, this, &AbstractScaningWidget::processNumber);
	QObject::connect(keyFilter, &filters::NoKeyEvents::returnObtained, this, &AbstractScaningWidget::barcodeConfirmed);
	QObject::connect(barcodeField, &QLineEdit::returnPressed, this, &AbstractScaningWidget::barcodeConfirmed);
	QObject::connect(searchButton, &QPushButton::clicked, this, &AbstractScaningWidget::searchRequired);
#else
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(submitButton, SIGNAL(clicked()), this, SLOT(submitPressed()));
	QObject::connect(keyFilter, SIGNAL(backRequired()), this, SIGNAL(backRequired()));
	QObject::connect(keyFilter, SIGNAL(numberObtained(QString)), this, SLOT(processBarcode(QString)));
	QObject::connect(keyFilter, SIGNAL(returnObtained()), this, SLOT(barcodeConfirmed()));
	QObject::connect(barcodeField, SIGNAL(returnPressed()), this, SLOT(barcodeConfirmed()));
#endif
}