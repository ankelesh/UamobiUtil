#pragma once
// Qt 5 only widgets
#ifdef QT_VERSION5X
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QPushButton>
#else
// Qt 4 widgets
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QScrollArea>
#include <QtGui/QLabel>
#endif

// Qt imports
#include <QtCore/QString>
// widgets imports
#include "widgets/parents/inframedWidget.h"
#include "widgets/parents/AbstractVariantSelectionWidget.h"
// networking imports
#include "networking/Parsers/RequestParser.h"
#include "networking/RequestAwaiter.h"
// utils imports
#include "widgets/utils/GlobalAppSettings.h"

/*
	This widget is used to select place. Generally it's behaviour is similar with ModeSelectionWidget, but this widget
	is top one of its branch.
*/


using parse_uniresults_functions::placesResponse;
namespace specwidgets
	// This namespace must contain transformed base widgets with minor changes and specializations of abstract widgets
{
	class _placeSelectionWidget : public AbstractVariantSelectionWidget // This class wraps places list
	{
		Q_OBJECT
	private:
		placesResponse& places;
	protected:
		// Inherited via AbstractVariantSelectionWidget
		virtual QString elemAsString(int index) override;
		virtual int countElems() override;
	public:
		_placeSelectionWidget(placesResponse& Places, QWidget* parent);
	private slots:
		virtual void indexSelected(int Index);
	signals:
		void placeSelected(parsedPlace);
	};
}

class PlaceSelectionWidget : public inframedWidget // This widget is top of ModeSelection and allows place selection from obtained list
{
	Q_OBJECT
protected:
	// uses GAS
	const GlobalAppSettings& globalSettings;
	// place list
	placesResponse allplaces;
	// widgets
	QVBoxLayout* mainLayout;
	QHBoxLayout* buttonLayout;
	QScrollArea* scrArea;
	QLabel* userTip;
	QLabel* modeTip;
	QLabel* placesTip;
	specwidgets::_placeSelectionWidget* placeSelection;
	QPushButton* backButton;

public:
	PlaceSelectionWidget(const GlobalAppSettings& go, QWidget* parent = Q_NULLPTR);
private slots:
	void placeSelected(parsedPlace);	//	sends place_select request, then emits parsedPlace
public slots:
	void loadPlaces();					//	loads place list from web
signals:
	void placeAcquired(parsedPlace);	//	is emitted when place_request is succesfull
};