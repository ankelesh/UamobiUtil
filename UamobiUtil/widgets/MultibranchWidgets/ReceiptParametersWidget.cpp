#include "ReceiptParametersWidget.h"
#include "widgets/utils/ElementsStyles.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif

void ReceiptParametersWidget::_handleRecord(RecEntity e)
{
	if (e.isNull())
	{
#ifdef DEBUG
		detrace_METHPERROR("_handleRecord", "null entity provided");
#endif
		return;
	}
	dependency = RecEntity(e->clone());
	if (e->myType() == UniformXmlObject::Order)
	{
		setMainView(upcastRecord<OrderEntity>(e)->text);
	}
	else
	{
		setMainView(e->getTitle());
	}
}
void ReceiptParametersWidget::_sendDataRequest()
{

}
ReceiptParametersWidget::ReceiptParametersWidget(QWidget* parent)
	: IndependentBranchNode(independent_nodes::ReceiptParameters, parent),
	mainLayout(new QVBoxLayout(this)), innerWidget(new inframedWidget(this)),
	innerLayout(new QVBoxLayout(innerWidget)), 
	innerFrame(new QFrame(innerWidget)), frameLayout(new QVBoxLayout(innerFrame)),
	stateInfo(new QLabel(innerFrame)), closedButton(new MegaIconButton(innerFrame)),
	cancelledButton(new MegaIconButton(innerFrame)), dateField(new QDateEdit(innerFrame)),
	mainTextView(new QTextEdit(innerFrame)), inspectLayout(new QHBoxLayout(innerFrame)),
	inspectButton(new MegaIconButton(innerFrame)), continueLayout(new QHBoxLayout(innerFrame)),
	continueButton(new MegaIconButton(innerFrame)), backButton(new MegaIconButton(innerFrame))
{
#ifdef DEBUG
	detrace_DCONSTR("ReceiptParametersWidget");
#endif
	this->setLayout(new QVBoxLayout(this));
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	frameLayout->setContentsMargins(0, 0, 0, 0);
	frameLayout->setSpacing(0);
	continueLayout->setContentsMargins(0, 0, 0, 0);
	continueLayout->setSpacing(0);
	innerLayout->addWidget(innerFrame);
	innerFrame->setLayout(frameLayout);
	frameLayout->addWidget(stateInfo);
	frameLayout->addWidget(closedButton);
	frameLayout->addWidget(cancelledButton);
	frameLayout->addWidget(dateField);
	frameLayout->addWidget(mainTextView);
	frameLayout->addLayout(inspectLayout);
	inspectLayout->addStretch();
	inspectLayout->addWidget(inspectButton);
	frameLayout->addLayout(continueLayout);
	continueLayout->addWidget(backButton);
	continueLayout->addWidget(continueButton);
	stateInfo->setText(tr("receipt_parameters_status"));
	closedButton->setText(tr("receipt_parameters_closed_button"));
	cancelledButton->setText(tr("receipt_parameters_cancelled_button"));
	inspectButton->setText(tr("receipt_parameters_inspect"));
	continueButton->setText(tr("receipt_parameters_continue"));
	backButton->setText(tr("receipt_parameters_back"));

	stateInfo->setFont(GENERAL_FONT);
	mainTextView->setFont(AppFonts->makeCustomFont(0.03));
	mainTextView->setReadOnly(true);

	continueButton->setIcon(QIcon(":/res/forward.png"));
	backButton->setIcon(QIcon(":/res/back.png"));
	continueButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	// hidings dependent
	mainTextView->installEventFilter(keyfilter);
	innerWidget->installEventFilter(keyfilter);
	stateInfo->hide();
	closedButton->hide();
	cancelledButton->hide();
	dateField->hide();
	inspectButton->hide();

#ifdef QT_VERSION5X
	QObject::connect(closedButton, &MegaIconButton::clicked, this, &ReceiptParametersWidget::closedClicked);
	QObject::connect(cancelledButton, &MegaIconButton::clicked, this, &ReceiptParametersWidget::cancelledClicked);
	QObject::connect(inspectButton, &MegaIconButton::clicked, this, &ReceiptParametersWidget::inspectClicked);
	QObject::connect(continueButton, &MegaIconButton::clicked, this, &ReceiptParametersWidget::continueClicked);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &ReceiptParametersWidget::backRequired);
#else
	QObject::connect(closedButton, SIGNAL(clicked()), this, SLOT(closedClicked()));
	QObject::connect(cancelledButton, SIGNAL(clicked()), this, SLOT(cancelledClicked()));
	QObject::connect(inspectButton, SIGNAL(clicked()), this, SLOT(inspectClicked()));
	QObject::connect(continueButton, SIGNAL(clicked()), this, SLOT(continueClicked()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
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
	emit done(dependency);
}


void ReceiptParametersWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& /*overloads*/)
{
}

void ReceiptParametersWidget::returnReaction()
{
	continueClicked();
}

void ReceiptParametersWidget::setMainView(const QString& rtxt)
{
	mainTextView->setAcceptRichText(true);
	mainTextView->insertHtml(rtxt);
}
