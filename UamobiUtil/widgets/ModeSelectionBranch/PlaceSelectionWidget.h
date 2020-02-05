#pragma once
// Qt 5 only widgets
#ifdef QT_VERSION5X
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QListView>
#else
// Qt 4 widgets
#include <QtGui/QVBoxLayout>
#include <QtGui/QScrollArea>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#endif

// Qt imports
#include <QtCore/QString>
// widgets imports
#include "widgets/parents/IndependentBranchNode.h"
#include "widgets/parents/AbstractVariantSelectionWidget.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
// networking imports
#include "networking/Parsers/RequestParser.h"
#include "networking/RequestAwaiter.h"
// utils imports
#include "widgets/utils/GlobalAppSettings.h"

/*
	This widget is used to select place. Generally it's behaviour is similar with ModeSelectionWidget, but this widget
	is top one of its branch.
	update:
	fixed minor bugs, added chaining to ordersSelect (! possible this is renundant, think about adding another subclass particulary for
	branch node version

	__ASSOCIATED_DATABASE_FUNCTION__  :   P'placesResponse'  list_places()
	__ASSOCIATED_DATABASE_FUNCTION__  :   P'simpliestResponse'   select_place(place_code)
*/


class PlaceSelectionWidget : public IndependentBranchNode // This widget is top of ModeSelection and allows place selection from obtained list
{
	Q_OBJECT
protected:
	// pointer to function used for fetching place list
	QueryTemplates::QueryId listPlacesQueryId;
	// place list
	DataEntityListModel* allplaces;
	// widgets
	QVBoxLayout* mainLayout;
	QHBoxLayout* buttonLayout;
	QLabel* userTip;
	QLabel* modeTip;
	QLabel* placesTip;
	QListView* placeSelection;
	MegaIconButton* backButton;

	RequestAwaiter* awaiter;

	Place place;

	void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
public:
	PlaceSelectionWidget( QWidget* parent = Q_NULLPTR,
		QueryTemplates::QueryId listPlacesQuery = QueryTemplates::placeList
	);
	virtual void show() override;
	virtual bool isExpectingControl(int);
	void setMode(Mode);
private slots:
	void placeSelected(RecEntity);	//	sends place_select request, then emits parsedPlace
	void parse_loaded_places();			//	netresponses
	void place_select_response();
	void was_timeout();
public slots:
	void loadPlaces();			
	// Inherited via IndependentBranchNode
	//	is emitted when place_request is succesfull
};