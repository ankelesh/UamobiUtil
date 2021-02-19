#pragma once
#include "widgets/parents/IndependentBranchNode.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/MegaIconButton.h"
#ifdef QT_VERSION5X
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QListView>
#include <QtWidgets/QBoxLayout>
#else
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QListView>
#include <QtGui/QBoxLayout>
#endif
#include "widgets/utils/GlobalAppSettings.h"
#include "networking/RequestAwaiter.h"
#include "networking/things.h"


/*
	This widget is used to check and change filters which will apply to parent document's query.
	If there will be more entities with ability to check themselves, this widget can be
	made more abstract by removing bindings to entity fields.

*/
using QueryTemplates::QueryCache;

class FilterSelectWidget : public IndependentBranchNode
{
	Q_OBJECT
protected:
	DataEntityListModel* doctypes;

	QVBoxLayout* mainLayout;
	QLabel* title;
	QHBoxLayout* topPanelLayout;
	MegaIconButton* allonButton;
	MegaIconButton* alloffButton;
	QListView* typesel;
	QHBoxLayout* footerLayout;
	MegaIconButton* backButton;
	MegaIconButton* okButton;

	RequestAwaiter* awaiter;

	QueryCache localCache;

	void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
	virtual void _arrowReaction(int) override;
	virtual void _returnReaction() override;

public:
	FilterSelectWidget(QWidget* parent = Q_NULLPTR);
	virtual void setFocus() override;
	// sends request to list all filters. Deprecated, used only for separation
	void loadFilters();
protected slots:
	// checks all entities in model
	void checkAll();
	// unchecks entities in model
	void uncheckAll();
#ifdef QT_VERSION5X
	// swaps state to !state. Two definitions to avoid incompatibility on Qt4
    void changeState(const QModelIndex & index);
#else
    void changeState(QModelIndex  index);
#endif
	// sends all selected joined fields, then emits done
	void okPressed();
	void was_timeout();
	// sets model with new values
	void parse_doctype_list_response();
	// emits done if 200
	void parse_doctype_selection_response();
};
