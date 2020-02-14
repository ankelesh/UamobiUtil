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
#ifdef Q_OS_WIN
#ifndef Q_OS_WINCE
	setFixedSize(imitatePhoneSize(0.66));
#endif
#endif
#ifdef QT_VERSION5X
	QObject::connect(_upCO<MainPageWidget>(), &MainPageWidget::loggedIn, 
		this, &UamobiUtil::gotoModeSelection);
#else
    QObject::connect(_upCO<MainPageWidget>(), SIGNAL(loggedIn()), this, SLOT(gotoModeSelection()));
#endif
	_upCO<MainPageWidget>()->loadUsers();
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
    QObject::connect(mb, SIGNAL(modeAcquired(QHash<QString, QString>, Mode)), this, SLOT(interpretMode(QHash<QString, QString>, Mode)));
#endif
	_hideAndDeleteCurrent(mb);
}

void UamobiUtil::gotoReceiptBranch(QHash<QString, QString> opts, Mode mode)
{
#ifdef DEBUG
	detrace_METHCALL("goto Receipt");
#endif
	using namespace independent_nodes;
	AbsBranch* mainBranch;
	if (mode->submode.isEmpty())
	{
		mainBranch = BranchFactory::createNWBranch(
			embeddedBranches::receiptDesc,
			this
		);
	}
	else if (mode->submode.contains("warehouse", Qt::CaseInsensitive))
	{
		mainBranch = BranchFactory::createNWBranch(
			embeddedBranches::warehouseReceiptDesc,
			this
		);
	}
	else
	{
		mainBranch = BranchFactory::createNWBranch(
			embeddedBranches::correctionReceiptDesc,
			this
		);
	}
#ifdef QT_VERSION5X
	QObject::connect(mainBranch, &AbsBranch::backRequired, this, &UamobiUtil::gotoModeSelection);
	QObject::connect(mainBranch, &AbsBranch::done, this, &UamobiUtil::closeBranch);
#else
    QObject::connect(mainBranch, SIGNAL(backRequired()), this, SLOT(gotoModeSelection()));
    QObject::connect(mainBranch, SIGNAL(done(RecEntity)), this, SLOT(closeBranch(RecEntity)));
#endif
	_hideAndDeleteCurrent(mainBranch);
	mainBranch->raiseThisBranch(mode.staticCast<AbsRecEntity>());
}

void UamobiUtil::gotoInventoryBranch(QHash<QString, QString> opts, Mode mode)
{
	using namespace independent_nodes;
	AbsBranch* mainBranch;
	if (mode->submode.isEmpty())
	{
		mainBranch = BranchFactory::createNWBranch(
			embeddedBranches::inventoryDesc,
			this
		);
	}
	else 
	{
		mainBranch = BranchFactory::createNWBranch(
			embeddedBranches::partInventoryDesc,
			this
		);
	}
#ifdef QT_VERSION5X
	QObject::connect(mainBranch, &AbsBranch::backRequired, this, &UamobiUtil::gotoModeSelection);
    QObject::connect(mainBranch, &AbsBranch::done, this, &UamobiUtil::closeBranch);
#else
    QObject::connect(mainBranch, SIGNAL(backRequired()), this, SLOT(gotoModeSelection()));
    QObject::connect(mainBranch, SIGNAL(done(RecEntity)), this, SLOT(closeBranch(RecEntity)));
#endif
	_hideAndDeleteCurrent(mainBranch);
	mainBranch->raiseThisBranch(mode.staticCast<AbsRecEntity>());
}

void UamobiUtil::gotoPrintingBranch(QHash<QString, QString>, Mode mode)
{
	using namespace independent_nodes;
	AbsBranch* mainBranch;
	
	mainBranch = BranchFactory::createNWBranch(
			embeddedBranches::printingDesc,
		this
		);
#ifdef QT_VERSION5X
	QObject::connect(mainBranch, &AbsBranch::backRequired, this, &UamobiUtil::gotoModeSelection);
	QObject::connect(mainBranch, &AbsBranch::done, this, &UamobiUtil::closeBranch);
#else
	QObject::connect(mainBranch, SIGNAL(backRequired()), this, SLOT(gotoModeSelection()));
	QObject::connect(mainBranch, SIGNAL(done(RecEntity)), this, SLOT(closeBranch(RecEntity)));
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

	if (mode->mode == "receipt")
	{
		gotoReceiptBranch(sets, mode);
	}
	else if (mode->mode == "inventory")
	{
		gotoInventoryBranch(sets, mode);
	}
	else if (mode->mode == "printing")
	{
		gotoPrintingBranch(sets, mode);
	}
}



void UamobiUtil::closeBranch(RecEntity e)
{
	gotoModeSelection();
}

void UamobiUtil::hideCurrent()
{
	_hideAndDeleteCurrent(untouchable);
}
