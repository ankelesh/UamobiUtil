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
#ifdef DEBUG
#include "debugtrace.h"
#endif


void PlaceSelectionWidget::_handleRecord(RecEntity e)
{
	if (e.isNull())
		return;
	if (e->myType() == UniformXmlObject::Mode)
	{
		setMode(upcastRecord<ModeEntity>(e));
	}
	loadPlaces();
}

PlaceSelectionWidget::PlaceSelectionWidget(QWidget* parent,
	QueryTemplates::QueryId listPlacesQuery)
	: IndependentBranchNode(independent_nodes::PlaceSelect, true, parent), 
	listPlacesQueryId(listPlacesQuery), 
	allplaces(new DataEntityListModel(this)),
	mainLayout(new QVBoxLayout(this)), buttonLayout(new QHBoxLayout(this)),
	 userTip(new QLabel(this)),
	modeTip(new QLabel(this)), placesTip(new QLabel(this)),
	placeSelection(new QListView(this)),
	backButton(new MegaIconButton(this)), awaiter(new RequestAwaiter(AppSettings->timeoutInt, this)),
	place(new PlaceEntity())
{
	this->setLayout(mainLayout);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);
	buttonLayout->setContentsMargins(0, 0, 0, 0);
	buttonLayout->setSpacing(0);
	mainLayout->addWidget(userTip);
	mainLayout->addWidget(modeTip);
	mainLayout->addWidget(placesTip);
	mainLayout->addWidget(placeSelection);
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

	placeSelection->setModel(allplaces);
#ifdef QT_VERSION5X
	QObject::connect(allplaces, &DataEntityListModel::dataEntityClicked, this, &PlaceSelectionWidget::placeSelected);
	QObject::connect(placeSelection, &QListView::clicked, allplaces, &DataEntityListModel::mapClickToEntity);
	QObject::connect(backButton, &QPushButton::clicked, this, &PlaceSelectionWidget::backRequired);
	QObject::connect(awaiter, &RequestAwaiter::requestTimeout, this, &PlaceSelectionWidget::was_timeout);
#else
	QObject::connect(allplaces, SIGNAL(dataEntityClicked(RecEntity)), this, SLOT(placeSelected(RecEntity)));
	QObject::connect(placeSelection, SIGNAL(clicked(const QModelIndex&)), this, SLOT(mapClickToEntity(const QModelIndex&)));
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

void PlaceSelectionWidget::setMode(Mode pmode)
{
	modeTip->setText(tr("place_selection_mode_tip: ") + pmode->name);
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
	place->sendAssociatedPostRequest(QStringList(), awaiter);
}

void PlaceSelectionWidget::parse_loaded_places()
{
	ResponseParser parser(new LinearListParser(awaiter->restext, awaiter->errtext));
	PolyResponse result = RequestParser::parseResponse(parser, RecEntity(place->clone()));
	if (result.isError)
	{
		userTip->setText(result.errtext);
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
	place->adjustQueryAndUseGet(listPlacesQueryId, awaiter);
}

void PlaceSelectionWidget::_sendDataRequest()
{
}
