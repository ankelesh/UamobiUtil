#include "ControlManager.h"

void ControlManager::allocateIntControl(QString& name)
{
}

ControlManager::ControlManager(QWidget* parent, QLayout* layout)
	: QObject(parent), controlsList(), currentControl("None"),
	doesNeedValues(false), controls(0), whereToPlace(layout)
{
	
}

bool ControlManager::parseControls(QStringList list)
{
	QStringList::iterator start = list.begin();
	while (start != list.end())
	{
		switch (start->count())
		{
		case 3:
			if (*start == "qty")
			{
				
			}
		}
		++start;
	}
	return false;
}

bool ControlManager::parseControls(QString)
{
	return false;
}

void ControlManager::setControl(QString)
{
}

void ControlManager::setControl(int)
{
}

void ControlManager::setCurrentValue(QString)
{
}

void ControlManager::setCurrentValue(int)
{
}

void ControlManager::listenControl(QString&)
{
}

void ControlManager::autosetControl(QString&)
{
}
