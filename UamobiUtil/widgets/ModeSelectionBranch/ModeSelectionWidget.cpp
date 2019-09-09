#include "ModeSelectionWidget.h"

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
	logoutButton(new QPushButton(innerWidget)), placeSelection(new PlaceSelectionWidget(go,this)), current (innerWidget)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	mainLayout->addWidget(placeSelection);
	innerWidget->setLayout(innerLayout);
	innerLayout->addWidget(userTip);
	innerLayout->addWidget(modesTip);
	scrArea->setWidgetResizable(true);
	innerLayout->addWidget(scrArea);
	innerLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(logoutButton);
	buttonLayout->addStretch();
	placeSelection->hide();
	userTip->setText(tr("mode_selection_user_tip!"));
	modesTip->setText(tr("mode_selection_modes_tip:"));
	logoutButton->setText(tr("mode_selection_logout_tip"));

	loadModes();
	modeSelection->reload();

	scrArea->setWidget(modeSelection);

#ifdef QT_VERSION5X
	QObject::connect(logoutButton, &QPushButton::clicked, this, &ModeSelectionWidget::logoutPressed);
	QObject::connect(modeSelection, &specwidgets::_modeSelectionWidget::modeSelected, this, &ModeSelectionWidget::modeSelected);
	QObject::connect(placeSelection, &PlaceSelectionWidget::placeAcquired, this, &ModeSelectionWidget::placeReady);
	QObject::connect(placeSelection, &PlaceSelectionWidget::backRequired, this, &ModeSelectionWidget::hideCurrent);
#else
    QObject::connect(logoutButton, SIGNAL(clicked()), this, SLOT(logoutPressed()));
    QObject::connect(modeSelection, SIGNAL(modeSelected(parsedMode)), this, SLOT(modeSelected(parsedMode)));
    QObject::connect(placeSelection, SIGNAL(placeAcquired(parsedPlace)), this, SLOT(placeReady(parsedPlace)));
    QObject::connect(placeSelection, SIGNAL(backRequired()), this , SLOT(hideCurrent()));
#endif
}

bool ModeSelectionWidget::back()
{
	return false;
}

void ModeSelectionWidget::loadModes()
{
	using parse_uniresults_functions::modesResponse;
	using RequestParser::interpretAsModeList;
	RequestAwaiter awaiter(globalSettings.timeoutInt, this);
	globalSettings.networkingEngine->modeList(&awaiter, RECEIVER_SLOT_NAME, "nolang");
	awaiter.run();
	while (awaiter.isAwaiting())
	{
		qApp->processEvents();
	}
	if (awaiter.wasTimeout())
	{
		userTip->setText(tr("mode_selection_timeout!"));
	}
	else
	{
		allmodes = interpretAsModeList(awaiter.restext, awaiter.errtext);
		modeSelection->reload();
	}
}

void ModeSelectionWidget::logoutPressed()
{
	if (current->back())
	{
		return;
	}
	globalSettings.networkingEngine->userLogOut(Q_NULLPTR, "");
	emit backRequired();
}

void ModeSelectionWidget::modeSelected(parsedMode Mode)
{
	RequestAwaiter awaiter(globalSettings.timeoutInt, this);
	globalSettings.networkingEngine->modeSelect(Mode.mode, Mode.submode, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	while (awaiter.isAwaiting())
	{
		qApp->processEvents();
	}
	if (awaiter.wasTimeout())
	{
		userTip->setText(tr("mode_selection_timeout!"));
	}
	else
	{
        parse_uniresults_functions::PositionalResponse resp =
                RequestParser::interpretAsPositionalResponse(awaiter.restext, awaiter.errtext);
		if (resp.success)
		{
			settings = resp.values;
			innerWidget->hide();
			placeSelection->show();
			placeSelection->loadPlaces();
			current = placeSelection;
		}
	}
}

void ModeSelectionWidget::placeReady(parsedPlace pl)
{
	emit placeAndModeAcquired(settings);
}

void ModeSelectionWidget::hideCurrent()
{
	current->hide();
	innerWidget->show();
	current = innerWidget;
}

