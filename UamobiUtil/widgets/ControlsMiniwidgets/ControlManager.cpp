#include "ControlManager.h"
#include "submodules/UNAQtCommons/widgets/ControlsWidgets/QuantityControl.h"
#include "submodules/UNAQtCommons/widgets/ControlsWidgets/DateTimeControl.h"
#include "submodules/UNAQtCommons/widgets/ControlsWidgets/LabelControl.h"
#include "submodules/UNAQtCommons/widgets/ControlsWidgets/BarcodeControl.h"
#include "submodules/UNAQtCommons/widgets/ControlsWidgets/StringControl.h"
#include <qlayout.h>


abs_control* fabricateControl(InputControl control, QBoxLayout* layout, QWidget* parent)
{
	switch (int(control->getAttachedNumber()))
	{
	case abs_control::Float:
	{
		QuantityControl* qc = new QuantityControl(false, control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, qc->myWidget());
		qc->setValue(control->defaultValue);
		return qc;
	}
	default:
	case abs_control::Int:
	{
		QuantityControl* qc = new QuantityControl(true, control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, qc->myWidget());
		qc->setValue(control->defaultValue);
		return qc;
	}
	case abs_control::Date:
	{
		DateTimeControl* dc = new DateTimeControl(false, control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, dc->myWidget());
		dc->setValue(control->defaultValue);
		return dc;
	}
	case abs_control::Label:
	{
		LabelControl* lc = new LabelControl(control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, lc->myWidget());
		lc->setValue(control->defaultValue);
		return lc;
	}
	case abs_control::Barcode:
	{
		BarcodeControl* bc = new BarcodeControl(control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, bc->myWidget());
		bc->setValue(control->defaultValue);
		return bc;
	}
	case abs_control::String:
	{
		StringControl* sc = new StringControl(control->getTitle(), parent);
		layout->insertWidget(layout->count() - 1, sc->myWidget());
		sc->setValue(control->defaultValue);
		return sc;
	}
	case abs_control::None:
		return Q_NULLPTR;
	}
}
