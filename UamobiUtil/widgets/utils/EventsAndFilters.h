#pragma once
#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtGui/QKeyEvent>
/*
	This file contains everything related to handling events. Mostly KeyEvents
		NoKeyEvents:
			Despite it's name it does not eat key releases: it just intercepts them and
			emits corresponding signals. Main goal of this class is to keep outer reactions
			of widget while shutting down it's own. Example of usage: class ScaningWidget.
			There is barcodelist - a ListWidget, which must not react to number key presses,
			but all numbers must be counted. So this filter intercepts them and puts into signals.

		CaptureBackFilter:
			This filter is lighter version of NoKeyEvents. Only reacts to back key.

*/

namespace filters
{
	class NoKeyEvents : public QObject	//	intercepts key events to emit own signals
	{
		Q_OBJECT
	protected:

		bool eventFilter(QObject* object, QEvent* ev);

	public:
		NoKeyEvents(QObject* ob) : QObject(ob) {};
	signals:
		void numberObtained(QString);	//	holds number ready to be appended
		void returnObtained();			//	return was pressed
		void backRequired();	//	back or escape was pressed
		void unknownObtained(); //  unknown key code was captured. Usually they belong to device-specific buttons
		void eraseRequired();	//  backspace was pressed
	};

	class CaptureBackFilter : public QObject
	{
		Q_OBJECT
	protected:
		bool eventFilter(QObject* object, QEvent* ev);
	public:
		CaptureBackFilter(QObject* ob) : QObject(ob) {};
	signals:
		void backRequired();
	};
    class LineEditHelper : public QObject
    {
        Q_OBJECT
    protected:
        bool eventFilter(QObject *watched, QEvent *event) override;
    public:
        LineEditHelper(QObject* ob) : QObject(ob){};
    };
	class GeneralPurposeFilter : public QObject
	{
		Q_OBJECT
	public:
		enum captureTypes{Number,Return, Back, Scan, Symbol, All, infPack};
	protected:
		
		captureTypes mytypes[5];
		int typeLen;
		typedef	bool(GeneralPurposeFilter::* CaptureFunction)(QKeyEvent*);
		CaptureFunction captureFunction;

		CaptureFunction someCaptureFunctions[5];

		CaptureFunction makeCaptureFunction(captureTypes type);

		bool eventFilter(QObject* ob, QEvent* ev) override;
		bool captReturn(QKeyEvent*);
		bool captNumber(QKeyEvent*);
		bool captSymbol(QKeyEvent*);
		bool captBack(QKeyEvent*);
		bool captAll(QKeyEvent*);
		bool captSome(QKeyEvent*);
		bool captScan(QKeyEvent*);
		bool captInf(QKeyEvent*);

	public:
		GeneralPurposeFilter(captureTypes type = All, QObject* parent = Q_NULLPTR);
		GeneralPurposeFilter(captureTypes type1, captureTypes type2, QObject* parent = Q_NULLPTR, captureTypes type3 = All, captureTypes type4 = All, captureTypes type5 = All);
	signals:
		void returnPressed();
		void backPressed();
		void scanPressed();
		void numberPressed(int);
		void symbolPressed(QString);
	};
}
