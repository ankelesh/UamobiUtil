#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QWidget>
#include <QtWidgets/QBoxLayout>
#else
// Qt4 only widgets
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#endif
// widgets imports
#include "widgets/parents/inframedWidget.h"
#include "widgets/utils/SpecializedWidgets.h"
#include "widgets/parents/AbstractVariantSelectionWidget.h"
// networking imports
#include "networking/things.h"

/*
	This widget is a specialized avsw widget. It was basically first it's prototype, so it remains in global namespace
	For other info check AVSW widget
*/

class LoginSelectWidget : public AbstractVariantSelectionWidget
{
	Q_OBJECT
private:
	QVector<UserProfile>& profiles;
protected:
	// Inherited via AbstractVariantSelectionWidget
	virtual QString elemAsString(int index) override;
	virtual int countElems() override;
public:
	LoginSelectWidget(QVector<UserProfile>& Profiles, QWidget* parent = Q_NULLPTR);
private slots:
	virtual void indexSelected(int Index);
signals:
	void profilePicked(UserProfile);

};
