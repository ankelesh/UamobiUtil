#pragma once
#include "widgets/parents/AbstractSearchAndPickWidget.h"



/*
	This file contains specialization of ASAPW for stillage selection.

	__ASSOCIATED_DB_FUNCTIONS__	:	P'parsedStillage' rec_list_stillages(text)


*/

namespace specwidgets
{
	class StillageSelectList : public AbstractListSelectionWidget
	{
		Q_OBJECT
	protected:
		QVector<parsedStillage>& coreStillList;
		// Inherited via AbstractListSelectionWidget
		virtual int countElems() override;
		virtual void itemSelectedFromList(QListWidgetItem*) override;
		virtual QString elemToString(int);
	public:
		StillageSelectList(QVector<parsedStillage>& cgl, QWidget* parent = Q_NULLPTR);
	signals:
		void stillageSelected(parsedStillage);
	};
}



class StillageSelectionWidget : public AbstractSearchAndPickWidget
{
	Q_OBJECT
protected:
	QVector<parsedStillage> stills;

	// Inherited via AbstractSearchAndPickWidget
	virtual void interpretResult() override;
public:
	StillageSelectionWidget(GlobalAppSettings& go, QWidget* parent);

protected slots:
	void stillageSelected(parsedStillage);

signals:
	void done(QString grp, QString pname);
};