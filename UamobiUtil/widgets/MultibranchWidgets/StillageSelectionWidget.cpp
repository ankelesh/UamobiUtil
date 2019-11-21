#include "StillageSelectionWidget.h"
#include "networking/Parsers/RequestParser.h"

#ifdef DEBUG
#include "debugtrace.h"
#endif

int specwidgets::StillageSelectList::countElems()
{
	return coreStillList.count();
}

void specwidgets::StillageSelectList::itemSelectedFromList(QListWidgetItem*)
{
	emit stillageSelected(coreStillList.at(currentRow()));
}

QString specwidgets::StillageSelectList::elemToString(int ind)
{
	return coreStillList.at(ind).name;
}

specwidgets::StillageSelectList::StillageSelectList(QVector<parsedStillage>& cgl, QWidget* parent)
	: AbstractListSelectionWidget(parent), coreStillList(cgl)
{
}

void StillageSelectionWidget::interpretResult()
{

	stills = RequestParser::interpretAsStillageList(awaiter.restext, awaiter.errtext);
}

StillageSelectionWidget::StillageSelectionWidget(GlobalAppSettings& go, QWidget* parent)
	: AbstractSearchAndPickWidget(go, parent, "stillages", &DataUpdateEngine::recListTemplated, false), stills()
{
	specwidgets::StillageSelectList* gsel = new specwidgets::StillageSelectList(stills, this);
#ifdef QT_VERSION5X
	QObject::connect(gsel, &specwidgets::StillageSelectList::stillageSelected, this, &StillageSelectionWidget::stillageSelected);
#else
	QObject::connect(gsel, SIGNAL(stillageSelected(parsedStillage)), this, SLOT(stillageSelected(parsedStillage)));
#endif
	emplaceSelection(gsel);
}

void StillageSelectionWidget::stillageSelected(parsedStillage s)
{
	emit done(s.code, "stillage");
}
