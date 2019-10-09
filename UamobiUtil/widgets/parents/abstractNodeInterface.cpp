#include "abstractNodeInterface.h"

bool abstractNode::_hideCurrent(inframedWidget* replacement)
{
	if (current != untouchable)
	{
		current->hide();
		current = replacement;
		replacement->show();
		return true;
	}
	return false;
}

void abstractNode::_hideAny(inframedWidget* replacement)
{
	current->hide();
	replacement->show();
	current = replacement;
}

void abstractDynamicNode::hideAndDeleteCurrent(QPointer<inframedWidget>* replacement)
{
	if (current != untouchable)
	{
		delete (*current);
		current = replacement;
		(*current)->show();
	}
}

void abstractDynamicNode::hideAndDeleteAny(QPointer<inframedWidget>* replacement)
{
	delete (*current);
	current = replacement;
	(*current)->show();
}