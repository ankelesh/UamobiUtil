#include "ModeSelectionWidget.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif

// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QScroller>
#else
 // Qt 4 only imports
#include "legacy/qtCompatibility/scrollgrabber.h"
#endif
#include "widgets/utils/ElementsStyles.h"


QString specwidgets::_modeSelectionWidget::elemAsString(int index)
{
	return modes.at(index).name;
}

int specwidgets::_modeSelectionWidget::countElems()
{
	return modes.count();
}

specwidgets::_modeSelectionWidget::_modeSelectionWidget(modesResponse& Modes, QWidget* parent)
	: AbstractVariantSelectionWidget(parent), modes(Modes)
{
}

void specwidgets::_modeSelectionWidget::indexSelected(int Index)
{
	emit modeSelected(modes.at(Index));
}

ModeSelectionWidget::ModeSelectionWidget(const GlobalAppSettings& go, QWidget* parent)
	: inframedWidget(parent), globalSettings(go), allmodes(), mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	buttonLayout(new QHBoxLayout(innerWidget)), scrArea(new QScrollArea(innerWidget)), userTip(new QLabel(innerWidget)),
	modesTip(new QLabel(innerWidget)), modeSelection(new specwidgets::_modeSelectionWidget(allmodes, scrArea)),
	logoutButton(new MegaIconButton(innerWidget)), awaiter(globalSettings.timeoutInt, this)
{
	this->setLayout(mainLayout);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	innerLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(innerWidget);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(userTip);
	innerLayout->addWidget(modesTip);
	scrArea->setWidgetResizable(true);
	innerLayout->addWidget(scrArea);
	innerLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(logoutButton);
	//buttonLayout->addStretch();


	userTip->setText(tr("mode_selection_user_tip!"));
	userTip->setAlignment(Qt::AlignCenter);
	userTip->setStyleSheet(countAdaptiveFont(0.03));
	
	modesTip->setText(tr("mode_selection_modes_tip:"));
	modesTip->setAlignment(Qt::AlignCenter);
	modesTip->setStyleSheet(countAdaptiveFont(0.03));

	logoutButton->setIcon(QIcon(":/res/back.png"));
	logoutButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);
	logoutButton->setText(tr("mode_selection_logout_tip"));

	loadModes();
	modeSelection->reload();

	scrArea->setWidget(modeSelection);
    QScroller::grabGesture(scrArea, QScroller::LeftMouseButtonGesture);


#ifdef QT_VERSION5X
	QObject::connect(logoutButton, &QPushButton::clicked, this, &ModeSelectionWidget::logoutPressed);
	QObject::connect(modeSelection, &specwidgets::_modeSelectionWidget::modeSelected, this, &ModeSelectionWidget::modeSelected);
	QObject::connect(modeSelection, &specwidgets::_modeSelectionWidget::backRequired, this, &ModeSelectionWidget::backRequired);
	QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &ModeSelectionWidget::was_timeout);
#else
	QObject::connect(logoutButton, SIGNAL(clicked()), this, SLOT(logoutPressed()));
	QObject::connect(modeSelection, SIGNAL(modeSelected(parsedMode)), this, SLOT(modeSelected(parsedMode)));
    QObject::connect(modeSelection, SIGNAL(backRequired()), this,SIGNAL(backRequired()));
#endif
}

bool ModeSelectionWidget::back()
{
	return false;
}

bool ModeSelectionWidget::isExpectingControl(int val)
{
	if (awaiter.isAwaiting())
		return false;
	if (val >= -1 && val < allmodes.count()-1)
	{
		if (val == -1)
		{
			if (allmodes.count() > 10)
				val = 9;
			else
				return false;
		}
		
			modeSelected(allmodes.at(val));
			return true;
		
	}
	return false;
}

void ModeSelectionWidget::show()
{
	this->setFocus();
	inframedWidget::show();
}

void ModeSelectionWidget::loadModes()
{
	if (awaiter.isAwaiting())
		return;
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &ModeSelectionWidget::parse_modes);
	globalSettings.networkingEngine->modeList(&awaiter, RECEIVER_SLOT_NAME, "nolang");
	awaiter.run();
}

void ModeSelectionWidget::logoutPressed()
{
	globalSettings.networkingEngine->userLogOut(Q_NULLPTR, "");
	emit backRequired();
}

void ModeSelectionWidget::modeSelected(parsedMode Mode)
{ 
	if (awaiter.isAwaiting())
		return;
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &ModeSelectionWidget::mode_select_response);
	globalSettings.networkingEngine->modeSelect(Mode.mode, Mode.submode, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	selected = Mode;
}

void ModeSelectionWidget::hideCurrent()
{
	emit backRequired();
}

void ModeSelectionWidget::parse_modes()
{
	using RequestParser::interpretAsModeList;
	allmodes = interpretAsModeList(awaiter.restext, awaiter.errtext);
	modeSelection->reload();
	awaiter.disconnect(SIGNAL(requestReceived), this, SLOT(parse_modes()));
}

void ModeSelectionWidget::mode_select_response()
{
	parse_uniresults_functions::PositionalResponse resp =
		RequestParser::interpretAsPositionalResponse(awaiter.restext, awaiter.errtext);
#ifdef DEBUG
	detrace_METHEXPL("request was " << resp.success);
#endif

	if (resp.success)
	{
		settings = resp.values;
#ifdef DEBUG
		detrace_METHEXPL("mode approved: " << Mode.debugSnapshot());
#endif
	}
	else
	{
		selected.mode = "None";
	}
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(mode_select_response()));
}

void ModeSelectionWidget::was_timeout()
{

	userTip->setText(tr("mode_selection_timeout!") + QString::number(globalSettings.timeoutInt));
	awaiter.disconnect(SIGNAL(requestReceived), this, SLOT(parse_modes()));
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(mode_select_response()));
}

ModeBranchRootWidget::ModeBranchRootWidget(const GlobalAppSettings& go, QWidget* parent)
	: ModeSelectionWidget(go, parent), abstractNode(), placeSelection(new PlaceSelectionWidget(go, this))
{
	mainLayout->addWidget(placeSelection);
	placeSelection->hide();
	current = innerWidget;
	untouchable = innerWidget;
	main = this;
#ifdef QT_VERSION5X
	QObject::connect(placeSelection, &PlaceSelectionWidget::placeAcquired, this, &ModeBranchRootWidget::placeAcquired);
	QObject::connect(placeSelection, &PlaceSelectionWidget::backRequired, this, &ModeBranchRootWidget::hideCurrent);
#else
	QObject::connect(placeSelection, SIGNAL(placeAcquired(parsedPlace)), this, SLOT(placeAcquired(parsedPlace)));
	QObject::connect(placeSelection, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
#endif
}


void ModeBranchRootWidget::placeAcquired(parsedPlace pm)
{
#ifdef DEBUG
	detrace_METHEXPL("emitting mode " << selected.debugSnapshot());
#endif
	emit modeAcquired(settings, selected);
}

void ModeBranchRootWidget::hideCurrent()
{
	if (!current->back())
	{
		if (current == innerWidget)
		{
			emit backRequired();
		}
		else
		{
			_hideCurrent(innerWidget);
		}
	}
}

void ModeBranchRootWidget::mode_select_response()
{
	ModeSelectionWidget::mode_select_response();
	if (selected.mode == "None")
	{
		return;
	}
	else
	{
		_hideAny(placeSelection);
		placeSelection->loadPlaces();
	}
}
