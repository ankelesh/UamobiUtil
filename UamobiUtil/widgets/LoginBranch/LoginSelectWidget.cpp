#include "LoginSelectWidget.h"


LoginSelectWidget::LoginSelectWidget(QVector<UserProfile>& Profiles, QWidget* parent)
	:AbstractVariantSelectionWidget(parent),profiles(Profiles)
{
	init();
}

QString LoginSelectWidget::elemAsString(int index)
{
	return profiles.at(index).name;
}

int LoginSelectWidget::countElems()
{
	return profiles.count();
}

void LoginSelectWidget::indexSelected(int Index)
{
	emit profilePicked(profiles.at(Index));
}
