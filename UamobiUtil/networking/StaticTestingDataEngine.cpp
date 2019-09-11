#include "StaticTestingDataEngine.h"

void StaticTestingDataEngine::pushQuery(const QString& res, const QString& err, QObject* receiver, const char* slot)
{
	pendingResponse.first = res;
	pendingResponse.second = err;
	receiverCoords.first = receiver;
	receiverCoords.second = slot;
	timer->start();
}

StaticTestingDataEngine::StaticTestingDataEngine(int delay)
	: DataUpdateEngine(), timer(new QTimer(this))
{
	timer->setSingleShot(true);
	timer->setInterval(delay);
	QObject::connect(timer, &QTimer::timeout, this, &StaticTestingDataEngine::delayOut);
}

void StaticTestingDataEngine::cancelPending()
{
}

void StaticTestingDataEngine::userUpdateList(QObject* receiver, const char* slot, const QString& placeCodeFilter)
{
	pushQuery(
		"<?xml version=\"1.0\" encoding=\"CP1251\"?>\n"
		" <users>\n"
		"  <status>200</status>\n"
		"  <user>\n"
		"   <login>Adm</login>\n"
		"   <name>101_Adm</name>\n"
		"   <br/>\n"
		"  </user>\n"
		"  <user>\n"
		"   <login>TSD1</login>\n"
		"   <name>522_Oper1</name>\n"
		"   <br/>\n"
		"  </user>\n"
		"  <user>\n"
		"   <login>DepAdm</login>\n"
		"   <name>900 - DepAdm</name>\n"
		"   <br/>\n"
		"  </user>\n"
		"  <user>\n"
		"   <login>VGrosu</login>\n"
		"   <name>901 - VGrosu</name>\n"
		"   <br/>\n"
		"  </user>\n"
		"  <servicename>Romina</servicename>\n"
		" </users>\n"
		,
		"",
		receiver, slot
	);
}

void StaticTestingDataEngine::userLogIn(const QString& acc, const QString& passwd, QObject* receiver, const char* slot)
{
	pushQuery(
		"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
		" <login>"
		"  <status>200</status>"
		" <session>38212</session>"
		" </login>"
		,
		"",
		receiver, slot
	);
}

void StaticTestingDataEngine::userLogOut(QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::modeList(QObject* receiver, const char* slot, const QString lang)
{
	pushQuery(
		"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
		" <modes>"
		"  <status>200</status>"
		" 	<mode>"
		" 		<modeName>receipt</modeName>"
		" 		<captionMode>Receipt</captionMode>"
		" 	</mode>"
		" 	<mode>"
		" 		<modeName>movingunload</modeName>"
		" 		<captionMode>Moving Unload( Put into cell )</captionMode>"
		" 	</mode>"
		" 	<mode>"
		" 		<modeName>movingload</modeName>"
		" 		<captionMode>Moving Load( Take from cell )</captionMode>"
		" 	</mode>"
		" 	<mode>"
		" 		<modeName>emitting</modeName>"
		" 		<captionMode>Emitting</captionMode>"
		" 	</mode>"
		" </modes>",
		"",
		receiver, slot
	);
}

void StaticTestingDataEngine::modeSelect(const QString& mode, const QString& submode, QObject* receiver, const char* slot)
{
	pushQuery(
		"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
		" <mode>"
		"  <status>200</status>"
		" <settings>"
		"   <s>"
		"     <name>skiplistplaces</name>"
		"     <value>false</value>"
		"   </s>"
		"   <s>"
		"     <name>skiplistsuppliers</name>"
		"     <value>false</value>"
		"   </s>"
		"   <s>"
		"     <name>allowbindingtobasedocbybarcode</name>"
		"     <value>false</value>"
		"   </s>"
		"   <s>"
		"     <name>enabledcontrols</name>"
		"     <value>1,2,3,8,9</value>  </s>"
		"   <s>"
		"     <name>autosubmit</name>"
		"     <value>false</value>"
		"   </s>"
		"   <s>"
		"     <name>autosave</name>"
		"     <value>false</value>"
		"   </s>"
		"   <s>"
		"     <name>activatebtnphoto</name>"
		"     <value>true</value>"
		"   </s>"
		" </settings>"
		" </mode>"
		, "", receiver, slot
	);
}

void StaticTestingDataEngine::placeList(QObject* receiver, const char* slot)
{
	pushQuery(
		"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
		" <places>"
		"  <status>200</status>"
		"  <place>"
		"   <code>916</code>"
		"   <name>DEPOZIT Produs Finit</name>"
		"  </place>"
		"  <place>"
		"   <code>4063</code>"
		"   <name>Productie (WIP)</name>"
		"  </place>"
		"  <place>"
		"   <code>4973</code>"
		"   <name>DEPOZIT Punti Presa</name>"
		"  </place>"
		"  <place>"
		"   <code>45244</code>"
		"   <name>DEPOZIT Camera SEGREGARE</name>"
		"  </place>"
		" </places>"
		, "", receiver, slot
	);
}

void StaticTestingDataEngine::placeSelect(const QString& place, QObject* receiver, const char* slot)
{
	pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
		" <place>"
		"  <status>200</status>"
		" </place>"
		, "", receiver, slot);
}

