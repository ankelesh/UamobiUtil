#include "GroupSelectionWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "networking/Parsers/RequestParser.h"


int specwidgets::GroupSelectList::countElems()
{
	return coreGroupList.count();
}

void specwidgets::GroupSelectList::itemSelectedFromList(QListWidgetItem* it)
{
	emit groupSelected(coreGroupList.at(currentRow()));
}

QString specwidgets::GroupSelectList::elemToString(int ind)
{
	return coreGroupList.at(ind).name;
}

specwidgets::GroupSelectList::GroupSelectList(QVector<parsedGroup>& cgl, QWidget* parent)
	: AbstractListSelectionWidget(parent), coreGroupList(cgl)
{
	init();
}

GroupSelectionWidget::GroupSelectionWidget(GlobalAppSettings& go, QWidget* parent)
	: AbstractSearchAndPickWidget(go, parent, "groups", &DataUpdateEngine::recListTemplated, false), groups()
{
	userHelp->setText(tr("Group_selection"));
	specwidgets::GroupSelectList* gsel = new specwidgets::GroupSelectList(groups, this);

#ifdef QT_VERSION5X
	QObject::connect(gsel, &specwidgets::GroupSelectList::groupSelected, this, &GroupSelectionWidget::groupSelected);
#else
	QObject::connect(gsel, SIGNAL(groupSelected(parsedGroup)), this, SLOT(groupSelected(parsedGroup)));
#endif
	emplaceSelection(gsel);
}

void GroupSelectionWidget::groupSelected(parsedGroup gp)
{
	emit done(gp.code, "group");
}

void GroupSelectionWidget::interpretResult()
{
	groups = RequestParser::interpretAsGroupList(awaiter.restext, awaiter.errtext);
}
