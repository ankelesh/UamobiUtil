#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QWidget>
#include <QtWidgets/QBoxLayout>
#else
// Qt4 only widgets
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#endif
// widgets imports
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/SpecializedWidgets.h"
#include "widgets/parents/AbstractVariantSelectionWidget.h"
// networking imports
#include "networking/things.h"

/*
	This widget is a specialized avsw widget. It was basically first it's prototype, so it remains in global namespace
	For other info check AVSW widget
*/


