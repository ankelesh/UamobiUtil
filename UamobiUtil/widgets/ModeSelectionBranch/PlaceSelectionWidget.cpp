#include "PlaceSelectionWidget.h"
#include "widgets/utils/ElementsStyles.h"
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QScroller>

#else
 // Qt 4 only imports
#include "legacy/qtCompatibility/scrollgrabber.h"
#endif
#ifdef DEBUG
#include "debugtrace.h"
#endif
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

PlaceSelectionWidget::PlaceSelectionWidget(const GlobalAppSettings& go, QWidget* parent,
	NoArgsRequestMP lplaces, interpretsPointers::interpretAsPlaceLike inter)
	: inframedWidget(parent), globalSettings(go), listPlaces(lplaces), interpreter(inter), allplaces(),
	mainLayout(new QVBoxLayout(this)), buttonLayout(new QHBoxLayout(this)),
	scrArea(new QScrollArea(this)), userTip(new QLabel(this)),
	modeTip(new QLabel(this)), placesTip(new QLabel(this)),
	placeSelection(new specwidgets::_placeSelectionWidget(allplaces, scrArea)),
	backButton(new MegaIconButton(this)), awaiter(go.timeoutInt, this)
{
	this->setLayout(mainLayout);
	mainLayout->addWidget(userTip);
	mainLayout->addWidget(modeTip);
	mainLayout->addWidget(placesTip);
	scrArea->setWidgetResizable(true);
	mainLayout->addWidget(scrArea);
	mainLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(backButton);
	//buttonLayout->addStretch();

	userTip->setText(tr("place_selection_user_tip!"));
	userTip->setStyleSheet(countAdaptiveFont(0.03));
	userTip->setAlignment(Qt::AlignCenter);

	modeTip->setText(tr("place_selection_mode_tip: "));
	modeTip->setStyleSheet(countAdaptiveFont(0.03));
	modeTip->setAlignment(Qt::AlignCenter);
	
	placesTip->setText(tr("place_selection_place_tip: "));
	placesTip->setStyleSheet(countAdaptiveFont(0.03));
	placesTip->setAlignment(Qt::AlignCenter);

	backButton->setText(tr("place_selection_back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);


    scrArea->setWidget(placeSelection);
	QScroller::grabGesture(scrArea, QScroller::LeftMouseButtonGesture);


#ifdef QT_VERSION5X
	QObject::connect(placeSelection, &specwidgets::_placeSelectionWidget::placeSelected, this, &PlaceSelectionWidget::placeSelected);
	QObject::connect(backButton, &QPushButton::clicked, this, &PlaceSelectionWidget::backRequired);
	QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &PlaceSelectionWidget::was_timeout);
#else
	QObject::connect(placeSelection, SIGNAL(placeSelected(parsedPlace)), this, SLOT(placeSelected(parsedPlace)));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
#endif
}

void PlaceSelectionWidget::show()
{
	this->setFocus();
	inframedWidget::show();
}

bool PlaceSelectionWidget::isExpectingControl(int val)
{
	if (val >= -1 && val < allplaces.count() - 1)
	{
		if (val == -1)
		{
			if (allplaces.count() > 10)
				val = 9;
			else
			{
				emit backRequired();
				return false;
			}
		}
		placeSelected(allplaces.at(val));
		return true;

	}


	return false;
}

void PlaceSelectionWidget::placeSelected(parsedPlace pl)
{
	if (awaiter.isAwaiting())
		return;
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &PlaceSelectionWidget::place_select_response);
	globalSettings.networkingEngine->placeSelect(pl.code, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
}

void PlaceSelectionWidget::parse_loaded_places()
{
#ifdef DEBUG
	detrace_METHEXPL("loaded");
#endif
	allplaces = interpreter(awaiter.restext, awaiter.errtext);
	placeSelection->reload();
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(parse_loaded_places()));
}


void PlaceSelectionWidget::place_select_response()
{
	if (RequestParser::interpretAsSimpliestResponse(awaiter.restext, awaiter.errtext).resp)
		emit placeAcquired(pl);
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(place_select_response()));
}

void PlaceSelectionWidget::was_timeout()
{
	userTip->setText(tr("mode_selection_timeout!") + QString::number(globalSettings.timeoutInt));
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(place_select_response()));
	awaiter.disconnect(SIGNAL(requestReceived()), this, SLOT(parse_loaded_places()));
}

void PlaceSelectionWidget::loadPlaces()
{
	if (awaiter.isAwaiting())
		return;
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &PlaceSelectionWidget::parse_loaded_places);
	DataUpdateEngine* httpointer = (globalSettings.networkingEngine);
	(*httpointer.*listPlaces)(&awaiter, RECEIVER_SLOT_NAME);
}
