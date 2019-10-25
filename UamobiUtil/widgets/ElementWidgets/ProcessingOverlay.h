#pragma once
#ifdef QT_VERSION5X
#include <QtWidgets/qwidget.h>
#else
#include <QtGui/QWidget>
#endif
#include <QtCore/QTimer>


class ProcessingOverlay : public QWidget
{
	Q_OBJECT
protected:
	float currentState;
	float endPoint;
	int innerPolySize;
	QTimer* redrawTimer;
	QPolygon mainPolygon;
	QPolygon innerPolygon;
	QPoint centralPoint;
	QPoint textPoint;
    QSize oldSize;
	void paintEvent(QPaintEvent*);
	void resizeEvent(QResizeEvent*);
public:
	ProcessingOverlay(int interval, QWidget* parent = Q_NULLPTR);
	void restart();
	virtual void hide();
	virtual void show();
	void resize(const QSize&);
private slots:
	void step();
};
extern void showProcessingOverlay();
extern void bindProcessingOverlay(ProcessingOverlay*);
extern void hideProcessingOverlay();
