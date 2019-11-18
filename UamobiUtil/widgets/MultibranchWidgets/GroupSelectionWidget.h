#pragma once
#include "widgets/parents/AbstractListSelectionWidget.h"
#include "widgets/parents/AbstractSearchAndPickWidget.h"
#include <QBoxLayout>
#include "networking/things.h"
#include <QLabel>
#include "widgets/ElementWidgets/MegaIconButton.h"
#include "networking/RequestAwaiter.h"
#include "widgets/utils/GlobalAppSettings.h"

namespace specwidgets
{
	class GroupSelectList : public AbstractListSelectionWidget
	{
		Q_OBJECT
	protected:
		QVector<parsedGroup>& coreGroupList;
		// Inherited via AbstractListSelectionWidget
		virtual int countElems() override;
		virtual void itemSelectedFromList(QListWidgetItem*) override;
		virtual QString elemToString(int);
	public:
		GroupSelectList(QVector<parsedGroup>& cgl, QWidget* parent = Q_NULLPTR);
	signals:
		void groupSelected(parsedGroup);
	};
}




class GroupSelectionWidget : public AbstractSearchAndPickWidget
{
	Q_OBJECT
protected:
	QVector<parsedGroup> groups;

	// Inherited via AbstractSearchAndPickWidget
	virtual void interpretResult() override;
public:
	GroupSelectionWidget(GlobalAppSettings& go, QWidget* parent);

protected slots:
	void groupSelected(parsedGroup);

signals:
	void done(QString grp, QString pname);


};






