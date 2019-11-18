#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QLabel>
#else
 // Qt 4 only imports
#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QBoxLayout>
#include <QtGui/QLabel>

#endif
// widgets imports
#include "widgets/parents/inframedWidget.h"
#include "widgets/parents/AbstractListSelectionWidget.h"
#include "widgets/parents/abstractNodeInterface.h"
#include "widgets/ElementWidgets/MegaIconButton.h"
// networking imports
#include "networking/RequestAwaiter.h"
#include "networking/things.h"
// util imports
#include "widgets/utils/GlobalAppSettings.h"
#include "widgets/utils/ElementsStyles.h"



class AbstractSearchAndPickWidget : public inframedWidget
{
	Q_OBJECT
protected:
	GlobalAppSettings& globalSettings;

	TemplatedListRequest listItems;

	QVBoxLayout* mainLayout;
	inframedWidget* innerWidget;
	QVBoxLayout* innerLayout;
	QHBoxLayout* headerLayout;
	QHBoxLayout* footerLayout;
	QLabel* userHelp;
	QLineEdit* userinputField;
	MegaIconButton* searchButton;
	MegaIconButton* ordfilterButton;
	AbstractListSelectionWidget* itemSelection;
	MegaIconButton* backButton;

	QIcon withOrd;
	QIcon withoutOrd;
	RequestAwaiter awaiter;
	QString itemName;

	void emplaceSelection(AbstractListSelectionWidget* toEmplace);
	virtual void interpretResult()=0;
public:
	AbstractSearchAndPickWidget(GlobalAppSettings& go, QWidget* parent,
		QString itemname,
		TemplatedListRequest meth = &DataUpdateEngine::recListTemplated,
		bool isRequiredOrd = true);

	virtual void show() override;
protected slots:
	void searchPrimed();
	void ordFilterSwitched(bool);
	void parse_response();
	void was_timeout();

public slots:
	void loadItems();
};