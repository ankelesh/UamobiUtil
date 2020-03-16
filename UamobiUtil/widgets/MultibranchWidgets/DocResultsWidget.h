#pragma once

// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QLabel>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QListView>
#else
 // Qt 4 only imports
#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>
#include <QtGui/QListView>
#endif
#include "networking/RequestAwaiter.h"
#include "networking/Parsers/RequestParser.h"
#include "widgets/parents/IndependentBranchNode.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/MultibranchWidgets/ScaningRelated/ControlListWidget.h"
#include "networking/things.h"
/*
	This widget provides realization of list containing all items included in current document.
	It expects paged result and allows some extra operations over results - like delete.
	Also it has "save and exit" button which will save current state of the document.
	It has abstraction node interfaces for future options - possibly will be required redacting or 
	more widgets on select.
*/

using QueryTemplates::OverloadableQuery;
using QueryTemplates::QueryCache;

class DocResultsWidget : public IndependentBranchNode, abstractNode
	// Provides operations over document 
{
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QHBoxLayout* toolPanel;
	MegaIconButton* deleteAllButton;
	MegaIconButton* deleteSelectedButton;

	QLabel* userInfo;
	QHBoxLayout* listHeaderLayout;
	MegaIconButton* previousButton;
	QLabel* indexationInfo;
	MegaIconButton* nextButton;
	QListView * itemInfoStorage;
	QHBoxLayout* footerLayout;
	MegaIconButton* backButton;
	MegaIconButton* saveButton;

	DataEntityListModel* items;
	int pagenumber;
	RequestAwaiter* awaiter;

	QueryCache localCache;

	ControlListWidget* attachedControls;

	void _handleRecord(RecEntity) override;
	// fills indexation label and enables navigation buttons
	void setIndexation(XmlObjects& settings);
	virtual void _sendDataRequest() override;
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;
	void getAttachedControls();
public:
	DocResultsWidget(QWidget* parent);
	// sends request for full item list
	void loadItems();
	// parses response and sets indexation and model
	void refresh();
protected slots:
	// decrements counter and loads items
	void previousPage();
	// increments counter and loads items
	void nextPage();
	// sends save request
	void saveDocument();


	// receives response and hides overlay. Renundant now.
	void items_response();			
	// checks response to be only one character - '_'. If true, done is emitted
	void save_response();

	void get_attached_response();

	virtual void attachedControlsDone();

	void was_timeout();
	// sends delete all query
	void deleteAll();
	// sends delete by barcode query using current selected item's id
	virtual void handleDelete();

	void hideCurrent();
};
