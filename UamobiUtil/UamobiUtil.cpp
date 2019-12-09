#include "UamobiUtil.h"
#include "widgets/InventoryBranch/InventoryRootWidget.h"
#include <QtGui/qevent.h>
#ifdef DEBUG
#include "debugtrace.h"
#endif

UamobiUtil::UamobiUtil(GlobalAppSettings& go, QWidget* parent)
	: QWidget(parent), globalSettings(go), mainLayout(new QVBoxLayout(this)),
	mainPage(),
	modeSelectionBranch(),
	current(&mainPage), overlay(new ProcessingOverlay(go.timeoutInt, this))
{
	bindProcessingOverlay(overlay);
	overlay->hide();
	mainPage = QPointer<inframedWidget>(new MainPageWidget(globalSettings, this));
	this->setLayout(mainLayout);
#ifdef Q_OS_WINCE
	this->setBaseSize(calculateAdaptiveSize(0.8));
	this->setMaximumSize(calculateAdaptiveSize(1));
	this->setMaximumHeight(0.9);
	this->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
#endif
	//mainLayout->setSizeConstraint(QBoxLayout::SizeConstraint::SetMaximumSize);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	mainLayout->addWidget(mainPage);
	mainPage->setFocus();
#ifdef QT_VERSION5X
	auto mlp = qobject_cast<MainPageWidget*>(mainPage);

	QTimer::singleShot(500, mlp, SLOT(loadUsers()));
	QObject::connect(mlp, &MainPageWidget::loggedIn, this, &UamobiUtil::gotoModeSelection);
#else
	MainPageWidget* mlp = qobject_cast<MainPageWidget*> (mainPage);
	QTimer::singleShot(500, mlp, SLOT(loadUsers()));
	QObject::connect(mlp, SIGNAL(loggedIn()), this, SLOT(gotoModeSelection()));
#endif
}

void UamobiUtil::gotoModeSelection()
{
	(*current)->hide();
	if (*current != mainPage)
	{
		mainLayout->removeWidget(*current);
		delete (*current);
	}
	ModeSelectionWidget* mb = new ModeBranchRootWidget(globalSettings, this);
#ifdef QT_VERSION5X
	QObject::connect(mb, &ModeSelectionWidget::backRequired, this, &UamobiUtil::hideCurrent);
	QObject::connect(mb, &ModeSelectionWidget::modeAcquired, this, &UamobiUtil::interpretMode);
#else
	QObject::connect(mb, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(mb, SIGNAL(modeAcquired(QHash<QString, QString>, parsedMode)), this, SLOT(interpretMode(QHash<QString, QString>, parsedMode)));
#endif
	modeSelectionBranch = mb;
	current = &modeSelectionBranch;
	mainLayout->addWidget(*current);
	(*current)->show();
}

void UamobiUtil::gotoReceiptBranch(QHash<QString, QString> opts, parsedMode mode)
{
#ifdef DEBUG
	detrace_METHCALL("goto Receipt");
#endif
	(*current)->hide();
	if (*current != mainPage)
	{
		mainLayout->removeWidget(*current);
		(*current)->deleteLater();
	}
	ReceiptRootWidget* RR = new ReceiptRootWidget(globalSettings, opts, mode, this);
#ifdef QT_VERSION5X
	QObject::connect(RR, &ReceiptRootWidget::backRequired, this, &UamobiUtil::hideCurrent);
#else
	QObject::connect(RR, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
#endif
	receiptBranch = RR;
	current = &receiptBranch;
	mainLayout->addWidget(*current);
	(*current)->show();
}

void UamobiUtil::gotoInventoryBranch(QHash<QString, QString> opts, parsedMode mode)
{
	(*current)->hide();
	if (*current != mainPage)
	{
		mainLayout->removeWidget(*current);
		(*current)->deleteLater();
	}
	InventoryRootWidget* IR = new InventoryRootWidget(globalSettings, opts, mode, this);
#ifdef QT_VERSION5X
	QObject::connect(IR, &InventoryRootWidget::backRequired, this, &UamobiUtil::hideCurrent);
#else
	QObject::connect(IR, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
#endif
	inventoryBranch = IR;
	current = &inventoryBranch;
	mainLayout->addWidget(*current);
	(*current)->show();
}

void UamobiUtil::resizeEvent(QResizeEvent* rev)
{
	overlay->resize(rev->size());
	QWidget::resizeEvent(rev);
}

void UamobiUtil::interpretMode(QHash<QString, QString> sets, parsedMode mode)
{
#ifdef DEBUG
	detrace_METHEXPL("mode: " << mode.mode << " while submode " << mode.submode << " and name " << mode.name);
#endif

	if (mode.mode == "receipt")
	{
		gotoReceiptBranch(sets, mode);
	}
	else if (mode.mode == "inventory")
	{
		gotoInventoryBranch(sets, mode);
	}
}

void UamobiUtil::gotoBranch(branches branch)
{
	switch (branch)
	{
	case login:
		hideCurrent();
		mainPage->show();
		current = &mainPage;
		break;
	case modeselect:
		gotoModeSelection();
		break;

	default:
		break;
	}
}

void UamobiUtil::hideCurrent()
{
	if (current != &mainPage)
	{
		if (current == &modeSelectionBranch)
		{
			(*current)->hide();
			mainLayout->removeWidget(*current);
			(*current)->deleteLater();
			current = &mainPage;
			(*current)->show();
			(*current)->setFocus();
		}
		else
		{
			gotoModeSelection();
		}
	}
}