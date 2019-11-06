#pragma once
// Qt 5 only imports
#ifdef QT_VERSION5X
#include <QtWidgets/QPushButton>
#else
//Qt 4  only imports
#include <QtGui/QPushButton>
#endif

/*
	This file contains QtWidgets with minor changes required to succesfully use them.
	Widget MUST be here if it adds only 1-2 signals or additional slot.
*/

namespace specwidgets
	// This namespace must contain transformed base widgets with minor changes and specializations of abstract widgets
{
	class indexedButton : public QPushButton
		// This class allows button to store and emit int value. is usefull for complex structures representation.
	{
		Q_OBJECT
	private:
		int index;  // int value which will be emitted after click
	public:
		indexedButton(int Index = 0, QWidget* parent = Q_NULLPTR);
		void setIndex(int Index = 0);		//	setter
	protected slots:
		virtual void clickCapt();					//	this slot is connected to own "clicked"
	signals:
		void indexClicked(int);				//	is emitted on-click
	};
	class toggledIndexedButton : public indexedButton
	{
		Q_OBJECT
	protected:
		QIcon& stateNon;
		QIcon& stateToggled;
		QIcon* curricon;
		void paintEvent(QPaintEvent*) override;
		virtual void clickCapt() override;
	public:
		toggledIndexedButton(QIcon& non, QIcon& is, int Index = 0, QWidget* parent = Q_NULLPTR);
	};
}
