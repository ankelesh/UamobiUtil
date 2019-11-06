#include "abstractNodeInterface.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif

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
	replacement->setFocus();

	if (replacement == untouchable)
	{
		main->setFocus();
	}
	replacement->show();

#ifdef DEBUG
	detrace_METHEXPL("replacement " << (long long int) replacement << " untch: " << (long long int) untouchable);
#endif
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