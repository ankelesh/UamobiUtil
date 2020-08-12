#include "ExtendedLabels.h"
#include "widgets/utils/ElementsStyles.h"
#include <QPaintEvent>
#include <QStyleOption>
#include <qstylepainter.h>
#include <QMouseEvent>
#include <cmath>

void TwoLevelCounterLabel::paintEvent(QPaintEvent* pev)
{
	QStylePainter qsp(this);
	QStyleOption qso;
	
	qso.initFrom(this);
	qsp.drawPrimitive(QStyle::PE_Frame, qso);
	QRect textBox = qso.rect;
	qsp.save();
	qsp.setPen(QPen(Qt::black, 4));
	qsp.setBrush(Qt::NoBrush);
	qsp.drawRect(qso.rect);
	qsp.restore();
	textBox.setBottomRight(QPoint(textBox.bottomRight().x(), textBox.bottomRight().y() - (textBox.height() * 0.6666)));
	
	qsp.setFont(textFont);
	qsp.drawText(textBox, Qt::AlignCenter, text);
	
	textBox.setTopLeft(textBox.bottomLeft());
	textBox.setBottomRight(qso.rect.bottomRight());
	qsp.setFont(counterFont);
    if (qIsNaN(value))
	{
		qsp.drawText(textBox, Qt::AlignCenter, "?");
	}
	else
	{
		qsp.drawText(textBox, Qt::AlignCenter, textValue);
	}

}

TwoLevelCounterLabel::TwoLevelCounterLabel(QString explanation, double val, QWidget* parent)
	: labels_private::abs_counter_label(GENERAL_FONT, AppFonts->makeFont(3.0), parent), text(explanation),
	value(val),textValue(QString::number(val))
{
}

void TwoLevelCounterLabel::_setText(const QString& txt)
{
	text = txt;
}

void TwoLevelCounterLabel::_setValue(const double& val)
{
	value = val;
	textValue = QString::number(val);
}

void TwoLevelCounterLabel::_clearValue()
{
	value = qQNaN();
	textValue.clear();
}

double TwoLevelCounterLabel::_getValue() const
{
	return value;
}

void ClickableTLCounterLabel::mouseReleaseEvent(QMouseEvent* qme)
{
	emit clicked(qme->button() == Qt::MouseButton::LeftButton);
	TwoLevelCounterLabel::mouseReleaseEvent(qme);
}

void ClickableTLCounterLabel::paintEvent(QPaintEvent* pev)
{
	TwoLevelCounterLabel::paintEvent(pev);
	QStylePainter sp(this);
	QRect innerRect = pev->rect();
	innerRect.setTopLeft(innerRect.topLeft() + QPoint(4, 4));
	innerRect.setBottomRight(innerRect.bottomRight() - QPoint(4, 4));
	sp.setBrush(Qt::NoBrush);
	sp.setPen(QPen(Qt::lightGray, 4));
	sp.drawRect(innerRect);
}

ClickableTLCounterLabel::ClickableTLCounterLabel(QString explanation, double value, QWidget* parent)
	: TwoLevelCounterLabel(explanation, value, parent)
{
}

void CounterLabel::paintEvent(QPaintEvent* pev)
{
	QStylePainter spainter(this);
	QStyleOptionFrame opt;
	spainter.drawPrimitive(QStyle::PE_Frame, opt);
	QRect counterRect = pev->rect();
	counterRect.setBottomRight(
		QPoint(
			counterRect.bottomRight().x() - counterRect.width() / 3,
			counterRect.bottomRight().y()
		)
	);

	spainter.drawRect(counterRect);
	spainter.drawItemText(counterRect, Qt::TextWordWrap | Qt::AlignCenter, opt.palette, true, text());
	counterRect.setTopLeft(counterRect.topRight());
	counterRect.setBottomRight(pev->rect().bottomRight());
	spainter.setBrush(Qt::white);
	spainter.setPen(QPen(Qt::black, 3));
	spainter.drawRect(counterRect);
	spainter.setFont(counterFont);
	spainter.drawItemText(counterRect, Qt::TextWordWrap | Qt::AlignCenter, opt.palette, true, counter);
	spainter.end();

}

CounterLabel::CounterLabel(QWidget* parent)
	: labels_private::abs_counter_label(parent), counter()
{


}


void CounterLabel::_setText(const QString& txt)
{
	QLabel::setText(txt);
}

void CounterLabel::_setValue(const double& val)
{
	counter = QString::number(val);
}

void CounterLabel::_clearValue()
{
	counter.clear();
}

double CounterLabel::_getValue() const
{
	return counter.toDouble();
}

void SemaphorLabel::_recolor()
{
	if (currentState >= 0 && currentState <= opfail)
	{
		QPalette p(this->palette());
		p.setColor(QPalette::Window, statedColors[currentState]);
		setPalette(p);
		update();
	}
}
QColor SemaphorLabel::statedColors[opfail + 1] = {
	QColor(166, 201, 227), QColor(228, 235, 181),QColor(168, 219, 145),QColor(230, 105, 80)
};
SemaphorLabel::SemaphorLabel(int defaultState, QWidget* parent)
	: QLabel(parent), currentState(((defaultState >= 0 && defaultState <= opfail) ? defaultState : passive))
{
	_recolor(); setAutoFillBackground(true); setAlignment(Qt::AlignCenter);
}

SemaphorLabel::SemaphorLabel(QWidget* parent)
	: QLabel(parent), currentState(passive)
{
	_recolor(); setAutoFillBackground(true); setAlignment(Qt::AlignCenter);
}

void SemaphorLabel::setState(int newState)
{
	currentState = newState;
	_recolor();
}

labels_private::abs_counter_label::abs_counter_label(QWidget* parent)
	:QLabel(parent), textFont(GENERAL_FONT), counterFont(AppFonts->makeFont(3.0))
{
}

double labels_private::abs_counter_label::getValue() const
{
	return _getValue();
}

void labels_private::abs_counter_label::setFonts(double txtpercent, double counterpercent)
{
	textFont = AppFonts->makeCustomFont( txtpercent);
	counterFont = AppFonts->makeCustomFont(counterpercent);
	update();
}

void labels_private::abs_counter_label::setText(const QString& s)
{
	_setText(s);
	update();
}

void labels_private::abs_counter_label::setValue(const double& val)
{
	_setValue(val);
	update();
}

void labels_private::abs_counter_label::clearValue()
{
	_clearValue();
	update();
}
