#include "ReceiptParametersWidget.h"
#include "widgets/utils/ElementsStyles.h"
#ifdef QT_VERSION5X
#include <QtWidgets/qscroller.h>
#else
#include "legacy/qtCompatibility/scrollgrabber.h"
#endif
void ReceiptParametersWidget::_handleRecord(RecEntity e)
{
	if (e.isNull())
		return;
	if (e->myType() == UniformXmlObject::Order)
	{
		setMainView(upcastRecord<OrderEntity>(e)->text);
	}
}
void ReceiptParametersWidget::_sendDataRequest()
{

}
ReceiptParametersWidget::ReceiptParametersWidget(QWidget* parent)
	: IndependentBranchNode(independent_nodes::ReceiptParameters, parent),
	mainLayout(new QVBoxLayout(this)), innerWidget(new inframedWidget(this)),
	innerLayout(new QVBoxLayout(innerWidget)), userInfo(new QLabel(innerWidget)),
	innerFrame(new QFrame(innerWidget)), frameLayout(new QVBoxLayout(innerFrame)),
	stateInfo(new QLabel(innerFrame)), closedButton(new MegaIconButton(innerFrame)),
	cancelledButton(new MegaIconButton(innerFrame)), dateField(new QDateEdit(innerFrame)),
	selectOrderLayout(new QHBoxLayout(innerFrame)), selectOrderButton(new MegaIconButton(innerFrame)),
	mainTextView(new QTextEdit(innerFrame)), inspectLayout(new QHBoxLayout(innerFrame)),
	inspectButton(new MegaIconButton(innerFrame)), continueLayout(new QHBoxLayout(innerFrame)),
	continueButton(new MegaIconButton(innerFrame)), backButton(new MegaIconButton(innerFrame))
{
	this->setLayout(new QVBoxLayout(this));
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	frameLayout->setContentsMargins(0, 0, 0, 0);
	frameLayout->setSpacing(0);
	selectOrderLayout->setContentsMargins(0, 0, 0, 0);
	selectOrderLayout->setSpacing(0);
	continueLayout->setContentsMargins(0, 0, 0, 0);
	continueLayout->setSpacing(0);
	innerLayout->addWidget(userInfo);
	innerLayout->addWidget(innerFrame);
	innerFrame->setLayout(frameLayout);
	frameLayout->addWidget(stateInfo);
	frameLayout->addWidget(closedButton);
	frameLayout->addWidget(cancelledButton);
	frameLayout->addWidget(dateField);
	frameLayout->addLayout(selectOrderLayout);
	selectOrderLayout->addWidget(selectOrderButton);
	frameLayout->addWidget(mainTextView);
	frameLayout->addLayout(inspectLayout);
	inspectLayout->addStretch();
	inspectLayout->addWidget(inspectButton);
	frameLayout->addLayout(continueLayout);
	continueLayout->addWidget(backButton);
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
	userInfo->setFont(makeFont(0.04));
	stateInfo->setFont(makeFont(0.04));
	mainTextView->setFont(makeFont(0.04));
	mainTextView->setReadOnly(true);
	QScroller::grabGesture(mainTextView, QScroller::LeftMouseButtonGesture);

	continueButton->setIcon(QIcon(":/res/forward.png"));
	backButton->setIcon(QIcon(":/res/back.png"));
	selectOrderButton->setIcon(QIcon(":/res/upload.png"));
	continueButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	selectOrderButton->setStyleSheet(SETTINGS_BUTTONS_STYLESHEET);
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
	QObject::connect(selectOrderButton, &MegaIconButton::clicked, this, &ReceiptParametersWidget::backToSupplier);
#else
	QObject::connect(closedButton, SIGNAL(clicked()), this, SLOT(closedClicked()));
	QObject::connect(cancelledButton, SIGNAL(clicked()), this, SLOT(cancelledClicked()));
	QObject::connect(inspectButton, SIGNAL(clicked()), this, SLOT(inspectClicked()));
	QObject::connect(continueButton, SIGNAL(clicked()), this, SLOT(continueClicked()));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(selectOrderButton, SIGNAL(clicked()), this, SLOT(backToSupplier()));
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

void ReceiptParametersWidget::backToSupplier()
{
	emit backTo(0);
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