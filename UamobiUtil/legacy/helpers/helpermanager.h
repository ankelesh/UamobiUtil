#ifndef HELPERMANAGER_H
#define HELPERMANAGER_H

#include <QObject>


//LEGACY
class HelperManager : public QObject
{
	Q_OBJECT
public:
	explicit HelperManager(QObject *parent = 0);
	static HelperManager *instance();

signals:
	void newCode(const QString &);

public slots:
	void enable();
	void disable();

private slots:
	void onNewCode(const QString &code);

private:
	bool m_enabled;
};

#endif // HELPERMANAGER_H
