#include "legacy.h"
#include <QtCore/QVector>
namespace legacy {
	struct modecut		//	nameless mode
	{
		QString modename;
		QString submodename;
		modecut(QString mn, QString smn) : modename(mn), submodename(smn) {}
		modecut() {}
	};

	QHash<QChar, QHash<QString, modecut> > _initlm()
		// Because app must be compiled in vs2005, this workaround is required to initialize static map
	{
		QHash<QChar, QHash<QString, modecut> > temp;
		QHash<QString, modecut> t;
		// map uses alphabetic sorting to enhance speed. May be too complicated.
		t["adjustment"] = modecut("inventory", "adjustment");
		temp['a'] = t;
		t.clear();
		t["batchprinting"] = modecut("inventory", "batchprinting");
		temp['b'] = t;
		t.clear();
		t["confirm"] = modecut("inventory", "confirm");
		temp['c'] = t;
		t.clear();
		t["decomplectation"] = modecut("inventory", "decomplectation");
		temp['d'] = t;
		t.clear();
		t["emitting"] = modecut("receipt", "emitting");
		temp['e'] = t;
		t.clear();
		t["finishedproductreceipt"] = modecut("receipt", "finishedproductreceipt");
		t["finishedproductreturn"] = modecut("receipt", "finishedproductreturn");
		temp['f'] = t;
		t.clear();
		t["goodandproductmove"] = modecut("inventory", "goodandproductmove");
		temp['g'] = t;
		t.clear();
		t["inventory"] = modecut("inventory", "");
		t["initialdownload"] = modecut("inventory", "initialdownload");
		t["instrumentdelivery"] = modecut("receipt", "instrumentdelivery");
		t["instrumentredelivery"] = modecut("receipt", "instrumentredelivery");
		t["instrumentdelivery"] = modecut("receipt", "instrumentdelivery");
		t["instrumentredelivery"] = modecut("receipt", "instrumentredelivery");
		t["inventoryoc"] = modecut("inventory", "inventoryoc");
		t["import"] = modecut("inventory", "import");
		t["instrumentdelivery"] = modecut("receipt", "instrumentdelivery");
		t["instrumentredelivery"] = modecut("receipt", "instrumentredelivery");
		temp['i'] = t;
		t.clear();
		t["locationinfo"] = modecut("printing", "locationinfo");
		temp['l'] = t;
		t.clear();
		t["movingload"] = modecut("inventory", "movingload");
		t["movingunload"] = modecut("inventory", "movingunload");
		t["materialshipment"] = modecut("receipt", "materialshipment");
		t["movingtorepacking"] = modecut("receipt", "movingtorepacking");
		temp['m'] = t;
		t.clear();
		t["printing"] = modecut("printing", "");
		t["productionwriteoff"] = modecut("inventory", "productionwriteoff");
		temp['p'] = t;
		t.clear();
		t["receipt"] = modecut("receipt", "");
		t["releasebuyer"] = modecut("receipt", "releasebuyer");
		t["redelivery"] = modecut("inventory", "redelivery");
		t["returning"] = modecut("receipt", "returning");
		t["returningtosupplier"] = modecut("receipt", "returningtosupplier");
		t["receiptworder"] = modecut("receipt", "receiptworder");
		t["receiptwoorder"] = modecut("receipt", "receiptwoorder");
		t["receiptfromwarehouses"] = modecut("receipt", "ReceiptFromWarehouse");
		temp['r'] = t;
		t.clear();
		t["sales"] = modecut("inventory", "sales");
		t["scaningact"] = modecut("inventory", "scaningact");
		temp['s'] = t;
		t.clear();
		t["tmcinfobylocation"] = modecut("printing", "tmcinfobylocation");
		temp['t'] = t;
		t.clear();
		return temp;
	}

	static QHash<QChar, QHash<QString, modecut> > legacymodes(_initlm());

	parsedMode parseLegacyMode(QString& name, QString& mode)
	{
		modecut m = legacymodes.value(mode.at(0)).value(mode);
		parsedMode tmp;
		tmp.name = name;
		tmp.mode = m.modename;
		tmp.submode = m.submodename;
		return tmp;
	}
	void filterNonCompatibleModes(QVector < parsedMode>& vect)
	{
		QVector<parsedMode> tvect;
		tvect.reserve(2);
		QVector<parsedMode>::iterator start = vect.begin();
		while (start != vect.end())
		{
			if (start->mode == "receipt")
			{
				if (start->submode.isEmpty()) {
					tvect << *start;
				}
				else if (start->submode == "ReceiptFromWarehouse")
				{
					tvect << *start;
				}
			}
			++start;
		}
		vect = tvect;
	}
}
