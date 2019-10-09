#pragma once

// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#else
 // Qt 4 only imports
#include <QtGui/qpushbutton.h>
#include <QtGui/QBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#endif
#include "widgets/parents/AbstractListSelectionWidget.h"
#include "networking/RequestAwaiter.h"
#include "networking/Parsers/RequestParser.h"
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/GlobalAppSettings.h"

namespace specwidgets
{
	class _ItemSelectionList : public AbstractListSelectionWidget
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
	QPushButton* searchButton;
	QHBoxLayout* listHeaderLayout;
	QPushButton* previousButton;
	QPushButton* nextButton;
	QLabel* indexationInfo;
	specwidgets::_ItemSelectionList* itemList;
	QHBoxLayout* footerLayout;
	QPushButton* backButton;

	int currentpage;
	QString toSearch;

public:
	ItemSearchWidget(GlobalAppSettings & go, QWidget* parent);
	void refresh();
public slots:
	void doSearch();
	void loadResults();
	void clear();
	void nextPage();
	void previousPage();

signals:
	void itemSelected(parsedItemSimplified);
};
