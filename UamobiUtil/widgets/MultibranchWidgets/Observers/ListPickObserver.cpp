#include "ListPickObserver.h"
#ifdef DEBUG
#include "debugtrace.h"
#endif

void ListPickObserver::parse_get_response()
{
	IdDependentSelectWidget::parse_get_response();
	if (entityModel->rowCount() == 0)
		emit done(dependency);
}

void ListPickObserver::_handleRecord(RecEntity e)
{
	entityModel->reset();
	IdDependentSelectWidget::_handleRecord(e);
}

ListPickObserver::ListPickObserver(RecEntity proto, QWidget* parent) :
	IdDependentSelectWidget(proto, parent)
{
#ifdef DEBUG
	detrace_DCONSTR("ListPickObserver");
#endif
}

void ListPickObserver::itemSelected(RecEntity e)
{
	emit done(e);
}