void StaticTestingDataEngine::docLock(const QString& subdocCode, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::docUnlock(bool save, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::docGetAllowedTypes(QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::docSetFilter(const QString& filter, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::docGetAllowedToCreateTypes(QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::docNewParent(const QString& sysfid, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::docSearchItems(const QString& text, const int page, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::itemGetInfo(const QString& code, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::docGetResults(const QString& docId, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::docGetResults(int page, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::docResultGetBoxData(const QString& boxId, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::docResultEditByBoxID(const QString& boxId, const double qty, const double qtyOfPkg, const QString& lot, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::docResultDeleteItem(const QString& boxId, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::docUpdateParams(const QString& docId, const bool cancelled, const bool closed, const QString& comment, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::docListParentDocs(QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::docListSubdocuments(QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::getPhotoItemList(QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::invGetParentDocsList(QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::invNew(const QDate& date, const QString& parent, const QString& comment, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::invSubmit(const QString& code, const QString& qty, const QString& infofor, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::invSubmitExpDates(const QString& code, const QString& expDatesString, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::recListSuppliers(const QString& text, const bool hasOrdersOnly, QObject* receiver, const char* slot)
{
	if (hasOrdersOnly)
	{
		pushQuery(
			"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			" <suppliers>"
			"  <status>200</status>"
			"  <supplier>"
			"   <code>29087</code>"
			"   <name>Saraa  Cablaggi SRL export </name>"
			"   <orders>8</orders>"
			"  </supplier>"
			" </suppliers>"
			, "", receiver, slot
		);
	}
	else
	{
		QString supp = "HUARAA";
		if (supp.contains(text.toUpper()))
		{
			pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
				" <suppliers>"
				"  <status>200</status>"
				"  <supplier>"
				"   <code>1488</code>"
				"   <name>HUARAA ZIGA EXPORT </name>"
				"   <orders>1488</orders>"
				"  </supplier>"
				" </suppliers>",
				"", receiver, slot
			);
		}
		else
		{
			pushQuery("< ? xml version = \"1.0\" encoding=\"CP1251\"?>"
				" <suppliers>"
				"  <status>404</status>"
				"  <message>ORA-06502: PL/SQL: : буфер символьных строк слишком маленький ошибка числа или значения</message>"
				"  <stack>ORA-06512: на  \"ROMINA.PG_MOBILE_WEB\", line 5629"
				" </stack>"
				" </suppliers>"
				, "", receiver, slot);
		}
	}
}

void StaticTestingDataEngine::recListOrders(const QString& supplier, QObject* receiver, const char* slot)
{
	if (supplier == "29087")
	{
		pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			" <orders>"
			"  <status>200</status>"
			"  <order>"
			"   <code>247895</code>"
			"   <title>Nr. cererii: 247895, FPR133/09.09.2019</title>"
			"   <text>Data livr.: 09.09.2019  </text>"
			"  </order>"
			"  <order>"
			"   <code>247411</code>"
			"   <title>Nr. cererii: 247411, FPR132/06.09.2019</title>"
			"   <text>Data livr.: 06.09.2019  </text>"
			"  </order>"
			"  <order>"
			"   <code>246927</code>"
			"   <title>Nr. cererii: 246927, FPR131/05.09.2019</title>"
			"   <text>Data livr.: 05.09.2019  </text>"
			"  </order>"
			"  <order>"
			"   <code>246915</code>"
			"   <title>Nr. cererii: 246915, FPR130/05.09.2019</title>"
			"   <text>Data livr.: 05.09.2019  </text>"
			"  </order>"
			"  <order>"
			"   <code>245544</code>"
			"   <title>Nr. cererii: 245544, FPR129/02.09.2019</title>"
			"   <text>Data livr.: 02.09.2019  </text>"
			"  </order>"
			"  <order>"
			"   <code>245024</code>"
			"   <title>Nr. cererii: 245024, FPR128/30.08.2019</title>"
			"   <text>Data livr.: 30.08.2019  </text>"
			"  </order>"
			"  <order>"
			"   <code>244529</code>"
			"   <title>Nr. cererii: 244529, FPR127/29.08.2019</title>"
			"   <text>Data livr.: 29.08.2019  </text>"
			"  </order>"
			"  <order>"
			"   <code>243651</code>"
			"   <title>Nr. cererii: 243651, FPR126/26.08.2019</title>"
			"   <text>Data livr.: 26.08.2019  </text>"
			"  </order>"
			" <supplier>Saraa  Cablaggi SRL export </supplier>"
			" </orders>", "", receiver, slot);
	}
	else
	{
		pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			" <orders>"
			"  <status>200</status>"

			" <supplier>HUARAA ZIGA export </supplier>"
			" </orders>", "", receiver,slot);
	}
}

void StaticTestingDataEngine::recListOrdersByItem(const QString& barcode, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::recGetOrderInfo(const QString& ordercode, const QString& supplier, QObject* receiver, const char* slot)
{
	pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
		"<order>"
		"<status>200</status>"
		"<richdata>"
		"<![CDATA["
		"<div><b>Furnizor:</b> Saraa  Cablaggi SRL export </div>"
		"<div><b>Doc. nr.:</b> 247895</div>"
		"]]>"
		"</richdata>"
		"</order>"
		, "", receiver, slot);
}

void StaticTestingDataEngine::recNew(const QDate& date, const QString& parent, const QString& comment, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::recSubmit(const QString& code, const QString& num, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::recSubmitExpDates(const QString& code, const QString& expDatesString, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::recTestingInfo(QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::docGetMultipleVersions(const QString& barcode, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::getWarehousesList(const QString& text, const bool hasOrd, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::makeRequest(DataRequest* dr)
{
}

void StaticTestingDataEngine::delayOut()
{
	if (!pendingResponse.first.isEmpty())
	{
		emit responseArrived(pendingResponse.first, pendingResponse.second);

		QMetaObject::invokeMethod(receiverCoords.first, receiverCoords.second,
			Q_ARG(QString, pendingResponse.first), Q_ARG(QString, pendingResponse.second));
		pendingResponse.first.clear();
		pendingResponse.second.clear();
	}
}
