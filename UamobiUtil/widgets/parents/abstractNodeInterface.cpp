#include "abstractNodeInterface.h"

bool abstractNode::_hideCurrent(inframedWidget* replacement)
{
	if (current != untouchable)
	{
		current->hide();
		current = replacement;
		if (replacement == untouchable)
		{
			main->setFocus();
		}
		else
		{
			replacement->setFocus();
		}
		replacement->show();
		return true;
	}
	return false;
}

void abstractNode::_hideAny(inframedWidget* replacement)
{
	current->hide();
	current = replacement;
	if (replacement == untouchable)
	{
		main->setFocus();
	}
	else
	{
		replacement->setFocus();
	}
	replacement->show();
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