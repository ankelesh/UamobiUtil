#include "UamobiUtil.h"
#include <QtGui/qevent.h>
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "widgets/BranchingTools/EmbeddedBranches.h"

UamobiUtil::UamobiUtil( QWidget* parent)
	: QWidget(parent), abstractDynamicNode(
		new MainPageWidget(this),
		new QVBoxLayout(this)),
	overlay(new ProcessingOverlay(AppSettings->timeoutInt, this))
{
	bindProcessingOverlay(overlay);
	overlay->hide();
	this->setLayout(mainLayout);
#ifdef Q_OS_WINCE
	this->setBaseSize(calculateAdaptiveSize(0.8));
	this->setMaximumSize(calculateAdaptiveSize(1));
	this->setMaximumHeight(0.9);
	this->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
#endif
#ifdef QT_VERSION5X

	QTimer::singleShot(500, _upCO<MainPageWidget>(), SLOT(loadUsers()));
	QObject::connect(_upCO<MainPageWidget>(), &MainPageWidget::loggedIn, 
		this, &UamobiUtil::gotoModeSelection);
#else
	//TODO
	MainPageWidget* mlp = qobject_cast<MainPageWidget*> (mainPage);
	QTimer::singleShot(500, mlp, SLOT(loadUsers()));
	QObject::connect(mlp, SIGNAL(loggedIn()), this, SLOT(gotoModeSelection()));
#endif
}

void UamobiUtil::gotoModeSelection()
{
	ModeSelectionWidget* mb = new ModeSelectionWidget(this);
	mb->loadModes();
#ifdef QT_VERSION5X
	QObject::connect(mb, &ModeSelectionWidget::backRequired, this, &UamobiUtil::hideCurrent);
	QObject::connect(mb, &ModeSelectionWidget::modeAcquired, this, &UamobiUtil::interpretMode);
#else
	QObject::connect(mb, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(mb, SIGNAL(modeAcquired(QHash<QString, QString>, parsedMode)), this, SLOT(interpretMode(QHash<QString, QString>, parsedMode)));
#endif
	_hideAndDeleteCurrent(mb);
}

void UamobiUtil::gotoReceiptBranch(QHash<QString, QString> opts, Mode mode)
{
#ifdef DEBUG
	detrace_METHCALL("goto Receipt");
#endif
	AbsBranch* mainBranch;
	if (mode->submode.isEmpty())
	{
		mainBranch = BranchFactory::createNWAdjustableBranch(
			embeddedBranches::receiptDesc
		);
	}
	else if (mode->submode.contains("warehouse", Qt::CaseInsensitive))
	{
		mainBranch = BranchFactory::createNWAdjustableBranch(
			embeddedBranches::warehouseReceiptDesc
		);
	}
	else
	{
		mainBranch = BranchFactory::createNWAdjustableBranch(
			embeddedBranches::correctionReceiptDesc
		);
	}
#ifdef QT_VERSION5X
	QObject::connect(mainBranch, &AbsBranch::backRequired, this, &UamobiUtil::hideCurrent);
	QObject::connect(mainBranch, &AbsBranch::done, this, &UamobiUtil::closeBranch);
#else
	QObject::connect(RR, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
#endif
	_hideAndDeleteCurrent(mainBranch);
	mainBranch->raiseThisBranch(mode.staticCast<AbsRecEntity>());
}

void UamobiUtil::gotoInventoryBranch(QHash<QString, QString> opts, Mode mode)
{
	AbsBranch* mainBranch;
	if (mode->submode.isEmpty())
	{
		mainBranch = BranchFactory::createNWAdjustableBranch(
			embeddedBranches::inventoryDesc
		);
	}
	else 
	{
		mainBranch = BranchFactory::createNWAdjustableBranch(
			embeddedBranches::partInventoryDesc
		);
	}
#ifdef QT_VERSION5X
	QObject::connect(mainBranch, &AbsBranch::backRequired, this, &UamobiUtil::hideCurrent);
	QObject::connect(mainBranch, &AbsBranch::done, this, &UamobiUtil::closeBranch);
#else
	QObject::connect(RR, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
#endif
	_hideAndDeleteCurrent(mainBranch);
	mainBranch->raiseThisBranch(mode.staticCast<AbsRecEntity>());
}

void UamobiUtil::resizeEvent(QResizeEvent* rev)
{
	overlay->resize(rev->size());
	QWidget::resizeEvent(rev);
}

void UamobiUtil::interpretMode(QHash<QString, QString> sets, Mode mode)
{
#ifdef DEBUG
	detrace_METHEXPL("mode: " << mode.mode << " while submode " << mode.submode << " and name " << mode.name);
#endif

	if (mode->mode == "receipt")
	{
		gotoReceiptBranch(sets, mode);
	}
	else if (mode->mode == "inventory")
	{
		gotoInventoryBranch(sets, mode);
	}
}



void UamobiUtil::closeBranch(RecEntity e)
{
	_hideAndDeleteCurrent(untouchable);
	_upCO<MainPageWidget>()->loadUsers();
}

void UamobiUtil::hideCurrent()
{
	_hideAndDeleteCurrent(untouchable);
}