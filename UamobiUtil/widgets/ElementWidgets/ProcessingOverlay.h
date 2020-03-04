#pragma once
#ifdef QT_VERSION5X
#include <QtWidgets/qwidget.h>
#else
#include <QtGui/QWidget>
#endif
#include <QtCore/QTimer>

/*
	This file contains smart hovering widget that is trying to pin to the screen center. It will display
	timeout counter with provided interval and will automatically hide itself on timeout.
	Widget elements:
		Hourglass made from 4 triangles
		Countdown in left corner in seconds
		Explaining text

	This widget contains possibility to open it from any place in your code in case you placed it as child of the root widget.
	Use bindOverlay to bind particular object, use hideProcessingOverlay and showProcessingOverlay function for showing
	and hiding bound widget.
*/

class ProcessingOverlay : public QWidget
{
	Q_OBJECT
protected:
	float currentState;	//	current state of countdown
	float endPoint;		//	end point of countdown
	int innerTopSize;	//	size of inner polygon which is representing "remaining" time
	int innerLowerSize;
	int totalRibSize;
	QTimer* redrawTimer;	//	timer which is calling repaint
	QPolygon TopTriangle;	//	outer polygon representing all time
	QPoint lowerPointLeft;
	QPoint lowerPointRight;
	QPolygon LowerInnerTriangle;
	QPolygon lowerTriangle;	//	inner polygon representing remaining time
	QPoint centralPoint;	//	center of widget, used as starting point for drawing polygons
	QPoint textPoint;		//	point where text is drawn
	QLinearGradient gradient;
	void paintEvent(QPaintEvent*);
	void resizeEvent(QResizeEvent*);
public:
	ProcessingOverlay(int interval, QWidget* parent = Q_NULLPTR);
	void restart();	//	equal to show
	virtual void hide();	//	stops timer, hides this
	virtual void show();	//	restarts timer and shows widget
	void resize(const QSize&);	//	normal resize
	void setTemporaryDelay(int additionalDelay);
private slots:
	void step();				//	is triggered by redrawTimer
};
extern void showProcessingOverlay();	//	shows bound overlay
extern void bindProcessingOverlay(ProcessingOverlay*);	//	binds overlay to static pointer. Can be only one.
extern void hideProcessingOverlay();	//	hides boud overlay
