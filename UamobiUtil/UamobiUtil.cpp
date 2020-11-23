#include "UamobiUtil.h"
#include <QtGui/qevent.h>
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "widgets/BranchingTools/EmbeddedBranches.h"
#include "widgets/BranchingTools/BranchException.h"
#include "SoundWrappers/SoundEffectPlayer.h"

UamobiUtil::UamobiUtil( QWidget* parent)
	: QWidget(parent), abstractDynamicNode(
		new MainPageWidget(this),
		new QVBoxLayout(this)),
	overlay(new ProcessingOverlay(AppSettings->timeoutInt, this))
{
	bindProcessingOverlay(overlay);
	overlay->hide();
	this->setLayout(mainLayout);
	setFont(*(AppFonts->general()));
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
#ifdef Q_OS_ANDROID
	setMaximumSize(calculateAdaptiveSize(1));
#endif
#ifdef QT_VERSION5X
	QObject::connect(_upCO<MainPageWidget>(), &MainPageWidget::loggedIn, 
		this, &UamobiUtil::gotoModeSelection);
#else
    QObject::connect(_upCO<MainPageWidget>(), SIGNAL(loggedIn()), this, SLOT(gotoModeSelection()));
#endif
    _upCO<MainPageWidget>()->loadUsers();
}

UamobiUtil::~UamobiUtil()
{
    AppSettings->dump();
}

void UamobiUtil::gotoModeSelection()
{
	ModeSelectionWidget* mb = new ModeSelectionWidget(this);
    AppSounds->play(SoundEffectPlayer::ItemScannedDSE);
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


void UamobiUtil::resizeEvent(QResizeEvent* rev)
{
	overlay->resize(rev->size());
	QWidget::resizeEvent(rev);
}

void UamobiUtil::interpretMode(QHash<QString, QString> /*sets*/, Mode mode)
{
	using namespace independent_nodes;
	detrace_MODEINTERPRET(mode->name, mode->mode, mode->submode);
	AbsBranch* mainBranch;

	if (mode->submode.isEmpty())
		return;
	mainBranch = BranchFactory::createNWBranch(
		mode->submode,
		this
	);
	mainLayout->addWidget(mainBranch);
#ifdef QT_VERSION5X
	QObject::connect(mainBranch, &AbsBranch::backRequired, this, &UamobiUtil::gotoModeSelection);
	QObject::connect(mainBranch, &AbsBranch::done, this, &UamobiUtil::closeBranch);
    QObject::connect(mainBranch, &AbsBranch::exceptionThrown, this, &UamobiUtil::handleUnhandledBranchException);
#else
	QObject::connect(mainBranch, SIGNAL(backRequired()), this, SLOT(gotoModeSelection()));
	QObject::connect(mainBranch, SIGNAL(done(RecEntity)), this, SLOT(closeBranch(RecEntity)));
    QObject::connect(mainBranch, SIGNAL(exceptionThrown(BranchException*)), this, SLOT(handleUnhandledBranchException(BranchException*)));
#endif
	_hideAndDeleteCurrent(mainBranch);
	mainBranch->raiseThisBranch(mode.staticCast<AbsRecEntity>());
}



void UamobiUtil::closeBranch(RecEntity /*e*/)
{
    gotoModeSelection();
}

void UamobiUtil::handleUnhandledBranchException(BranchException *ex)
{
    if (ex != Q_NULLPTR)
    {
        switch (ex->specialDestination())
        {
        case BranchException::ToLogin:
		{
			hideCurrent();
			AppNetwork->execQueryByTemplate(QueryTemplates::LogOut, Q_NULLPTR);
			QString t;
			AppNetwork->setSession(t);
			_upCO<MainPageWidget>()->loadUsers();
			break;
		}
        case BranchException::ToModeSelection:
        default:
            gotoModeSelection();
            break;
        }
    delete ex;
    }
}

void UamobiUtil::hideCurrent()
{
	_hideAndDeleteCurrent(untouchable);
}
