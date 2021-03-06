#include "InventoryParamsWidget.h"
#include "widgets/utils/ElementsStyles.h"
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif



void InventoryParamsWidget::_handleRecord(RecEntity e)
{
	if (e.isNull())
	{
#ifdef DEBUG
		detrace_METHPERROR("_handleRecord", "null item provided");
#endif
		return;
	}
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
#ifdef DEBUG
	detrace_DCONSTR("InventotyParams");
#endif
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

	title->setText(tr("inventory_params_title"));
	title->setFont(GENERAL_FONT);

	dateInfo->setText(tr("inventory_params_date_info"));
	dateInfo->setFont(GENERAL_FONT);

	dateField->setFont(GENERAL_FONT);
	selectDocumentButton->setText(tr("inventory_select_doc"));
	selectDocumentButton->setStyleSheet(NAVIGATE_BUTTONS_STYLESHEET);
	selectDocumentButton->setIcon(QIcon(":/resources/invz"));

	commentInfo->setText(tr("inventory_comment_info"));
	commentInfo->setFont(GENERAL_FONT);
	commentField->setFont(GENERAL_FONT);
	statusInfo->setText(tr("inventory_status_info"));
	statusInfo->setFont(GENERAL_FONT);
	backButton->setText(tr("back"));
	backButton->setIcon(QIcon(":/resources/back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	okButton->setText(tr("OK"));
	okButton->setIcon(QIcon(":/resources/submit"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);
	okButton->setDisabled(true);
	setTabOrder(okButton, commentField);
	okButton->setDefault(true);
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

void InventoryParamsWidget::setFocus()
{
	okButton->setFocus();
}

void InventoryParamsWidget::setDocument(FullDocument doc)
{
	okButton->setDisabled(false);
	commentField->setText(doc->comment);
	document = doc;
	okButton->setFocus();
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
