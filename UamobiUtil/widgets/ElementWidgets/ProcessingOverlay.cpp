#include "ProcessingOverlay.h"
#include <QtGui/QPainter>
#include <cmath>
#include "widgets/utils/ElementsStyles.h"
#include <QtGui/qevent.h>
#ifdef DEBUG
#include "debugtrace.h"
#endif

const double PI = 3.141592653589793238463;
const double RadMult = PI / 180;


QPoint find_point_on(const QPoint center, const int distance, const double grades)
{
	double arad = RadMult * grades;
    return QPoint(
               int(center.x() + distance * cos(arad)),
                int(center.y() + distance * sin(arad)));
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
    if (totalDistance == 0)
        return tret;
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
void ProcessingOverlay::paintEvent(QPaintEvent* /*pev*/)
{
	QRect bg(0, 0, width() - 1, height() - 1);
	QPainter p(this);
	gradient.setStart(0, 0);
	gradient.setFinalStop(qreal(width()) - 1, 0);
	p.setBrush(QBrush(Qt::white));
	p.drawRect(bg);
    p.drawText(10, 10, QString::number(int(currentState / 2.0f)));
	p.setRenderHint(QPainter::Antialiasing);
	p.setRenderHint(QPainter::TextAntialiasing);
	p.drawPolygon(TopTriangle);
	p.drawPolygon(lowerTriangle);
	p.drawText(textPoint, tr("Awaiting network response"));
	p.setBrush(gradient);
    p.setOpacity(1.0);
	p.drawPolygon(LowerInnerTriangle);
}

void ProcessingOverlay::resizeEvent(QResizeEvent* rev)
{
	QWidget::resizeEvent(rev);
}

ProcessingOverlay::ProcessingOverlay(int interval, QWidget* parent)
	: QWidget(parent), currentState(1), endPoint(10), innerTopSize(0), innerLowerSize(0),
	totalRibSize(0), redrawTimer(new QTimer(this)), TopTriangle(),
	lowerPointLeft(), lowerPointRight(), LowerInnerTriangle(), centralPoint(), textPoint(),
	gradient()
{
#ifdef Q_OS_WINCE
    this->setFixedSize(calculateAdaptiveSize(0.5));
    QSize partial = calculateAdaptiveSize(0.4);
#else
#ifdef Q_OS_WIN
	QSize parentG = imitatePhoneSize(0.66);
#else
	QSize parentG = calculateAdaptiveButtonHeight(1.0);
#endif
    this->setFixedSize(QSize(int(parentG.width() * 0.4), int(parentG.height() * 0.4)));
    QSize partial = QSize(int(parentG.width() * 0.3), int(parentG.height() * 0.3));
#endif
	this->move(partial.width(), partial.height());
	centralPoint = QPoint(this->width() / 2, this->height() / 2);
#ifdef Q_OS_WINCE
	textPoint = QPoint(0 + 2, height() - 2);
#else
	textPoint = QPoint(width() / 5, this->height() - 5);
#endif
	endPoint = interval / 500;
	currentState = endPoint;
    TopTriangle = makeTriangle(centralPoint, int(this->height() * 0.4), true);
    lowerTriangle = makeTriangle(centralPoint, int(this->height() * 0.4), false);
	lowerPointLeft = lowerTriangle.at(1);
	lowerPointRight = lowerTriangle.at(2);
    LowerInnerTriangle = makeParrallellogram(centralPoint, lowerPointRight, lowerPointLeft, int(this->height() *
		0.8f * (currentState / (endPoint))), totalRibSize);
    innerTopSize = int(this->height() * 0.4f * (currentState / endPoint));
    innerLowerSize = int(this->height() * 0.4f * (currentState / endPoint));
	totalRibSize = innerTopSize; 
    redrawTimer->setInterval(int(interval / endPoint));
	gradient.setColorAt(0, Qt::darkRed);
	gradient.setColorAt(0.25, Qt::red);
	gradient.setColorAt(1, Qt::white);
#ifdef QT_VERSION5X
	QObject::connect(redrawTimer, &QTimer::timeout, this, &ProcessingOverlay::step,Qt::DirectConnection);
#else
	QObject::connect(redrawTimer, SIGNAL(timeout()), this, SLOT(step()), Qt::DirectConnection);
#endif
}

void ProcessingOverlay::restart()
{
	currentState = endPoint;
    innerTopSize = int(this->height() * 0.4f * (currentState / endPoint));
    innerLowerSize = int(this->height() * 0.4f * (currentState / endPoint));
	totalRibSize = innerTopSize;
    LowerInnerTriangle = makeParrallellogram(centralPoint, lowerPointRight, lowerPointLeft, int(this->height() *
                                             0.8f * (currentState / (endPoint))), totalRibSize);
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
    this->setFixedSize(QSize(int(parentG.width() * 0.4), int(parentG.height() * 0.4)));
    QSize partial = QSize(int(parentG.width() * 0.3), int(parentG.height() * 0.3));
	this->move(partial.width(), partial.height());
	centralPoint = QPoint(this->width() / 2, this->height() / 2);
#ifdef Q_OS_WINCE
	textPoint = QPoint(0 + 2, height() - 2);
#else
	textPoint = QPoint(5, this->height() - 5);
#endif
    TopTriangle = makeTriangle(centralPoint, int(this->height() * 0.4), true);
    lowerTriangle = makeTriangle(centralPoint, int(this->height() * 0.4), false);
	lowerPointLeft = lowerTriangle.at(1);
	lowerPointRight = lowerTriangle.at(2);
    LowerInnerTriangle = makeParrallellogram(centralPoint, lowerPointRight,
         lowerPointLeft, int(this->height() * 1.38f * (currentState / (endPoint))), totalRibSize);

    innerTopSize = int(this->height() * 0.4f * (currentState / endPoint));
    innerLowerSize = int(this->height() * 0.4f * (currentState / endPoint));
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
    innerLowerSize = int(this->height() * 0.8f * (currentState / (endPoint)));
	LowerInnerTriangle = makeParrallellogram(centralPoint, lowerPointRight, lowerPointLeft, innerLowerSize, totalRibSize);
	update();
    if (currentState < 0.001f)
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
