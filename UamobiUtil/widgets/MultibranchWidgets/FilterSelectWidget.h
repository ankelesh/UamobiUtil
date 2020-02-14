#pragma once
#include "widgets/parents/IndependentBranchNode.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
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


	__ASSOCIATED_DATABASE_FUNCTION__   :  P'docFilterResponse' doc_get_allowed_types
	__ASSOCIATED_DATABASE_FUNCTION__   :  P'typicalResponse'   doc_select_filter

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
public:
	FilterSelectWidget(QWidget* parent = Q_NULLPTR);
	void loadFilters();
protected slots:
	void checkAll();
	void uncheckAll();
#ifdef QT_VERSION5X
    void changeState(const QModelIndex & index);
#else
    void changeState(QModelIndex  index);
#endif
	void okPressed();
	void was_timeout();
	void parse_doctype_list_response();
	void parse_doctype_selection_response();
};
