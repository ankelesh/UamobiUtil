#pragma once

// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QListWidget>
#else
 // Qt 4 only imports
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QBoxLayout>
#include <QtGui/QListWidget>
#endif
#include "networking/RequestAwaiter.h"
#include "networking/Parsers/RequestParser.h"
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/GlobalAppSettings.h"

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
	QLabel* userInfo;
	QHBoxLayout* listHeaderLayout;
	QPushButton* previousButton;
	QLabel* indexationInfo;
	QPushButton* nextButton;
	QListWidget* itemInfoStorage;
	QHBoxLayout* footerLayout;
	QPushButton* backButton;
	QPushButton* saveButton;

	parse_uniresults_functions::doclistResponse items;
	int pagenumber;

public:
	DocResultsWidget(GlobalAppSettings & go, QWidget* parent);
	void loadItems();
	void refresh();
	void show() override;

protected slots:
	void previousPage();
	void nextPage();
	void saveDocument();

signals:
	void documentSaved();
};
