#pragma once
#include <QLabel>
#include <cmath>

namespace labels_private {
	class abs_counter_label : public QLabel
	{
		Q_OBJECT
	protected:
		QFont textFont;
		QFont counterFont;
		virtual void _setText(const QString&) = 0;
		virtual void _setValue(const double& val) = 0;
		virtual void _clearValue() = 0;
		virtual double _getValue() const = 0;
	public:
		abs_counter_label(QWidget* parent = Q_NULLPTR);
		abs_counter_label(QFont tF, QFont cF, QWidget* parent = Q_NULLPTR) 
			: QLabel(parent), textFont(tF), counterFont(cF) {}
		double getValue() const;
		void setFonts(double txtpercent, double counterpercent);
	public slots:
		void setText(const QString&);
		void setValue(const double& val);
		void clearValue();
	};

}
class TwoLevelCounterLabel : public labels_private::abs_counter_label
{
	Q_OBJECT
protected:
	QString text;
	double value;
	QString textValue;
	virtual void paintEvent(QPaintEvent*)override;
	// Inherited via abs_counter_label
	virtual void _setText(const QString&) override;
	virtual void _setValue(const double& val) override;
	virtual void _clearValue() override;
	virtual double _getValue() const override;
public:
    TwoLevelCounterLabel(QString explanation = QString(), double value = std::nan(""), QWidget* parent = Q_NULLPTR);




};
class ClickableTLCounterLabel : public TwoLevelCounterLabel
{
	Q_OBJECT
protected:
	virtual void mouseReleaseEvent(QMouseEvent*) override;
	virtual void paintEvent(QPaintEvent*)override;
public:
	ClickableTLCounterLabel(QString explanation = QString(), double value = 0.0, QWidget* parent = Q_NULLPTR);
signals:
	void clicked(bool left = true);
};
class ClickableLabel : public QLabel
{
	Q_OBJECT
protected:
	virtual void mouseReleaseEvent(QMouseEvent* mev) override { emit clicked(); QLabel::mouseReleaseEvent(mev); }
public:
	ClickableLabel(QWidget* parent = Q_NULLPTR) :QLabel(parent) {}
signals:
	void clicked();
};

class CounterLabel : public labels_private::abs_counter_label
{
	Q_OBJECT
protected:
	QString counter;
	virtual void paintEvent(QPaintEvent*) override;
	// Inherited via abs_counter_label
	virtual void _setText(const QString&) override;
	virtual void _setValue(const double& val) override;
	virtual void _clearValue() override;
	virtual double _getValue() const override;
public:
	CounterLabel(QWidget* parent);
};


class SemaphorLabel : public QLabel
{
	Q_OBJECT
public:
	enum SemaStates {
		passive,
		awaiting,
		opsuccess,
		opfail
	};
protected:
	int currentState;

	static QColor statedColors[opfail + 1];
	void _recolor();
public:
	SemaphorLabel(int defaultState = passive, QWidget* parent = Q_NULLPTR);
	SemaphorLabel(QWidget* parent = Q_NULLPTR);

public slots:
	void setState(int newState);
};
