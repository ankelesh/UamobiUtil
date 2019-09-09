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
	mainLayout->addWidget(modeSelectionBranch);
#ifdef QT_VERSION5X
	auto mlp = qobject_cast<MainPageWidget*>(mainPage);
	QObject::connect(mlp, &MainPageWidget::loggedIn, this, &UamobiUtil::gotoModeSelection);

#else
	!impl
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
	QObject::connect(mb, &ModeSelectionWidget::placeAndModeAcquired, this, &UamobiUtil::interpretMode);
	QObject::connect(mb, &ModeSelectionWidget::backRequired, this, &UamobiUtil::hideCurrent);
	modeSelectionBranch = mb;
	current = &modeSelectionBranch;
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