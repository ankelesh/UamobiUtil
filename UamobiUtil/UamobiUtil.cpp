#include "UamobiUtil.h"

UamobiUtil::UamobiUtil(GlobalAppSettings& go, QWidget* parent)
	: QWidget(parent), globalSettings(go), mainLayout(new QVBoxLayout(this)),
	mainPage(QPointer<inframedWidget>(new MainPageWidget(globalSettings, this))),
	modeSelectionBranch(),
	current(&mainPage)
{
	this->setLayout(mainLayout);
	this->setBaseSize(calculateAdaptiveSize(0.8));
	mainLayout->addWidget(mainPage);
	mainPage->setFocus();
#ifdef QT_VERSION5X
	auto mlp = qobject_cast<MainPageWidget*>(mainPage);
	QObject::connect(mlp, &MainPageWidget::loggedIn, this, &UamobiUtil::gotoModeSelection);

#else
	MainPageWidget* mlp = qobject_cast<MainPageWidget*> (mainPage);
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
	QObject::connect(mb, &ModeSelectionWidget::modeAcquired, this, &UamobiUtil::gotoReceiptBranch);
#else
	QObject::connect(mb, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(mb, SIGNAL(modeAcquired(QHash<QString, QString>)), this, SLOT(gotoReceiptBranch(QHash<QString, QString>)));
#endif
	modeSelectionBranch = mb;
	current = &modeSelectionBranch;
	mainLayout->addWidget(*current);
	(*current)->show();
}

void UamobiUtil::gotoReceiptBranch(QHash<QString, QString> opts, parsedMode mode)
{
#ifdef DEBUG
	//detrace_METHCALL("goto Receipt");
#endif
	(*current)->hide();
	if (*current != mainPage)
	{
		mainLayout->removeWidget(*current);
		(*current)->deleteLater();
	}
	ReceiptRootWidget* RR = new ReceiptRootWidget(globalSettings, opts, mode.submode, this);
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

void UamobiUtil::interpretMode(QHash<QString, QString> sets, parsedMode mode)
{
	if (mode.mode == "receipt")
	{
		gotoReceiptBranch(sets, mode);
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
			delete (*current);
			current = &mainPage;
			(*current)->show();
		}
		else
		{
			gotoModeSelection();
		}
	}
}