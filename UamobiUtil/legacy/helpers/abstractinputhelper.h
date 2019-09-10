#ifndef INPUTMETHOD_H
#define INPUTMETHOD_H

#include <QWidget>

class AbstractInputHelper : public QObject
{
	Q_OBJECT

public:
	explicit AbstractInputHelper(QObject *parent = 0);
	~AbstractInputHelper() {}

signals:
	void newCode(const QString &);

public slots:
	virtual bool run() = 0;


};

#endif // INPUTMETHOD_H
