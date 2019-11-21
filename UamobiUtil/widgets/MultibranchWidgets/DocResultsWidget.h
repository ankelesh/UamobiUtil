#pragma once

// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QLabel>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QListWidget>
#else
 // Qt 4 only imports
#include <QtGui/QLabel>
#include <QtGui/QBoxLayout>
#include <QtGui/QListWidget>
#endif
#include "networking/RequestAwaiter.h"
#include "networking/Parsers/RequestParser.h"
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/ElementWidgets/MegaIconButton.h"

/*
	This widget provides realization of list containing all items included in current document.

	__ASSOCIATED_DATABASE_FUNCTION__   :  doc_list_items

*/

class DocResultsWidget : public inframedWidget
{
	Q_OBJECT
protected:
	GlobalAppSettings& globalSettings;

	QVBoxLayout* mainLayout;
	QHBoxLayout* toolPanel;
	MegaIconButton* deleteAllButton;
	MegaIconButton* deleteSelectedButton;

	QLabel* userInfo;
	QHBoxLayout* listHeaderLayout;
	MegaIconButton* previousButton;
	QLabel* indexationInfo;
	MegaIconButton* nextButton;
	QListWidget* itemInfoStorage;
	QHBoxLayout* footerLayout;
	MegaIconButton* backButton;
	MegaIconButton* saveButton;

	parse_uniresults_functions::doclistResponse items;
	int pagenumber;
	RequestAwaiter awaiter;
public:
	DocResultsWidget(GlobalAppSettings& go, QWidget* parent);
	void loadItems();
	void refresh();
	void show() override;

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
