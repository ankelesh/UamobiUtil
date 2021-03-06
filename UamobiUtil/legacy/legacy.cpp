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
		t["ordersandsldcorrection"] = modecut("receipt", "ordersandsldcorrection");
		temp['o'] = t;
		t.clear();
		t["printing"] = modecut("printing", "");
		t["productionwriteoff"] = modecut("inventory", "productionwriteoff");
		t["partialinventory"] = modecut("inventory", "partialinventory");
		temp['p'] = t;
		t.clear();
		t["receipt"] = modecut("receipt", "");
		t["releasebuyer"] = modecut("receipt", "releasebuyer");
		t["redelivery"] = modecut("inventory", "redelivery");
		t["returning"] = modecut("receipt", "returning");
		t["returningtosupplier"] = modecut("receipt", "returningtosupplier");
		t["receiptworder"] = modecut("receipt", "receiptworder");
		t["receiptwoorder"] = modecut("receipt", "receiptwoorder");
		t["receiptfromwarehouses"] = modecut("receipt", "receiptfromwarehouses");
		temp['r'] = t;
		t.clear();
		t["sales"] = modecut("inventory", "sales");
		t["scaningact"] = modecut("inventory", "scaningact");
		temp['s'] = t;
		t.clear();
		t["tmcinfobylocation"] = modecut("printing", "tmcinfobylocation");
		temp['t'] = t;
		t.clear();
		t["univscanact"] = modecut("inventory", "univscanact");
		temp['u'] = t;
		return temp;
	}

	static QHash<QChar, QHash<QString, modecut> > legacymodes(_initlm());

	QPair<QString, QString> splitLegacyMode(QString& mode)
	{
		modecut m = legacymodes.value(mode.at(0)).value(mode);
		if (m.modename.isEmpty())
			return QPair<QString, QString>(mode, QString::null);

		return QPair<QString, QString>(m.modename, m.submodename);
	}


	int guessControlType(QString& cname)
	{
		if (cname.contains("qty"))
		{
			return 1;
		}
		return 0;
	}
}