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
#include "widgets/ElementWidgets/MegaIconButton.h"
// networking imports
#include "networking/RequestAwaiter.h"


using QueryTemplates::OverloadableQuery;
class PlaceSelectionWidget : public IndependentBranchNode // This widget is top of ModeSelection and allows place selection from obtained list
{
	Q_OBJECT
protected:
	// place list
	DataEntityListModel* allplaces;
	// widgets
	QVBoxLayout* mainLayout;
	QHBoxLayout* buttonLayout;
    QLabel* userTip;
	QListView* placeSelection;
	MegaIconButton* backButton;

	RequestAwaiter* awaiter;

	Place place;

	OverloadableQuery loadPlacesQuery;
	OverloadableQuery selectPlaceQuery;
	RecEntity entityToExtract;
	// Inherited via IndependentBranchNode
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
	void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
public:
	PlaceSelectionWidget(RecEntity toExtract, QWidget* parent = Q_NULLPTR);
	virtual void show() override;
    virtual bool isExpectingControl(int) override;
	void loadPlaces();
private slots:
	void placeSelected(RecEntity);	//	sends place_select request, then emits parsedPlace
	void parse_loaded_places();			//	netresponses
	void place_select_response();
	void was_timeout();	

};
