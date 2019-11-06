#pragma once
#include "widgets/parents/AbstractCheckboxSelection.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
#ifdef QT_VERSION5X
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#else
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#endif
#include "widgets/utils/GlobalAppSettings.h"
#include "networking/RequestAwaiter.h"
#include "networking/things.h"


/*
	This widget is used to check and change filters which will apply to parent document's query.


	__ASSOCIATED_DATABASE_FUNCTION__   :  P'docFilterResponse' doc_get_allowed_types
	__ASSOCIATED_DATABASE_FUNCTION__   :  P'typicalResponse'   doc_select_filter

*/
namespace specwidgets {
	class TypeCheckboxSelection : public AbstractCheckboxSelection
	{
		Q_OBJECT
	protected:
		QVector<parsedDocType>& alltypes;
		// Inherited via AbstractCheckboxSelection
		virtual QString elemAsString(int index) override;
		virtual int countElems() override;
	public:
		TypeCheckboxSelection(QVector<parsedDocType>& at, QVector<bool>& statesv, QWidget* parent);
	};
}

class FilterSelectWidget : public inframedWidget
{
	Q_OBJECT
protected:
	GlobalAppSettings& globalSettings;
	QVector<parsedDocType> doctypes;
	QVector<bool> selectionState;

	QVBoxLayout* mainLayout;
	QLabel* title;
	QHBoxLayout* topPanelLayout;
	MegaIconButton* allonButton;
	MegaIconButton* alloffButton;
	QScrollArea* scrArea;
	specwidgets::TypeCheckboxSelection* typesel;
	QHBoxLayout* footerLayout;
	MegaIconButton* backButton;
	MegaIconButton* okButton;

	RequestAwaiter awaiter;
public:
	FilterSelectWidget(GlobalAppSettings& go, QWidget* parent = Q_NULLPTR);
	void loadFilters();
protected slots:
	void checkAll();
	void uncheckAll();
	void okPressed();
	void was_timeout();
	void parse_doctype_list_response();
	void parse_doctype_selection_response();
signals:
	void filterApplied();
};
