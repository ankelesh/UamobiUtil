#include "PlaceSelectionWidget.h"
#include "widgets/utils/ElementsStyles.h"
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QScroller>

#else
 // Qt 4 only imports
#include "legacy/qtCompatibility/scrollgrabber.h"
#endif
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#define DEBUG
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
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	buttonLayout->setContentsMargins(0, 0, 0, 0);
	buttonLayout->setSpacing(0);
	mainLayout->addWidget(userTip);
	mainLayout->addWidget(modeTip);
	mainLayout->addWidget(placesTip);
	scrArea->setWidgetResizable(true);
	mainLayout->addWidget(scrArea);
	mainLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(backButton);
	//buttonLayout->addStretch();
	QFont scf = makeFont(0.04);
	userTip->setText(tr("place_selection_user_tip!"));
	userTip->setFont(scf);
	userTip->setAlignment(Qt::AlignCenter);

	modeTip->setText(tr("place_selection_mode_tip: "));
	modeTip->setFont(scf);
	modeTip->setAlignment(Qt::AlignCenter);

	placesTip->setText(tr("place_selection_place_tip: "));
	placesTip->setFont(scf);
	placesTip->setAlignment(Qt::AlignCenter);

	backButton->setText(tr("place_selection_back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	scrArea->setWidget(placeSelection);
	QScroller::grabGesture(scrArea, QScroller::LeftMouseButtonGesture);
	placeSelection->installEventFilter(keyfilter);

#ifdef QT_VERSION5X
	QObject::connect(placeSelection, &specwidgets::_placeSelectionWidget::placeSelected, this, &PlaceSelectionWidget::placeSelected);
	QObject::connect(backButton, &QPushButton::clicked, this, &PlaceSelectionWidget::backRequired);
	QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &PlaceSelectionWidget::was_timeout);
#else
	QObject::connect(placeSelection, SIGNAL(placeSelected(parsedPlace)), this, SLOT(placeSelected(parsedPlace)));
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(&awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
#endif
}

void PlaceSelectionWidget::show()
{
	this->setFocus();
	inframedWidget::show();
}

bool PlaceSelectionWidget::isExpectingControl(int val)
{
	if (val >= -1 && val <= allplaces.count() - 1)
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
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &PlaceSelectionWidget::place_select_response);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(place_select_response()));
#endif
	globalSettings.networkingEngine->placeSelect(pl.code, &awaiter, RECEIVER_SLOT_NAME);
	awaiter.run();
}

void PlaceSelectionWidget::parse_loaded_places()
{
	allplaces = interpreter(awaiter.restext, awaiter.errtext);
#ifdef DEBUG
	parse_uniresults_functions::placesResponse temp;
	for (int i = 0; i < allplaces.count(); ++i)
	{
		if (allplaces.at(i).name.startsWith("L-73"))
			temp << allplaces.at(i);
	}
	allplaces = temp;
#endif

	placeSelection->reload();
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void PlaceSelectionWidget::place_select_response()
{
	hideProcessingOverlay();
	if (RequestParser::interpretAsSimpliestResponse(awaiter.restext, awaiter.errtext).resp)
		emit placeAcquired(pl);
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), this, SLOT(place_select_response()));
}

void PlaceSelectionWidget::was_timeout()
{
	userTip->setText(tr("mode_selection_timeout!") + QString::number(globalSettings.timeoutInt));
	QObject::disconnect(&awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void PlaceSelectionWidget::loadPlaces()
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &PlaceSelectionWidget::parse_loaded_places);
#else
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(parse_loaded_places()));
#endif
	DataUpdateEngine* httpointer = (globalSettings.networkingEngine);
	(*httpointer.*listPlaces)(&awaiter, RECEIVER_SLOT_NAME);
}