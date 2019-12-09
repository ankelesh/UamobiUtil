#include "ProcessingOverlay.h"
#include <QtGui/QPainter>
#include <cmath>
#include "widgets/utils/ElementsStyles.h"
#include <QtGui/qevent.h>
#ifdef DEBUG
#include "debugtrace.h"
#endif

const float PI = 3.141592653589793238463;
const float RadMult = PI / 180;


QPoint find_point_on(const QPoint center, const int distance, const double grades)
{
	double arad = RadMult * grades;
	return QPoint(center.x() + distance * cos(arad), center.y() + distance * sin(arad));
}

QPolygon makeTriangle(const QPoint& center, const int& size, bool isUp)
// makes triangle
{
	QVector<QPoint> points;
	points.reserve(3);
	points.push_back(center);
	points.push_back(find_point_on(center, size, ((isUp) ? 215 : 145)));
	points.push_back(find_point_on(center, size, ((isUp) ? 325 : 35)));
	return QPolygon(points);
}
QPoint pointOnLine(const QPoint f, const QPoint s, const int distance, const int totalDistance)
{
	QPoint tret;
	tret.rx() = f.x() - ((distance * (f.x() - s.x()) / totalDistance));
	tret.ry() = f.y() - ((distance * (f.y() - s.y()) / totalDistance));
	return tret;
}
QPolygon makeParrallellogram(const QPoint center, const QPoint lowerR, const QPoint lowerL, const int dist, const int sz)
{
	QVector<QPoint> points;
	points.reserve(4);
	points.push_back(lowerR);
	points.push_back(lowerL);
	points.push_back(pointOnLine(lowerL, center, dist, sz));
	points.push_back(pointOnLine(lowerR, center, dist, sz));
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
	p.drawPolygon(TopTriangle);
	p.drawPolygon(lowerTriangle);
	p.drawText(textPoint, tr("Awaiting network response"));
	p.setBrush(QBrush(Qt::red));
	p.setOpacity(0.6);
	p.drawPolygon(LowerInnerTriangle);
}

void ProcessingOverlay::resizeEvent(QResizeEvent* rev)
{
	QWidget::resizeEvent(rev);
}

ProcessingOverlay::ProcessingOverlay(int interval, QWidget* parent)
	: QWidget(parent), currentState(1), endPoint(10), redrawTimer(new QTimer(this))
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
	TopTriangle = makeTriangle(centralPoint, this->height() * 0.7, true);
	lowerTriangle = makeTriangle(centralPoint, this->height() * 0.7, false);
	lowerPointLeft = lowerTriangle.at(1);
	lowerPointRight = lowerTriangle.at(2);
	LowerInnerTriangle = makeTriangle(centralPoint, this->height() * 0.7, false);
	innerTopSize = this->height() * (0.7 * (currentState / endPoint));
	innerLowerSize = this->height() * (0.7 * (currentState / endPoint));
	totalRibSize = innerTopSize; 
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
	innerTopSize = this->height() * (0.7 * (currentState / endPoint));
	innerLowerSize = this->height() * (0.7 * (currentState / endPoint));
	LowerInnerTriangle = makeParrallellogram(centralPoint, lowerPointRight, lowerPointLeft, this->height() * (1.38 * (currentState / (endPoint))), totalRibSize);
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
	TopTriangle = makeTriangle(centralPoint, this->height() * 0.7, true);
	lowerTriangle = makeTriangle(centralPoint, this->height() * 0.7, false);
	lowerPointLeft = lowerTriangle.at(1);
	lowerPointRight = lowerTriangle.at(2);
	LowerInnerTriangle = makeParrallellogram(centralPoint, lowerPointRight, lowerPointLeft, this->height() * (1.38 * (currentState / (endPoint))), totalRibSize);

	innerTopSize = this->height() * (0.7 * (currentState / endPoint));
	innerLowerSize = this->height() * (0.7 * (currentState / endPoint));
	totalRibSize = innerTopSize;
}

void ProcessingOverlay::setTemporaryDelay(int additionalDelay)
{
	currentState = endPoint + additionalDelay / 500;
	redrawTimer->start();
	QWidget::show();
}

void ProcessingOverlay::step()
{
	--currentState;
	innerLowerSize = this->height() * (1.38 * (currentState / (endPoint)));
	LowerInnerTriangle = makeParrallellogram(centralPoint, lowerPointRight, lowerPointLeft, innerLowerSize, totalRibSize);
	update();
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
void showProcessingOverlay(int additionalDelay)
{
	if (ov != Q_NULLPTR)
	{
		ov->setTemporaryDelay(additionalDelay);
		ov->raise();
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