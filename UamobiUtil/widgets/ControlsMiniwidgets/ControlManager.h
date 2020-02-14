#pragma once
#include <QObject>
#include <QHash>
#include "QuantityControl.h"
#include "legacy/legacy.h"

/*
	This file is providing fabricators for controls. It tries to guess control type by legacy names, then constructs and returns control.
*/
namespace ControlManager
{
	enum GuessedControlType {None, Quantity};
}


abs_control* fabricateControl(QString initstr, QBoxLayout* layout, QWidget* parent);