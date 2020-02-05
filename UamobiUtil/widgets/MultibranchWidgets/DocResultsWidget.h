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
#include "networking/things.h"
/*
	This widget provides realization of list containing all items included in current document.

	__ASSOCIATED_DATABASE_FUNCTION__   :  doc_list_items

*/

class DocResultsWidget : public IndependentBranchNode, abstractNode
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

	void _handleRecord(RecEntity) override;
	void setIndexation(XmlObjects& settings);
	virtual void _sendDataRequest() override;
public:
	DocResultsWidget(QWidget* parent);
	void loadItems();
	void refresh();
	void show() override;
	void clear();
protected slots:
	void previousPage();
	void nextPage();
	void saveDocument();
	void items_response();			//	netresponse
	void save_response();
	void was_timeout();
	void deleteAll();
	void deleteCurrent();
signals:
	void documentSaved();

};
