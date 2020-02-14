#include "InventoryParamsWidget.h"
#include "widgets/utils/ElementsStyles.h"
void InventoryParamsWidget::_handleRecord(RecEntity e)
{
	if (e.isNull())
		return;
	if (e->myType() == UniformXmlObject::Document)
		setDocument(upcastRecord<FullDocumentEntity>(e));
	if (e->myType() == UniformXmlObject::LesserDocument)
	{
		QSharedPointer<LesserDocumentEntity> temp = upcastRecord<LesserDocumentEntity>(e);
		setDocument(FullDocument(new FullDocumentEntity(
			temp->code)));
	}
}
void InventoryParamsWidget::_sendDataRequest()
{

}
InventoryParamsWidget::InventoryParamsWidget(QWidget* parent)
	: IndependentBranchNode(independent_nodes::InventoryParameters, parent), mainLayout(new QVBoxLayout(this)),
	title(new QLabel(this)), dateInfo(new QLabel(this)),
	dateField(new QDateEdit(this)), selectDocumentButton(new MegaIconButton(this)),
	commentInfo(new QLabel(this)), commentField(new QTextEdit(this)),
	statusInfo(new QLabel(this)), compcancelPanel(new QHBoxLayout(this)),
	completedButton(), cancelledButton(),
	footerLayout(new QHBoxLayout(this)), backButton(new MegaIconButton(this)),
	okButton(new MegaIconButton(this))
{
	this->setLayout(mainLayout);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(title);
	mainLayout->addWidget(dateInfo);
	mainLayout->addWidget(dateField);
	mainLayout->addWidget(selectDocumentButton);
	mainLayout->addWidget(commentInfo);
	mainLayout->addWidget(commentField);
	mainLayout->addWidget(statusInfo);
	mainLayout->addLayout(compcancelPanel);
	compcancelPanel->setSpacing(0);
	compcancelPanel->setContentsMargins(0, 0, 0, 0);
	mainLayout->addLayout(footerLayout);
	footerLayout->setContentsMargins(0, 0, 0, 0);
	footerLayout->setSpacing(0);
	footerLayout->addWidget(backButton);
	footerLayout->addWidget(okButton);

	QFont scf = makeFont(0.04);
	title->setText(tr("inventory_params_title"));
	title->setFont(scf);

	dateInfo->setText(tr("inventory_params_date_info"));
	dateInfo->setFont(scf);

	dateField->setFont(scf);
	selectDocumentButton->setText(tr("inventory_select_doc"));
	selectDocumentButton->setStyleSheet(NAVIGATE_BUTTONS_STYLESHEET);
	selectDocumentButton->setIcon(QIcon(":/res/invz.png"));

	commentInfo->setText(tr("inventory_comment_info"));
	commentInfo->setFont(scf);
	commentField->setFont(scf);
	statusInfo->setText(tr("inventory_status_info"));
	statusInfo->setFont(scf);
	backButton->setText(tr("back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	okButton->setText(tr("OK"));
	okButton->setIcon(QIcon(":/res/submit.png"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	okButton->setDisabled(true);

#ifdef QT_VERSION5X
	QObject::connect(backButton, &MegaIconButton::clicked, this, &InventoryParamsWidget::backRequired);
	QObject::connect(selectDocumentButton, &MegaIconButton::clicked, this, &InventoryParamsWidget::selectDocPressed);
	QObject::connect(okButton, &MegaIconButton::clicked, this, &InventoryParamsWidget::okPressed);
#else
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(selectDocumentButton, SIGNAL(clicked()), this, SLOT(selectDocPressed()));
	QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(okPressed()));
#endif
}

void InventoryParamsWidget::setDocument(FullDocument doc)
{
	okButton->setDisabled(false);
	commentField->setText(doc->comment);
	document = doc;
}


void InventoryParamsWidget::selectDocPressed()
{
	emit backRequired();
}

void InventoryParamsWidget::okPressed()
{
	document->comment = commentField->toPlainText();
	emit done(RecEntity(document->clone()));
}

void InventoryParamsWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& /*overloads*/)
{
}
