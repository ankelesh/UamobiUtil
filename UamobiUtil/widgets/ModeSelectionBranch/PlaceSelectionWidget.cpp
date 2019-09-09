#include "PlaceSelectionWidget.h"


QString specwidgets::_placeSelectionWidget::elemAsString(int index)
{
	return places.at(index).name;
}

int specwidgets::_placeSelectionWidget::countElems()
{
	return places.count();
}

specwidgets::_placeSelectionWidget::_placeSelectionWidget(placesResponse& Places, QWidget* parent)
	: AbstractVariantSelectionWidget(parent), places(Places)
{
}

void specwidgets::_placeSelectionWidget::indexSelected(int Index)
{
	emit placeSelected(places.at(Index));
}

PlaceSelectionWidget::PlaceSelectionWidget(const GlobalAppSettings& go, QWidget* parent)
	: inframedWidget(parent), globalSettings(go), allplaces(),
	mainLayout(new QVBoxLayout(this)), buttonLayout(new QHBoxLayout(this)),
	scrArea(new QScrollArea(this)), userTip(new QLabel(this)),
	modeTip(new QLabel(this)), placesTip(new QLabel(this)),
	placeSelection(new specwidgets::_placeSelectionWidget(allplaces, scrArea)),
	backButton(new QPushButton(this))
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(userTip);
	mainLayout->addWidget(modeTip);
	mainLayout->addWidget(placesTip);
	scrArea->setWidgetResizable(true);
	mainLayout->addWidget(scrArea);
	mainLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(backButton);
	buttonLayout->addStretch();

	userTip->setText(tr("place_selection_user_tip!"));
	modeTip->setText(tr("place_selection_mode_tip: "));
	placesTip->setText(tr("place_selection_place_tip: "));
	backButton->setText(tr("place_selection_back"));


	scrArea->setWidget(placeSelection);

#ifdef QT_VERSION5X
	QObject::connect(placeSelection, &specwidgets::_placeSelectionWidget::placeSelected, this, &PlaceSelectionWidget::placeSelected);
	QObject::connect(backButton, &QPushButton::clicked, this, &PlaceSelectionWidget::backRequired);
#else
    QObject::connect(placeSelection, SIGNAL(placeSelected(parsedPlace)), this, SLOT(placeSelected(parsedPlace)));
    QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
#endif

}

void PlaceSelectionWidget::placeSelected(parsedPlace pl)
{
	RequestAwaiter awaiter(globalSettings.timeoutInt, this);
	globalSettings.networkingEngine->placeSelect(pl.code, &awaiter, RECEIVER_SLOT_NAME);
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
		if (RequestParser::interpretAsSimpliestResponse(awaiter.restext, awaiter.errtext).resp)
			emit placeAcquired(pl);
	}
}

void PlaceSelectionWidget::loadPlaces()
{
	RequestAwaiter awaiter;
	globalSettings.networkingEngine->placeList(&awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
	while (awaiter.isAwaiting())
	{
		qApp->processEvents();
	}
	if (awaiter.wasTimeout())
	{
		detrace_METHEXPL("timeout")
		userTip->setText(tr("places_selection_timeout!"));
	}
	else
	{
		detrace_METHEXPL("loaded")
		allplaces = RequestParser::interpretAsPlaceList(awaiter.restext, awaiter.errtext);
		placeSelection->reload();
	}
}

