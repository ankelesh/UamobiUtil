#include "AbstractScaningWidget.h"

AbstractScaningWidget::AbstractScaningWidget(GlobalAppSettings& go, QWidget* parent)
	: inframedWidget(parent), globalSettings(go), 
	mainLayout(new QVBoxLayout(this)), innerWidget(new inframedWidget(this)),
	innerLayout(new QVBoxLayout(innerWidget)), userInfo(new QLabel(innerWidget)),
	stateInfo(new QLabel(innerWidget)), barcodeField(new QLineEdit(innerWidget)),
	mainTextView(new QTextEdit(innerWidget)), buttonPanel(new QHBoxLayout(innerWidget)),
	backButton(new QPushButton(innerWidget)), photoButton(new QPushButton(innerWidget)),
	submitButton(new QPushButton(innerWidget)), keyFilter(new filters::NoKeyEvents(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(userInfo);
	innerLayout->addWidget(stateInfo);
	innerLayout->addWidget(barcodeField);
	innerLayout->addWidget(mainTextView);
	innerLayout->addLayout(buttonPanel);
	buttonPanel->addWidget(backButton);
	buttonPanel->addWidget(photoButton);
	buttonPanel->addWidget(submitButton);

	userInfo->setText(tr("scaning_widget_user_info"));
	stateInfo->setText(tr("scaning_widget_state_ready"));
	mainTextView->setText(tr("scaning_widget_filler_text"));
	backButton->setText(tr("scaning_widget_back"));
	photoButton->setText(tr("scaning_widget_photo"));
	submitButton->setText(tr("scainig_widget_submit"));

	innerWidget->installEventFilter(keyFilter);
	barcodeField->setFocus();

#ifdef QT_VERSION5X

	QObject::connect(backButton, &QPushButton::clicked, this, &AbstractScaningWidget::backRequired);
	QObject::connect(submitButton, &QPushButton::clicked, this, &AbstractScaningWidget::submitPressed);
	QObject::connect(photoButton, &QPushButton::clicked, this, &AbstractScaningWidget::photoPressed);
	QObject::connect(keyFilter, &filters::NoKeyEvents::backRequired, this, &AbstractScaningWidget::backRequired);
	QObject::connect(keyFilter, &filters::NoKeyEvents::numberObtained, this, &AbstractScaningWidget::processNumber);
	QObject::connect(keyFilter, &filters::NoKeyEvents::returnObtained, this, &AbstractScaningWidget::barcodeConfirmed);
	QObject::connect(barcodeField, &QLineEdit::returnPressed, this, &AbstractScaningWidget::barcodeConfirmed);
#else
	!!!implement!!!
#endif
}
