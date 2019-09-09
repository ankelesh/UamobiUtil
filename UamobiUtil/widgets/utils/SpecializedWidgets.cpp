#include "SpecializedWidgets.h"
#include <exception>

specwidgets::indexedButton::indexedButton(int Index, QWidget* parent) : QPushButton(parent), index(Index)
{
#ifdef QT_VERSION5X
	if (!QObject::connect(this, &QPushButton::clicked, this, &indexedButton::clickCapt)) throw  std::exception("no connection in indexed button");
#else
    if (!QObject::connect(this, SIGNAL(clicked()), this, SLOT(clickCapt()))) throw  std::exception("no connection in indexed button");
#endif
}
void specwidgets::indexedButton::setIndex(int Index)
{
	index = Index;
}
void specwidgets::indexedButton::clickCapt()
{
	emit indexClicked(index);
}
;
