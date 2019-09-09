#include "UamobiUtil.h"



UamobiUtil::UamobiUtil(GlobalAppSettings & go, QWidget* parent)
	: QWidget(parent), globalSettings(go), mainLayout(new QVBoxLayout(this)),
	mainPage(QPointer<inframedWidget>(new MainPageWidget(globalSettings,this))),
	modeSelectionBranch(),
	current(&mainPage)
{
	this->setLayout(mainLayout);
	this->setBaseSize(calculateAdaptiveSize(1));
	mainLayout->addWidget(mainPage);
#ifdef QT_VERSION5X
	auto mlp = qobject_cast<MainPageWidget*>(mainPage);
	QObject::connect(mlp, &MainPageWidget::loggedIn, this, &UamobiUtil::gotoModeSelection);

#else
    MainPageWidget * mlp = qobject_cast<MainPageWidget*> (mainPage);
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
    ModeSelectionWidget* mb = new ModeSelectionWidget(globalSettings, this);
#ifdef QT_VERSION5X
    QObject::connect(mb, &ModeSelectionWidget::backRequired, this, &UamobiUtil::hideCurrent);
	QObject::connect(mb, &ModeSelectionWidget::placeAndModeAcquired, this, &UamobiUtil::gotoSupplierSelection);
#else
    QObject::connect(mb, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	throw;
#endif
	modeSelectionBranch = mb;
	current = &modeSelectionBranch;
	mainLayout->addWidget(*current);
	(*current)->show();
}

void UamobiUtil::gotoSupplierSelection()
{
	detrace_METHCALL("goto Suppl")
	(*current)->hide();
	if (*current != mainPage)
	{
		mainLayout->removeWidget(*current);
		(*current)->deleteLater();
	}
	SuppliersSelectWidget* SS = new SuppliersSelectWidget(globalSettings, this);
#ifdef QT_VERSION5X
	QObject::connect(SS, &SuppliersSelectWidget::backRequired, this, &UamobiUtil::hideCurrent);
#else
	QObject::connect(SS, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
#endif
	suppliersSelectionBranch = SS;
	current = &suppliersSelectionBranch;
	mainLayout->addWidget(*current);
	(*current)->show();
}

void UamobiUtil::interpretMode(QHash<QString, QString>)
{
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
		(*current)->hide();
		mainLayout->removeWidget(*current);
		delete (*current);
	}
}
