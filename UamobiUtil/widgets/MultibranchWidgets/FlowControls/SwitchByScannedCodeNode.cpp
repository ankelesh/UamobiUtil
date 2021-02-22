#include "SwitchByScannedCodeNode.h"
#include "widgets/BranchingTools/BranchFactory.h"
#include <QLabel>
#include <QLineEdit>
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#include "networking/RequestAwaiter.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/ElementsStyles.h"
#include "networking/Parsers/RequestParser.h"
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
#include "submodules/UNAQtCommons/barcodeHandling/BarcodeObserver.h"

void SwitchByScannedCodeWidget::_handleRecord(RecEntity e)
{
	toGiveToBranch = e;
}

void SwitchByScannedCodeWidget::_sendDataRequest()
{

}

void SwitchByScannedCodeWidget::_backReaction()
{
	if (currentlyOpened == untouchable)
		emit backRequired();
}

void SwitchByScannedCodeWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	if (!overloads.isEmpty())
	{
		QStringList defs;
		defs << "barcode";
		barcodeChecker = overloads.first().assertedAndMappedCopy(inventoryListParentDocs,defs, defs);
	}
}

void SwitchByScannedCodeWidget::setFocus()
{
	barcodeInput->setFocus();
}

SwitchByScannedCodeWidget::SwitchByScannedCodeWidget(BranchDescription branch, QWidget* parent)
	: IndependentBranchNode(independent_nodes::SwitchByScanBc, parent),
	abstractDynamicNode(	new inframedWidget(this), new QVBoxLayout(this)),
	innerLayout(new QVBoxLayout(untouchable)), info(new QLabel(untouchable)), 
	barcodeInput(new QLineEdit(untouchable)), buttonLayout(new QHBoxLayout(untouchable)),
	backButton(new MegaIconButton(untouchable)), skipButton(new MegaIconButton(untouchable)),
	normalFlowBranch(Q_NULLPTR), barcodeChecker(QueryTemplates::receiptOrderByBC), prototype(), descr(branch),awaiter(new RequestAwaiter(AppSettings->timeoutInt, this))
{
	mainLayout->addWidget(untouchable);
	untouchable->setLayout(innerLayout);
	innerLayout->addWidget(info);
	innerLayout->addWidget(barcodeInput);
	innerLayout->addStretch(1);
	innerLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(backButton);
	buttonLayout->addWidget(skipButton);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	buttonLayout->setSpacing(0);
	buttonLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);

	
	setFont(*AppFonts->general());
	if (branch->namesOverload.isEmpty())
		info->setText(tr("Please scan code"));
	else
		info->setText(branch->namesOverload.first());
	backButton->setText(tr("back"));
	skipButton->setText(tr("skip"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	backButton->setIcon(QIcon(":/res/back.png"));
	skipButton->setIcon(QIcon(":/res/editable.png"));
	info->setWordWrap(true);
	if (branch->entity.isNull())
	{
		prototype = RecEntity(new OrderEntity());
	}
	else
	{
		prototype = branch->entity;
	}
	setTabOrder(barcodeInput, skipButton);
	backButton->setFocusPolicy(Qt::NoFocus);
	skipButton->setDefault(true);
	
#ifdef QT_VERSION5X
	QObject::connect(backButton, &MegaIconButton::clicked, this, &SwitchByScannedCodeWidget::backRequired);
	QObject::connect(skipButton, &MegaIconButton::clicked, this, &SwitchByScannedCodeWidget::_branchSkipped);
	QObject::connect(awaiter, &RequestAwaiter::requestSuccess, this, &SwitchByScannedCodeWidget::_parseResponse);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &SwitchByScannedCodeWidget::_timeout);
	QObject::connect(BarcodeObs, &BarcodeObserver::barcodeCaught, this, &SwitchByScannedCodeWidget::_barcodeReceived);
	QObject::connect(barcodeInput, &QLineEdit::returnPressed, this, &SwitchByScannedCodeWidget::_checkBarcode);
#else
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(skipButton, SIGNAL(clicked()), this, SLOT(_branchSkipped()));
	QObject::connect(awaiter, SIGNAL(requestSuccess(QString, QString)), this, SLOT(_parseResponse(QString, QString)));
	QObject::connect(awaiter, SIGNAL(requestTimeout()), this, SLOT(_timeout()));
	QObject::connect(BarcodeObs, SIGNAL(barcodeCaught(QString)), this, SLOT(_barcodeReceived(QString)));
	QObject::connect(barcodeInput, SIGNAL(returnPressed()), this, SLOT(_checkBarcode()));
#endif
	BarcodeObs->activate();
}

void SwitchByScannedCodeWidget::_innerBranchEnded(RecEntity e)
{
	emit done(e);
}

void SwitchByScannedCodeWidget::_innerBranchBack()
{
	_hideAndDeleteCurrent(untouchable);
}

void SwitchByScannedCodeWidget::_branchSkipped()
{
	if (descr->hasSubElements())
	{
		IndependentBranchNode* node = BranchNodeFactory::createNode(descr->first(), this);
		_hideAndDeleteCurrent(node);
		node->loadData();
		node->processRecord(toGiveToBranch);
#ifdef QT_VERSION5X
		QObject::connect(currentlyOpened, &inframedWidget::backRequired, this, &SwitchByScannedCodeWidget::_innerBranchBack);
		QObject::connect(_upCO<IndependentBranchNode>(), &IndependentBranchNode::done, this,&SwitchByScannedCodeWidget::_innerBranchEnded);
#else
		QObject::connect(currentlyOpened, SIGNAL(backRequired()), this, SLOT(_innerBranchBack()));
		QObject::connect(_upCO<IndependentBranchNode>(), SIGNAL(done(RecEntity)), this, SLOT(_innerBranchEnded(RecEntity)));
#endif
	}
	else
	{
#ifdef DEBUG
		detrace_METHPERROR("_branchSkipped", "no subbranch provided, skip unsucceeded");
#endif
		info->setText(tr("you must scan code!"));
	}
}

void SwitchByScannedCodeWidget::_timeout()
{
	info->setText(tr("Timeout!"));
}

void SwitchByScannedCodeWidget::_parseResponse(QString res, QString err)
{
	ResponseParser parser(new LinearListParser(res, err));
	PolyResponse resp =  RequestParser::parseResponse(parser, prototype);
	if (resp.isError)
	{
#ifdef DEBUG
		detrace_NETERROR(resp.errtext, "Switch by scanned code failed");
#endif
		info->setText(resp.errtext);
	}
	else
	{
		if (resp.isEmpty())
		{
			info->setText(tr("No such code!"));
#ifdef DEBUG
			detrace_NETERROR("no entities in response", "Switch by scanned code failed");;
#endif
		}
		else
		{
			emit done(resp.objects.first());
		}
	}
}

void SwitchByScannedCodeWidget::_barcodeReceived(QString bc)
{
	barcodeInput->setText(bc);
	_checkBarcode();
}

void SwitchByScannedCodeWidget::_checkBarcode()
{
	if (barcodeInput->text().isEmpty() || awaiter->isAwaiting())
	{
		return;
	}

	AppNetwork->execQueryByTemplate(barcodeChecker, barcodeInput->text(), awaiter);
}
