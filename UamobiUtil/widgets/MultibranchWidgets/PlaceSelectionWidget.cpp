#include "PlaceSelectionWidget.h"
#include "widgets/utils/ElementsStyles.h"
// Qt 5 only imports
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif
#include "networking/Parsers/RequestParser.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/ExtendedDelegates/ZebraListItemDelegate.h"

void PlaceSelectionWidget::_handleRecord(RecEntity e)
{
	loadPlaces();
	if (e.isNull())
	{
#ifdef DEBUG
		detrace_METHPERROR("_handleRecord", "null entity provided");
#endif
		return;
	}
	if (e->myType() == UniformXmlObject::Mode)
	{
		Mode m = upcastRecord<ModeEntity>(e);
        userTip->setText(tr("please select place for mode: ") + m->name);
	}
}

PlaceSelectionWidget::PlaceSelectionWidget(QWidget* parent)
	: IndependentBranchNode(independent_nodes::PlaceSelect, true, parent),
	allplaces(new DataEntityListModel(this)),
	mainLayout(new QVBoxLayout(this)), buttonLayout(new QHBoxLayout(this)),
     userTip(new QLabel(this)),
	placeSelection(new QListView(this)),
	backButton(new MegaIconButton(this)), awaiter(new RequestAwaiter(AppSettings->timeoutInt, this)),
	place(new PlaceEntity()), loadPlacesQuery(), selectPlaceQuery()
{
#ifdef DEBUG
	detrace_DCONSTR("PlaceSelection");
#endif
	this->setLayout(mainLayout);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	buttonLayout->setContentsMargins(0, 0, 0, 0);
	buttonLayout->setSpacing(0);
    mainLayout->addWidget(userTip);
	mainLayout->addWidget(placeSelection);
	mainLayout->addLayout(buttonLayout);
	buttonLayout->addWidget(backButton);
	//buttonLayout->addStretch();
    userTip->setText(tr("please select place for mode: "));
	userTip->setFont(GENERAL_FONT);
    userTip->setAlignment(Qt::AlignCenter);
    userTip->setWordWrap(true);
	backButton->setText(tr("place_selection_back"));
	backButton->setIcon(QIcon(":/res/back.png"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	placeSelection->setModel(allplaces);
	placeSelection->setItemDelegate(new ZebraItemDelegate(this));
#ifdef QT_VERSION5X
	QObject::connect(allplaces, &DataEntityListModel::dataEntityClicked, this, &PlaceSelectionWidget::placeSelected);
	QObject::connect(placeSelection, &QListView::clicked, allplaces, &DataEntityListModel::mapClickToEntity);
	QObject::connect(backButton, &QPushButton::clicked, this, &PlaceSelectionWidget::backRequired);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &PlaceSelectionWidget::was_timeout);
#else
	QObject::connect(allplaces, SIGNAL(dataEntityClicked(RecEntity)), this, SLOT(placeSelected(RecEntity)));
   if (!QObject::connect(placeSelection, SIGNAL(clicked(QModelIndex)), allplaces, SLOT(mapClickToEntity(QModelIndex))))throw;
	QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
	QObject::connect(awaiter, SIGNAL(requestTimeout()), this, SLOT(was_timeout()));
#endif
}

void PlaceSelectionWidget::show()
{
	this->setFocus();
	inframedWidget::show();
}

bool PlaceSelectionWidget::isExpectingControl(int val)
{
	if (val >= -1 && val <= allplaces->rowCount() - 1)
	{
		if (val == -1)
		{
			if (allplaces->rowCount() > 10)
				val = 9;
			else
			{
				emit backRequired();
				return false;
			}
		}
		QModelIndex index = allplaces->index(val);
		if (index.isValid())
		{
			allplaces->mapClickToEntity(index);
			return true;
		}
		return true;
	}

	return false;
}


void PlaceSelectionWidget::placeSelected(RecEntity pl)
{
	if (awaiter->isAwaiting())
		return;
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &PlaceSelectionWidget::place_select_response);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(place_select_response()));
#endif
	place = upcastRecord(RecEntity(pl->clone()), place);
	if (selectPlaceQuery.isDefault())
    {
        QStringList t;
        t << place->getId();
        place->sendAssociatedPostRequest(t, awaiter);
    }
	else
		AppNetwork->execQueryByTemplate(selectPlaceQuery, place->getId(), awaiter);
}

void PlaceSelectionWidget::parse_loaded_places()
{
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	PolyResponse result = RequestParser::parseResponse(parser, RecEntity(place->clone()));
	if (result.isError)
	{
		userTip->setText(result.errtext);
#ifdef DEBUG
		detrace_NRESPERR(result.errtext);
#endif
	}
	else
	{
		allplaces->setData(result.objects);
	}
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void PlaceSelectionWidget::place_select_response()
{
	SimpliestResponceParser parser(awaiter->restext, awaiter->errtext);
	if (!parser.isSuccessfull())
	{
		userTip->setText(parser.getErrors());
#ifdef DEBUG
		detrace_NRESPERR(parser.getErrors());
#endif
	}
	else
	{
		emit done(RecEntity(place.staticCast<AbsRecEntity>()));
	}
	hideProcessingOverlay();
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), this, SLOT(place_select_response()));
}

void PlaceSelectionWidget::was_timeout()
{
	userTip->setText(tr("mode_selection_timeout!") + QString::number(AppSettings->timeoutInt));
	QObject::disconnect(awaiter, SIGNAL(requestReceived()), 0, 0);
	hideProcessingOverlay();
}

void PlaceSelectionWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	switch (
		((overloads.count() > 2) ? 2 : overloads.count())
		)
	{
	case 2:
    {
        QStringList t;
        t << "id";
		selectPlaceQuery = overloads.at(1).assertedAndMappedCopy(
            selectPlace, t,t);
    }
	case 1:
		loadPlacesQuery = overloads.at(0).assertedAndMappedCopy(
			placeList);
	default:
		break;
	}
}

void PlaceSelectionWidget::loadPlaces()
{
	if (awaiter->isAwaiting())
		return;
	showProcessingOverlay();
#ifdef QT_VERSION5X
	QObject::connect(awaiter, &RequestAwaiter::requestReceived, this, &PlaceSelectionWidget::parse_loaded_places);
#else
	QObject::connect(awaiter, SIGNAL(requestReceived()), this, SLOT(parse_loaded_places()));
#endif
	if (loadPlacesQuery.isDefault())
		place->sendAssociatedGetRequest(QStringList(), awaiter);
	else
		AppNetwork->execQueryByTemplate(loadPlacesQuery, awaiter);
}

void PlaceSelectionWidget::_sendDataRequest()
{

}
