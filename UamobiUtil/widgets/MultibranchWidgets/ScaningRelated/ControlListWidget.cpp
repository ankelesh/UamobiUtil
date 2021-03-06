#include "ControlListWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include <QTextStream>
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif

void ControlListWidget::focusInEvent(QFocusEvent* fev)
{
	inframedWidget::focusInEvent(fev);
	if (!controls.isEmpty())
        for (QVector<abs_control*>::iterator contr = controls.begin(); contr != controls.end(); ++contr)
        {
            switch((*contr)->myType())
            {
            case abs_control::Label:
            case abs_control::None:
                break;
            default:
                (*contr)->setFocus();
                return;
            }
        }
}

ControlListWidget::ControlListWidget(QWidget* parent)
	: inframedWidget(parent), mainLayout(new QVBoxLayout(this)),
	controls(), footerLayout(new QHBoxLayout(this)),
    okButton(new MegaIconButton(this)), backButton(new MegaIconButton(this)),
    sealed(false)
{
#ifdef DEBUG
	detrace_DCONSTR("ControlListWidget");
#endif

	this->setLayout(mainLayout);
	mainLayout->addStretch(1);
	mainLayout->addLayout(footerLayout);
	
	footerLayout->addWidget(backButton);
	footerLayout->addWidget(okButton);
	
	okButton->setIcon(QIcon(":/resources/submit"));
	okButton->setText(tr("submit"));
	okButton->setStyleSheet(OK_BUTTONS_STYLESHEET);

	backButton->setIcon(QIcon(":/resources/back"));
	backButton->setText(tr("back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

#ifdef QT_VERSION5X
	QObject::connect(okButton, &MegaIconButton::clicked, this, &ControlListWidget::checkAndConfirmControls);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &ControlListWidget::backRequired);
#else
	QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(checkAndConfirmControls()));
    QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
#endif
}

void ControlListWidget::clearControls()
{
	QVector<abs_control*>::iterator begin = controls.begin();
	while (begin != controls.end())
	{
		(*(begin++))->deleteLater();
	}
	controls.clear();
    sealed = false;
}

void ControlListWidget::useControls(const IControlList& clist)
{
	clearControls();
    IControlList::const_iterator begin = clist.begin();
	abs_control* newcontrol(Q_NULLPTR);
	while (begin != clist.end())
	{
		newcontrol = fabricateControl(*begin++, mainLayout, this);
		if (newcontrol == Q_NULLPTR)
			continue;
		if (!controls.isEmpty())
		{
			newcontrol->connectAsNext(controls.last());
		}
		controls.push_back(newcontrol);
		newcontrol->show();
	}
	if (newcontrol != Q_NULLPTR)
    {
        sealControls();
	}
}


QStringList ControlListWidget::getControlsValues()
{
	QStringList temp;
	QVector<abs_control*>::iterator begin = controls.begin();
	while (begin != controls.end())
	{
		temp << (*(begin++))->getValue();
	}
	return temp;
}

QString ControlListWidget::getValueAt(int index)
{
	if (index >= 0 && index < controls.count())
		return controls.at(index)->getValue();
	return QString();
}

void ControlListWidget::emplaceControl(InputControl c)
{
    if (sealed) return;
	abs_control* newcontrol = fabricateControl(c, mainLayout, this);
	if (newcontrol == Q_NULLPTR)
		return;
	if (!controls.isEmpty())
	{
		newcontrol->connectAsNext(controls.last());
	}
	newcontrol->setValue(c->defaultValue);
	controls.push_back(newcontrol);
    newcontrol->show();
}

void ControlListWidget::sealControls()
{
    if (sealed)
        return;
    if (controls.isEmpty())
        return;
#ifdef QT_VERSION5X
        QObject::connect(controls.last(), &abs_control::editingFinished, this, &ControlListWidget::checkAndConfirmControls);
#else
        QObject::connect(controls.last(), SIGNAL(editingFinished()), this, SLOT(checkAndConfirmControls()));
#endif
        sealed = true;
}


QString ControlListWidget::joinedControls()
{
	QString buffer;
	buffer.reserve(controls.count() * 5);
	QTextStream out(&buffer);
	QVector<abs_control*>::iterator begin = controls.begin();
	while (begin != controls.end())
	{
		if ((*begin)->myType() == abs_control::Label)
		{
			++begin;
			continue;
		}
		out << (*begin++)->getValue() << ",";
	}
	out.flush();
	buffer.chop(1);
	return buffer;
}

QString ControlListWidget::mappedControls()
{
	QString buffer;
	buffer.reserve(controls.count() * 10);
	QTextStream out(&buffer);
	QVector<abs_control*>::iterator begin = controls.begin();
	while (begin != controls.end())
	{
		if ((*begin)->myType() == abs_control::Label)
		{
			++begin;
			continue;
		}
		out << "&" << (*begin)->name  << "=" << (*begin)->getValue();
		++begin;
	}
	out.flush();
	return buffer;
}



ControlListWidget::~ControlListWidget()
{
	clearControls();
}

void ControlListWidget::checkAndConfirmControls()
{
	emit controlsConfirmed();
}
