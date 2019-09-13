#include "ReceiptParametersWidget.h"

ReceiptParametersWidget::ReceiptParametersWidget(GlobalAppSettings& go, QWidget* parent)
	: inframedWidget(parent), globalSettings(go),
	mainLayout(new QVBoxLayout(this)), innerWidget(new inframedWidget(this)),
	innerLayout(new QVBoxLayout(innerWidget)), userInfo(new QLabel(innerWidget)),
	innerFrame(new QFrame(innerWidget)), frameLayout(new QVBoxLayout(innerFrame)),
	stateInfo(new QLabel(innerFrame)), closedButton(new QPushButton(innerFrame)),
	cancelledButton(new QPushButton(innerFrame)),dateField(new QDateEdit(innerFrame)),
	selectOrderLayout(new QHBoxLayout(innerFrame)), selectOrderButton(new QPushButton(innerFrame)),
	mainTextView(new QTextEdit(innerFrame)), inspectLayout(new QHBoxLayout(innerFrame)),
	inspectButton(new QPushButton(innerFrame)), continueLayout(new QHBoxLayout(innerFrame)),
	continueButton(new QPushButton(innerFrame)), backButton(new QPushButton(innerFrame))
{
	this->setLayout(new QVBoxLayout(this));
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(userInfo);
	innerLayout->addWidget(innerFrame);
	innerFrame->setLayout(frameLayout);
	frameLayout->addWidget(stateInfo);
	frameLayout->addWidget(closedButton);
	frameLayout->addWidget(cancelledButton);
	frameLayout->addWidget(dateField);
	frameLayout->addLayout(selectOrderLayout);
	selectOrderLayout->addStretch();
	selectOrderLayout->addWidget(selectOrderButton);
	selectOrderLayout->addStretch();
	frameLayout->addWidget(mainTextView);
	frameLayout->addLayout(inspectLayout);
	inspectLayout->addStretch();
	inspectLayout->addWidget(inspectButton);
	frameLayout->addLayout(continueLayout);
	continueLayout->addWidget(backButton);
	continueLayout->addStretch();
	continueLayout->addWidget(continueButton);
	userInfo->setText(tr("receipt_parameters_announce"));
	stateInfo->setText(tr("receipt_parameters_status"));
	closedButton->setText(tr("receipt_parameters_closed_button"));
	cancelledButton->setText(tr("receipt_parameters_cancelled_button"));
	selectOrderButton->setText(tr("receipt_parameters_select_order"));
	inspectButton->setText(tr("receipt_parameters_inspect"));
	continueButton->setText(tr("receipt_parameters_continue"));
	backButton->setText(tr("receipt_parameters_back"));

	userInfo->setAlignment(Qt::AlignCenter);
	userInfo->setFont(QFont("Times new Roman", 1, 20, false));

	// hidings dependent

	stateInfo->hide();
	closedButton->hide();
	cancelledButton->hide();
	dateField->hide();
	inspectButton->hide();

#ifdef QT_VERSION5X
	QObject::connect(closedButton, &QPushButton::clicked, this, &ReceiptParametersWidget::closedClicked);
	QObject::connect(cancelledButton, &QPushButton::clicked, this, &ReceiptParametersWidget::cancelledClicked);
	QObject::connect(inspectButton, &QPushButton::clicked, this, &ReceiptParametersWidget::inspectClicked);
	QObject::connect(continueButton, &QPushButton::clicked, this, &ReceiptParametersWidget::continueClicked);
	QObject::connect(backButton, &QPushButton::clicked, this, &ReceiptParametersWidget::backRequired);

#else
	throw;
#endif
}

void ReceiptParametersWidget::closedClicked()
{

}

void ReceiptParametersWidget::cancelledClicked()
{

}

void ReceiptParametersWidget::inspectClicked()
{

}

void ReceiptParametersWidget::continueClicked()
{
	emit dataConfirmed();
}

void ReceiptParametersWidget::setMainView(const QString& rtxt)
{
	mainTextView->setAcceptRichText(true);
	mainTextView->insertHtml(rtxt);
}

