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
	This widget is created to wrap lists with paging navigation.
	It sends associated get request of entity with supplied page number,
	then awaits for request. On select this does not calls netrequest.

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
	virtual void focusInEvent(QFocusEvent*) override;
	// fills indexation labels and enables navigation buttons
	void setIndexation(XmlObjects items);
public:
	PagedSearchWidget(RecEntity proto, QWidget* parent);
	// parses response and fills entity model
	void refresh();
	
public slots:
	// wraps loadResult to drop counter before load
	void doSearch();		
	void nextPage();		//	sends request for currentpage++;
	void previousPage();	//	sends request for currentpage--;
	void loadResults();		//	unwrapped slot for loading data
	void search_response();
	void was_timeout();   
};
