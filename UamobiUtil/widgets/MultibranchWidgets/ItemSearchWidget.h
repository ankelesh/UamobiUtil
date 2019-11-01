#pragma once

// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#else
 // Qt 4 only imports
#include <QtGui/QBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#endif
#include "widgets/parents/AbstractListSelectionWidget.h"
#include "networking/RequestAwaiter.h"
#include "networking/Parsers/RequestParser.h"
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/ElementWidgets/MegaIconButton.h"

/*
	This widget is concentrated on forming list widget around response. 
	It provides input line for user and trigger-button. On selection is emitted signal
	containing parsedItem.

	__ASSOCIATED_DATABASE_FUNCTION__  :  P'searchResponse' doc_search_items()

*/


namespace specwidgets
{
	class _ItemSelectionList : public AbstractListSelectionWidget
		// specialization for items
	{
		Q_OBJECT
	protected:
		QVector<parsedItemSimplified> & allitems;
		// Inherited via AbstractListSelectionWidget
		virtual int countElems() override;
		virtual void itemSelectedFromList(QListWidgetItem*) override;
		virtual QString elemToString(int);

	public:
		_ItemSelectionList(parse_uniresults_functions::searchResponse & items, QWidget * parent = Q_NULLPTR);

	signals:
		void itemPicked(parsedItemSimplified);
	};
}


class ItemSearchWidget : public inframedWidget
{
	Q_OBJECT
protected:
	parse_uniresults_functions::searchResponse allitems;
	GlobalAppSettings& globalSettings;

	QVBoxLayout* mainLayout;
	QHBoxLayout* searchPanel;
	QLineEdit* searchInput;
	MegaIconButton* searchButton;
	QHBoxLayout* listHeaderLayout;
	MegaIconButton* previousButton;
	MegaIconButton* nextButton;
	QLabel* indexationInfo;
	specwidgets::_ItemSelectionList* itemList;
	QHBoxLayout* footerLayout;
	MegaIconButton* backButton;

	int currentpage;
	QString toSearch;
	RequestAwaiter awaiter;
public:
	ItemSearchWidget(GlobalAppSettings & go, QWidget* parent);
	void refresh();
public slots:
	void doSearch();		//	triggerSlot
	void loadResults();		//	inner slot for direct calling
	void clear();			//	clears field and list
	void nextPage();		//	sends request for next page
	void previousPage();	//	sends request for prev page
	void search_response();	//	netresponse functions
	void was_timeout();
signals:
	void itemSelected(parsedItemSimplified);	//	is emitted when item picked
};
