#pragma once

// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#else
 // Qt 4 only imports
#include <QtGui/QBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#endif
#include "networking/RequestAwaiter.h"
#include "widgets/parents/IndependentBranchNode.h"
#include "widgets/ElementWidgets/MegaIconButton.h"

/*
	This widget is concentrated on forming list widget around response.
	It provides input line for user and trigger-button. On selection is emitted signal
	containing parsedItem.

	__ASSOCIATED_DATABASE_FUNCTION__  :  P'searchResponse' doc_search_items()

*/
using QueryTemplates::OverloadableQuery;
class PagedSearchWidget : public IndependentBranchNode
{
	Q_OBJECT
protected:
	DataEntityListModel * entityModel;
	RecEntity prototype;

	QVBoxLayout* mainLayout;
	QHBoxLayout* searchPanel;
	QLineEdit* searchInput;
	MegaIconButton* searchButton;
	QHBoxLayout* listHeaderLayout;
	MegaIconButton* previousButton;
	MegaIconButton* nextButton;
	QLabel* indexationInfo;
	QListView* itemList;
	QHBoxLayout* footerLayout;
	MegaIconButton* backButton;

	int currentpage;
	QString toSearch;
	RequestAwaiter* awaiter;

	OverloadableQuery loadDataQuery;

	virtual void _handleRecord(RecEntity) override;
	virtual void _sendDataRequest() override;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
	void setIndexation(XmlObjects items);
public:
	PagedSearchWidget(RecEntity proto, QWidget* parent);
	void refresh();
public slots:
	void doSearch();		//	triggerSlot
	void loadResults();		//	inner slot for direct calling
	void clear();			//	clears field and list
	void nextPage();		//	sends request for next page
	void previousPage();	//	sends request for prev page
	void search_response();	//	netresponse functions
	void was_timeout();
};
