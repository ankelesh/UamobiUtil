#include "ListPickObserver.h"

ListPickObserver::ListPickObserver(RecEntity proto, QWidget* parent) :
	IdDependentSelectWidget(proto, parent)
{

}

void ListPickObserver::itemSelected(RecEntity e)
{
	emit done(e);
}