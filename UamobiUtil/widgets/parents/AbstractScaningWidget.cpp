#include "AbstractScaningWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
AbstractScaningWidget::AbstractScaningWidget(GlobalAppSettings& go, QWidget* parent)
	: inframedWidget(parent), globalSettings(go),
	mainLayout(new QVBoxLayout(this)), innerWidget(new inframedWidget(this)),
	innerLayout(new QVBoxLayout(innerWidget)), topPanelLayout(new QHBoxLayout(innerWidget)),
	userInfo(new QLabel(innerWidget)),
	barcodeField(new QLineEdit(innerWidget)),
	mainTextView(new QTextEdit(innerWidget)),
	buttonPanel(new QHBoxLayout(innerWidget)),
 quitButton(new MegaIconButton(innerWidget)), switchFocus(new MegaIconButton(innerWidget)),
	backButton(new MegaIconButton(innerWidget)),
	submitButton(new MegaIconButton(innerWidget)), searchButton(new MegaIconButton(innerWidget)),
 itemSuppliedValues(), 
	first_control(), second_control(), controlsAvailable(0), awaiter(go.timeoutInt, this)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addLayout(topPanelLayout);
	topPanelLayout->addWidget(quitButton);
	topPanelLayout->addWidget(userInfo);
	topPanelLayout->addWidget(switchFocus);
	innerLayout->addWidget(barcodeField);
	innerLayout->addWidget(mainTextView);
	innerLayout->addLayout(buttonPanel);
	buttonPanel->addWidget(backButton);
	buttonPanel->addWidget(searchButton);
	buttonPanel->addWidget(submitButton);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	topPanelLayout->setContentsMargins(0, 0, 0, 0);
	topPanelLayout->setSpacing(0);

	QFont scaledFont = makeFont(0.04);

	userInfo->setText(tr("scaning_widget_user_info"));
	userInfo->setAlignment(Qt::AlignCenter);
	userInfo->setFont(scaledFont);
	

	mainTextView->setText(tr("scaning_widget_filler_text"));
	mainTextView->setFont(scaledFont);
	mainTextView->setAcceptRichText(true);
	mainTextView->setReadOnly(true);
	mainTextView->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
	mainTextView->setStyleSheet("QLabel{background-color: white; border: 1px solid black}");
	mainTextView->setAlignment(Qt::AlignTop);
	backButton->setText(tr("scaning_widget_back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	submitButton->setText(tr("scaining_widget_submit"));
	submitButton->setIcon(QIcon(":/res/submit.png"));
	submitButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);

	searchButton->setText(tr("scaning_widget_search"));
	searchButton->setIcon(QIcon(":/res/search.png"));
	searchButton->setStyleSheet(CHANGE_BUTTONS_STYLESHEET);

	quitButton->setText(tr("quit"));
	quitButton->setIcon(QIcon(":/res/data.png"));
	quitButton->setStyleSheet(CANCEL_BUTTONS_STYLESHEET);
	quitButton->setMaximumWidth(calculateAdaptiveWidth(0.2));

	switchFocus->setText(tr("focus"));
	switchFocus->setIcon(QIcon(":/res/toinput.png"));
	switchFocus->setStyleSheet(SETTINGS_BUTTONS_STYLESHEET);
	switchFocus->setMaximumWidth(calculateAdaptiveWidth(0.2));
	barcodeField->setFocus();

#ifdef QT_VERSION5X

	QObject::connect(backButton, &QPushButton::clicked, this, &AbstractScaningWidget::backNeeded);
	QObject::connect(submitButton, &QPushButton::clicked, this, &AbstractScaningWidget::submitPressed);
    QObject::connect(barcodeField, &QLineEdit::returnPressed, this, &AbstractScaningWidget::barcodeConfirmed);
	QObject::connect(searchButton, &QPushButton::clicked, this, &AbstractScaningWidget::searchRequired);
    QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &AbstractScaningWidget::was_timeout);
	QObject::connect(quitButton, &MegaIconButton::clicked, this, &AbstractScaningWidget::backRequired);
	QObject::connect(switchFocus, &MegaIconButton::clicked, this, &AbstractScaningWidget::switchedFocus);
#else
    QObject::connect(backButton, SIGNAL(clicked()), this, SLOT(backNeeded()));
    QObject::connect(submitButton, SIGNAL(clicked()), this, SLOT(submitPressed()));
    QObject::connect(barcodeField, SIGNAL(editingFinished()), this, SLOT(barcodeConfirmed()));
    QObject::connect(searchButton, SIGNAL(clicked()), this, SLOT(searchRequired()));
    QObject::connect(&awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
    QObject::connect(quitButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
#endif
}

void AbstractScaningWidget::was_timeout()
{
	userInfo->setText("scaning_timeout:" + QString::number(globalSettings.timeoutInt));
	hideProcessingOverlay();
}

bool AbstractScaningWidget::isManualInFocus()
{
	return false;
}

bool AbstractScaningWidget::handleScannedBarcode()
{
	return false;
}

bool AbstractScaningWidget::handleNumberInbuffer()
{
	return false;
}

int AbstractScaningWidget::flushControl(int)
{
	return 0;
}

void AbstractScaningWidget::processBackPress()
{
}

void AbstractScaningWidget::removeManualFocus()
{
}

void AbstractScaningWidget::setControlFocus(int)
{
}

bool AbstractScaningWidget::isControlFocused()
{
	return false;
}
