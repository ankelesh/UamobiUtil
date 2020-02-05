#pragma once
#include "widgets/parents/AbstractCheckboxSelection.h"
#include "widgets/parents/IndependentBranchNode.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
#ifdef QT_VERSION5X
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QListView>
#else
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QListView>
#endif
#include "widgets/utils/GlobalAppSettings.h"
#include "networking/RequestAwaiter.h"
#include "networking/things.h"


/*
	This widget is used to check and change filters which will apply to parent document's query.


	__ASSOCIATED_DATABASE_FUNCTION__   :  P'docFilterResponse' doc_get_allowed_types
	__ASSOCIATED_DATABASE_FUNCTION__   :  P'typicalResponse'   doc_select_filter

*/


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
	void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
public:
	FilterSelectWidget(QWidget* parent = Q_NULLPTR);
	void loadFilters();
protected slots:
	void checkAll();
	void uncheckAll();
	void changeState(const QModelIndex& index);
	void okPressed();
	void was_timeout();
	void parse_doctype_list_response();
	void parse_doctype_selection_response();
signals:
	void filterApplied();
};
