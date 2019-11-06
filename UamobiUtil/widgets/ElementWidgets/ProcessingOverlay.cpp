#include "ProcessingOverlay.h"
#include <QtGui/QPainter>
#include <cmath>
#include "widgets/utils/ElementsStyles.h"
#include <QtGui/qevent.h>
#ifdef DEBUG
#include "debugtrace.h"
#endif

const double pi = 3.14159265358979323846;		//	not precise pi. More precision is not required

QPoint	find_corner(const QPoint& center, const int& size,
	const int& corner_num)
	//	finds corner of pointy hexagon
{
	float adeg = 60 * corner_num - 30;
	float arad = pi / 180 * adeg;
	return QPoint(center.x() + size * cos(arad), center.y() + size * sin(arad));
}
QPolygon makeHexagon(const QPoint& center, const int& size)
// makes polygon ready to be drawn
{
	QVector<QPoint> points;
	points.reserve(6);
	for (int i = 0; i < 6; ++i)
	{
		points.push_back(find_corner(center, size, i));
	}
	return QPolygon(points);
}

void ProcessingOverlay::paintEvent(QPaintEvent* pev)
{
	QRect bg(0, 0, width() - 1, height() - 1);
	QPainter p(this);
	p.setBrush(QBrush(Qt::white));
	p.drawRect(bg);
	p.drawText(10, 10, QString::number((int)currentState / 2));
	p.setRenderHint(QPainter::Antialiasing);
	p.setRenderHint(QPainter::TextAntialiasing);
	p.drawPolygon(mainPolygon);
	p.drawText(textPoint, tr("Awaiting network response"));
	p.setBrush(QBrush(Qt::red));
	p.setOpacity(0.6);
	p.drawPolygon(innerPolygon);
}

void ProcessingOverlay::resizeEvent(QResizeEvent* rev)
{
	QWidget::resizeEvent(rev);
}

ProcessingOverlay::ProcessingOverlay(int interval, QWidget* parent)
	: QWidget(parent), currentState(1), endPoint(10), redrawTimer(new QTimer(this)), mainPolygon(), innerPolygon(),
	centralPoint()
{
#ifdef Q_OS_WINCE
	this->setFixedSize(calculateAdaptiveSize(0.4));
	QSize partial = calculateAdaptiveSize(0.3);
#else
	QSize parentG = qApp->desktop()->size();

	this->setFixedSize(QSize(parentG.width() * 0.4, parentG.height() * 0.4));
	QSize partial = QSize(parentG.width() * 0.3, parentG.height() * 0.3);
#endif
	this->move(partial.width(), partial.height());
	centralPoint = QPoint(this->width() / 2, this->height() / 2);
#ifdef Q_OS_WINCE
	textPoint = QPoint(0 + 2, height() - 2);
#else
	textPoint = QPoint(width() / 5, this->height() - 5);
#endif
	endPoint = interval / 500;
	mainPolygon = makeHexagon(centralPoint, this->height() * 0.45);
	innerPolySize = this->height() * (0.45 * (currentState / endPoint));
	redrawTimer->setInterval(interval / endPoint);
#ifdef QT_VERSION5X
	QObject::connect(redrawTimer, &QTimer::timeout, this, &ProcessingOverlay::step);
#else
	QObject::connect(redrawTimer, SIGNAL(timeout()), this, SLOT(step()));
#endif
}

void ProcessingOverlay::restart()
{
	currentState = endPoint;
	innerPolySize = this->height() * (0.45 * (currentState / endPoint));
	innerPolygon = makeHexagon(centralPoint, innerPolySize);
	redrawTimer->start();
}

void ProcessingOverlay::hide()
{
	redrawTimer->stop();
	currentState = endPoint;
	QWidget::hide();
}

void ProcessingOverlay::show()
{
	restart();
	if (oldSize != parentWidget()->size()) {
		resize(parentWidget()->size());
	}
	QWidget::show();
}

void ProcessingOverlay::resize(const QSize& parentG)
{
	this->setFixedSize(QSize(parentG.width() * 0.4, parentG.height() * 0.4));
	QSize partial = QSize(parentG.width() * 0.3, parentG.height() * 0.3);
	this->move(partial.width(), partial.height());
	centralPoint = QPoint(this->width() / 2, this->height() / 2);
#ifdef Q_OS_WINCE
	textPoint = QPoint(0 + 2, height() - 2);
#else
	textPoint = QPoint(width() / 5, this->height() - 5);
#endif
	mainPolygon = makeHexagon(centralPoint, this->height() * 0.45);
	innerPolySize = this->height() * (0.45 * (currentState / endPoint));
	innerPolygon = makeHexagon(centralPoint, innerPolySize);
	oldSize = parentG;
}

void ProcessingOverlay::step()
{
	--currentState;
	innerPolySize = this->height() * (0.45 * (currentState / endPoint));
	innerPolygon = makeHexagon(centralPoint, innerPolySize);
	repaint();
	if (currentState == 0)
	{
		redrawTimer->stop();
		hide();
	}
}
// this pointer is used for outer calls to main overlay
static ProcessingOverlay* ov;

void showProcessingOverlay()
{
	if (ov != Q_NULLPTR) {
		if (ov->isHidden()) {
			ov->show();
			ov->raise();
		}
	}
}
void bindProcessingOverlay(ProcessingOverlay* over)
{
	ov = over;
}
void hideProcessingOverlay()
{
	if (ov != Q_NULLPTR)
	{
		if (!ov->isHidden())
			ov->hide();
	}
}