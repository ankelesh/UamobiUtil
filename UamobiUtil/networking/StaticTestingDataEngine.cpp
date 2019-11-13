#include "StaticTestingDataEngine.h"
#include "debugtrace.h"
#define DEBUG
void StaticTestingDataEngine::pushQuery(const QString res, const QString err, QObject* receiver, const char* slot)
{
#ifdef DEBUG
	detrace_METHCALL("pushQuery " << res);
#endif
	pendingResponse.first = res;
	pendingResponse.second = err;
	receiverCoords.first = receiver;
	receiverCoords.second = slot;
	timer->start();
}

QString StaticTestingDataEngine::emulateDocList()
{
	QString temp;
	QTextStream out(&temp);

	out << "<?xml version=\"1.0\" encoding=\"CP1251\"?>"
		"<searchresult>"
		"<status>200</status>"
		"<page>"
		"<from>1</from>"
		"<to>" + QString::number(docListEmulation.count()) + " </to>"
		"<last>true</last>"
		"</page>";
	for (int i = 0; i < docListEmulation.count(); ++i)
	{
		out << docListEmulation.at(i);
	}

	out << "<allowdelete>true</allowdelete></searchresult>";
	out.flush();
	return temp;
}

StaticTestingDataEngine::StaticTestingDataEngine(int delay)
	: DataUpdateEngine(), timer(new QTimer(this)), pendingResponse(), receiverCoords(),
	currentmode(NoneModeS), docListEmulation(), prescaningCode(false)
{
	timer->setSingleShot(true);
	timer->setInterval(delay);
    class_id=2;

#ifdef QT_VERSION5X
	QObject::connect(timer, &QTimer::timeout, this, &StaticTestingDataEngine::delayOut);
#else
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(delayOut()));
#endif
}

void StaticTestingDataEngine::cancelPending()
{
}

void StaticTestingDataEngine::userUpdateList(QObject* receiver, const char* slot, const QString& placeCodeFilter)
{
	detrace_METHCALL("StaticTestingDataEngine::userUpdateList");
	pushQuery(
		"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
		"<users>"
		"<status>200</status>"
		"<manually>true</manually>"
		"<servicename>Unimarket</servicename></users>"
		,
		"",
		receiver, slot
	);
}

void StaticTestingDataEngine::userLogIn(const QString& acc, const QString& passwd, QObject* receiver, const char* slot, const float v)
{
	detrace_METHCALL("StaticTestingDataEngine::userLogin");
	pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
		"<login>"
		"<status>200</status>"
		"<session>371</session>"
		"</login>"
		,
		"",
		receiver, slot
	);
}

void StaticTestingDataEngine::userLogOut(QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::userLogout");
}

void StaticTestingDataEngine::modeList(QObject* receiver, const char* slot, const QString lang)
{
	detrace_METHCALL("StaticTestingDataEngine::modeList");
	pushQuery(QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
		"<modes>"
		"<status>200</status>"
		"<mode>"
		"<modeName>inventory</modeName>"
		"<captionMode>inventory</captionMode>"
		"</mode>"
		"<mode>"
		"<modeName>receipt</modeName>"
		"<captionMode>receipt</captionMode>"
		"</mode>"
		"<mode>"
		"<modeName>ordersandsldcorrection</modeName>"
		"<captionMode>ordersandsldcorrection</captionMode>"
		"</mode>"
		"<mode>"
		"<modeName>receiptfromwarehouses</modeName>"
		"<captionMode>receiptfromwarehouses</captionMode>"
		"</mode>"
		"</modes>")
		,
		"",
		receiver, slot
	);
}
void StaticTestingDataEngine::modeSelect(const QString& mode, const QString& submode, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::modeSelect");
	currentmode = makeStaticModeFromRequizites(mode, submode);
	if (currentmode == NoneModeS)
	{
		pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<mode>"
			"<status>404</status>"
			"</mode>", "", receiver, slot);
	}
	pushQuery(
		"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
		"<mode>"
		"<status>200</status>"
		"</mode>"
		, "", receiver, slot
	);
}

void StaticTestingDataEngine::placeList(QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::placeList");
	switch (currentmode)
	{
	case InvModeS:
	case RecModeS:
	case RecWarehModeS:
		pushQuery(
			"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<places>"
			"<status>200</status>"
			"<place>"
			"<code>209</code>"
			"<name>L-09 str. Matei Basarab 5/1</name>"
			"</place>"
			"</places>"

			, "", receiver, slot
		);
		break;
	case RecOrdSldCorrModeS:
		pushQuery(
			"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<places>"
			"<status>200</status>"
		"<place>"
			"<code>273</code>"
			"<name>L-73 or.Balti, str. Calea Iesilor 112/57</name>"
			"</place>"
			"</places>"

			, "", receiver, slot
		);
		break;
	}
}

void StaticTestingDataEngine::placeSelect(const QString& place, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::placeSelect");
	pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
		" <place>"
		"  <status>200</status>"
		" </place>"
		, "", receiver, slot);
}

void StaticTestingDataEngine::docLock(const QString& subdocCode, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::docLock");
}

void StaticTestingDataEngine::docUnlock(bool save, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::docunlock");
	pushQuery("_", "", receiver, slot);
	docListEmulation.clear();
}

void StaticTestingDataEngine::docGetAllowedTypes(QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::docGetAllowedTypes");
}

void StaticTestingDataEngine::docSetFilter(const QString& filter, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::docSetFilter");
}

void StaticTestingDataEngine::docGetAllowedToCreateTypes(QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::docGetAllowedToCreateTypes");
}

void StaticTestingDataEngine::docNewParent(const QString& sysfid, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::docNewParent");
}

void StaticTestingDataEngine::docSearchItems(const QString& text, const int page, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::docSearchItems");
}

void StaticTestingDataEngine::itemGetInfo(const QString& code, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::itemGetInfo");
    if (code.isEmpty())
        return;
	switch (currentmode)
	{
	case InvModeS:
		pushQuery("<?xml version = \"1.0\" encoding=\"CP1251\"?>"
			"<item>"
			"<status>200</status>"
			"<richdata>"
			"<![CDATA["
			"<div style=\"background: #d0ffd0\"><center>Товар/Объект</center></div>"
			"<div style=\"text-decoration: underline\"><b>Штрикод:</b><span style=\"\"> 5941132016420 </span>"
			"</div>"
			"<div><b>Наименование:</b><span style=\"font-weight: bold;\"> DR.OETKER Vanilie 8g </span>"
			"</div>"
			"<div style=\"font-size:5pt\" /> &nbsp; </div><div><b>Цена:</b><span style=\"font-weight: bold;\"> 2.9 </span>"
			"</div>"
			"<div style=\"font-size:5pt\" /> &nbsp; </div><div><b>Код:</b><span style=\"\"> 5730 </span>"
			"</div>"
			"<div><b>Накоплено:</b><span style=\"\"> 0 </span>"
			"</div>"
			"<div><b>Поставщик:</b><span style=\"\"> Forward International SRL </span>"
			"</div>"
			"<div><b>Группа:</b><span style=\"\"> 16.02 Домашние кондитерские </span>"
			"</div>"
			"]]>"
			"</richdata>"
			"<controls>"
			"<control>"
			"<name>qty</name>"
			"<default></default>"
			"</control>"
			"</controls>"
			"</item>", "", receiver, slot);
		break;
	case RecOrdSldCorrModeS:
		pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<item>"
			"<status>200</status>"
			"<nn></nn>"
			"<richdata>"
			"<![CDATA["
			"<div style=\"background: #d0f0c0\"><center>Товар/Объект</center></div>"
			"<div style=\"text-decoration: underline\">"
			"<b>Штрихкод:</b>"
			"<span>"+code+"</span>"
			"</div>"
			"<div>"
			"<b>Наименование:</b>"
			"<span><b>DR.OETKER Vanilie 8g</b></span>"
			"</div>"
			"<br />"
			"<div>"
			"<b>Цена без НДС:</b>"
			"<span><b>2.1</b></span>"
			"</div>"
			"<br>"
			"<div>"
			"<b>Код:</b>"
			"<span>5730</span>"
			"</div>"
			"<div>"
			"<b>Кол-во:</b>"
			"<span>50</span>"
			"</div>"
			"]]>"
			"</richdata>"
			"<controls>"
			"<control>"
			"<name>qty</name>"
			"<default></default>"
			"</control>"
			"<control>"
			"<name>addqty</name>"
			"<default></default>"
			"</control>"
			"</controls>"
			"</item>"
			, "", receiver, slot);
		break;
	case RecModeS:
	case RecWarehModeS:
		if (prescaningCode)
		{
			if (code.contains(QRegExp("[a-z]", Qt::CaseInsensitive)))
			{
				pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
					"<item>"
					"<status>200</status>"
					"<nn>" + code + "</nn>"
					"<richdata>"
					"<![CDATA["
					"<div style=\"background: #b0f0ff\"><center>Накладная</center></div>"
					"<div style=\"text-align: left\">Была выбрана накладная с номером \"" + code + "\".<br>Нажмите <b>Scan</b> для сканирования.</div>"
					"]]>"
					"</richdata>"
					"</item>", "", receiver, slot
				);
				prescaningCode = false;
				return;
			}
			else
			{
				pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
					"<item>"
					"<status>200</status>"
					"<nn>" + code + "</nn>"
					"<richdata>"
					"<![CDATA["
					"<div style=\"text-align: left\">Пожалуйста введите код накладной.</div>"
					"]]>"
					"</richdata>"
					"</item>", "", receiver, slot);
				return;
			}
		}
		else
		{
			switch (currentmode)
			{
			case RecModeS:
				pushQuery(
					"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
					"<item>"
					"<status>200</status>"
					"<nn>AA123455</nn>"
					"<richdata>"
					"<![CDATA["
					"<div style=\"background: #d0f0c0\"><center>Товар/Объект</center></div>"
					"<div style=\"text-decoration: underline\">"
					"<b>Штрихкод:</b>"
					"<span>" + code + "</span>"
					"</div>"
					"<div>"
					"<b>Наименование:</b>"
					"<span><b>FRANZELUTA Franzela EXTRA c/s amb.taiat 430g </b></span>"
					"</div>"
					"<br />"
					"<div>"
					"<b>Цена без НДС:</b>"
					"<span><b>4.34</b></span>"
					"</div>"
					"<br>"
					"<div>"
					"<b>Код:</b>"
					"<span>" + code + "</span>"
					"</div>"
					"<div>"
					"<b>Кол-во:</b>"
					"<span>0</span>"
					"</div>"
					"]]>"
					"</richdata>"
					"<controls>"
					"<control>"
					"<name>qty</name>"
					"<default></default>"
					"</control>"
					"</controls>"
					"</item>", "", receiver, slot);
				break;
			case RecWarehModeS:
				pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
					"<item>"
					"<status>200</status>"
					"<nn>AA123455</nn>"
					"<richdata>"
					"<![CDATA["
					"<div style=\"background: #d0f0c0\"><center>Товар/Объект</center></div>"
					"<div style=\"text-decoration: underline\">"
					"<b>Штрихкод:</b>"
					"<span>" + code + "</span>"
					"</div>"
					"<div>"
					"<b>Наименование:</b>"
					"<span><b>DR.OETKER Vanilie 8g</b></span>"
					"</div>"
					"<br />"
					"<div>"
					"<b>Цена без НДС:</b>"
					"<span><b>2.1</b></span>"
					"</div>"
					"<br>"
					"<div>"
					"<b>Код:</b>"
					"<span>5730</span>"
					"</div>"
					"<div>"
					"<b>Кол-во:</b>"
					"<span>0</span>"
					"</div>"
					"]]>"
					"</richdata>"
					"<controls>"
					"<control>"
					"<name>qty</name>"
					"<default></default>"
					"</control>"
					"</controls>"
					"</item>"
					, "", receiver, slot);
				return;
			}
		}
	}
}

void StaticTestingDataEngine::docGetResults(const QString& docId, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::docGetResults");
	switch (currentmode)
	{
	case InvModeS:
	case RecModeS:
	case RecWarehModeS:
	case RecOrdSldCorrModeS:
		pushQuery(
			emulateDocList(), "", receiver, slot
		);
		break;
	default:
		break;
	}
}

void StaticTestingDataEngine::docGetResults(int page, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::docGetResultsPaged");
	switch (currentmode)
	{
	case InvModeS:
	case RecModeS:
	case RecWarehModeS:
	case RecOrdSldCorrModeS:
		pushQuery(
			emulateDocList(), "", receiver, slot
		);
		break;
	default:
		break;
	}
}

void StaticTestingDataEngine::docResultGetBoxData(const QString& boxId, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::docresultGetBoxData");
}

void StaticTestingDataEngine::docResultEditByBoxID(const QString& boxId, const double qty, const double qtyOfPkg, const QString& lot, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::docResultEditByBoxId");
}

void StaticTestingDataEngine::docResultDeleteItem(const QString& boxId, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::docResultDeleteItem");
}

void StaticTestingDataEngine::docUpdateParams(const QString& docId, const bool cancelled, const bool closed, const QString& comment, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::docUpdateParams");
}

void StaticTestingDataEngine::docListParentDocs(QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::docListParentDocs");
}

void StaticTestingDataEngine::docListSubdocuments(QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::docListSubdocuments");
}

void StaticTestingDataEngine::getPhotoItemList(QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::getPhotoItemList");
}

void StaticTestingDataEngine::invGetParentDocsList(QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::invGetParentDocsList");
	switch (currentmode)
	{
	case InvModeS:
		pushQuery(
			"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<parentdocs>"
			"<status>200</status>"
			"<parentdoc>"
			"<cod>18847291</cod>"
			"<title>Doc. nr. 18847291</title>"
			"<text>(12204)  Инвентаризация -  товары по факту </text>"
			"<doctype>(12204)  Инвентаризация -  товары по факту </doctype>"
			"</parentdoc>"
			"</parentdocs>|"
			"got <?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<parentdocs>"
			"<status>200</status>"
			"<parentdoc>"
			"<cod>18847291</cod>"
			"<title>Doc. nr. 18847291</title>"
			"<text>(12204)  Инвентаризация -  товары по факту </text>"
			"<doctype>(12204)  Инвентаризация -  товары по факту </doctype>"
			"</parentdoc>"
			"</parentdocs>"
			, "", receiver, slot
		);
		break;
	}
}

void StaticTestingDataEngine::invNew(const QDate& date, const QString& parent, const QString& comment, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::invNew");
}

void StaticTestingDataEngine::invSubmit(const QString& code, const QString& qty, const QString& infofor, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::invSubmit");
    if (code.isEmpty())
        return;
	switch (currentmode)
	{
	case InvModeS:
		pushQuery(
			"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<item>"
			"<status>200</status>"
			"<richdata>"
			"<![CDATA["
			"<div style=\"background: #d0ffd0\"><center>Товар/Объект</center></div>"
			"<div style=\"text-decoration: underline\"><b>Штрикод:</b><span style=\"\">" + code + "</span>"
			"</div>"
			"<div><b>Наименование:</b><span style=\"font-weight: bold;\"> DR.OETKER Vanilie 8g </span>"
			"</div>"
			"<div style=\"font-size:5pt\" /> &nbsp; </div><div><b>Цена:</b><span style=\"font-weight: bold;\"> 2.9 </span>"
			"</div>"
			"<div style=\"font-size:5pt\" /> &nbsp; </div><div><b>Код:</b><span style=\"\"> 5730 </span>"
			"</div>"
			"<div><b>Накоплено:</b><span style=\"\">" + qty + "</span>"
			"</div>"
			"<div><b>Поставщик:</b><span style=\"\"> Forward International SRL </span>"
			"</div>"
			"<div><b>Группа:</b><span style=\"\"> 16.02 Домашние кондитерские </span>"
			"</div>"
			"]]>"
			"</richdata>"
			"</item>", "", receiver, slot
		);
		break;
	}
	docListEmulation << "<result>"
		"<barcode>" + code + "</barcode>"
		"<qty>" + qty + "</qty>"
		"<title>Item: " + code + "</title>"
		"</result>";
}

void StaticTestingDataEngine::invSubmitExpDates(const QString& code, const QString& expDatesString, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::invSubmitExpdates");
}

void StaticTestingDataEngine::recListSuppliers(const QString& text, const bool hasOrdersOnly, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::recListSuppliers");

	switch (currentmode)
	{
	case RecModeS:
		if (hasOrdersOnly)
		{
			if (text.startsWith('F') || text.isEmpty())
			{
				pushQuery(
					"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
					" <suppliers>"
					"  <status>200</status>"
					"  <supplier>"
					"   <code>14385</code>"
					"   <name>Franzeluta SA</name>"
					"   <orders>1</orders>"
					"  </supplier>"
					" </suppliers>"
					, "", receiver, slot
				);
			}
		}
		return;
	case RecOrdSldCorrModeS:
		pushQuery(
			"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			" <suppliers>"
			"  <status>200</status>"
		"<supplier>"
			"<code>14483</code>"
			"<name>Forward International SRL</name>"
			"<orders>12</orders>"
			"</supplier>"
					" </suppliers>"
			, "", receiver, slot
		);
		return;

	}
}

void StaticTestingDataEngine::recListOrders(const QString& supplier, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::recListOrders" << supplier);
	switch (currentmode)
	{
	case RecModeS:
		if (supplier.contains("14385"))
		{
			pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
				" <orders>"
				"<status>200</status>"
				"<order>"
				"<code>18847292</code>"
				"<title>№ заявки: 18847292</title>"
				"<text>Дата п-ки: 06.11.2019 - 07.11.2019</text>"
				"</order>"
				" <supplier>Franzeluta SA</supplier>"
				" </orders>", "", receiver, slot);
		}
		return;
	case RecWarehModeS:
		pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			" <orders>"
			"<status>200</status>"
			"<order>"
			"<code>18819857</code>"
			"<title>№ заявки: 18819857</title>"
			"<text>Дата п-ки: 05.09.2019 - 07.09.2019  </text>"
			"</order>"
			" <supplier>Sectia Uscate</supplier>"
			" </orders>", "", receiver, slot);
		return;
	case RecOrdSldCorrModeS:
		pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			" <orders>"
			"<status>200</status>"
			"<order>"
			"<code>18833981</code>"
			"<title>№ заявки: 18833981</title>"
			"<text>Дата п-ки: 06.09.2019 - 13.09.2019  </text>"
			"</order>"
			" <supplier>Forward International SRL</supplier>"
			" </orders>", "", receiver, slot);
		return;
	}
}

void StaticTestingDataEngine::recListOrdersByItem(const QString& barcode, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::recListOrdersByItem");
}

void StaticTestingDataEngine::recGetOrderInfo(const QString& ordercode, const QString& supplier, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::recGetOrderInfo");
	switch (currentmode)
	{
	case RecModeS:
		pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<order>"
			"<status>200</status>"
			"<richdata>"
			"<![CDATA["
			"<div><b>Поставщик:</b> Franzeluta SA</div>"
			"<div><b>Док. №:</b> 18847292</div>"
			"<hr><div>FRANZELUTA Napolitane BUREONCA halva 200g - 6</div>"
			"<div>FRANZELUTA Biscuiti DE CASA 500g  - 5</div>"
			"<div>FRANZELUTA Napolitane BUREONCA cacao 200g - 6</div>"
			"<div>FRANZELUTA Turte PICI 300g - 0</div>"
			"<div>FRANZELUTA Turte LUX prune 300g  - -8</div>"
			"<div>FRANZELUTA Biscuiti NUCA 500g  - -8</div>"
			"<div>FRANZELUTA Covrigei PICIUL cu mac 500g - -16</div>"
			"<div>FRANZELUTA Turte CU MAC 500g  - -11</div>"
			"<div>FRANZELUTA Foietaj cu cascaval amb.80g - 57</div>"
			"<div>FRANZELUTA Napolitane BUREONCA lapte 200g - -4</div>"
			"]]>"
			"</richdata>"
			"</order>"
			, "", receiver, slot);
		break;
	case RecWarehModeS:
		pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<order>"
			"<status>200</status>"
			"<richdata>"
			"<![CDATA["
			"<div><b>Поставщик:</b> Sectia Uscate</div>"
			"<div><b>Док. №:</b> 18819857</div>"
			"<hr><div>VICTORITA Halva stafida 350g - 0</div>"
			"<div>VICTORITA Halva arahida 350g - 0</div>"
			"<div>VICTORITA Halva cacao 350g - 0</div>"
			"<div>TEADOR Bomboane Prune in ciocolata 200g - 0</div>"
			"<div>TEADOR Bomboane Nucleo cu miez de nuci 200g - 0</div>"
			"<div>TEADOR Bomboane Nucleo cu arahide 200g - 0</div>"
			"<div>TEADOR Bomboane Surpriza albastra 200g - 0</div>"
			"<div>TEADOR Bomboane Caise in ciocolata 200g - 0</div>"
			"<div>RIFERO Bomboane prune cu nuca in ciocol. 230g - 0</div>"
			"<div>RIFERO Bomboane prune in ciocolata 230g - 0</div>"
			"]]>"
			"</richdata>"
			"</order>"
			, "", receiver, slot);
		break;
	case RecOrdSldCorrModeS:
		pushQuery(
			"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<order>"
			"<status>200</status>"
			"<richdata>"
			"<![CDATA["
			"<div><b>Поставщик:</b> Forward International SRL</div>"
			"<div><b>Док. №:</b> 18833981</div>"
			"<hr><div>DR.OETKER Zahar pudra 80g - 0</div>"
			"<div>DR.OETKER Cacao 50g - 0</div>"
			"<div>BANEASA Paste fainoase conchiglie 1kg - 0</div>"
			"<div>BANEASA Paste fainoase penne rigate 1kg - 0</div>"
			"<div>GALLINA BLANCA Bulion de gaina 80g - 0</div>"
			"<div>TYMBARK Suc portocala 200ml - 5</div>"
			"<div>TYMBARK Suc piersic 200ml - 5</div>"
			"<div>TYMBARK Suc exotic 200ml - 10</div>"
			"<div>LA FESTA Ciocolata classico 25g 1buc - 0</div>"
			"<div>АЛАДУШКИН Faina de griu 2kg - 0</div>"
			"]]>"
			"</richdata>"
			"</order>", "", receiver, slot);
		break;
	}
}

void StaticTestingDataEngine::recNew(const QDate& date, const QString& parent, const QString& comment, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::recNew");
	switch (currentmode) {
	case InvModeS:
		pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<newdoc>"
			"<status>200</status>"
			"<doc>"
			"<code>3751521</code>"
			"<parentnr>" + parent + "</parentnr>"
			"<supplier></supplier>"
			"<date>" + date.toString() + "</date>"
			"<comment>" + comment + "</comment>"
			"<closed>false</closed>"
			"<canceled>false</canceled>"
			"<locked>false</locked>"
			"<scanmode>simple</scanmode>"
			"</doc>"
			"</newdoc>"
			, "", receiver, slot);
		break;
	case RecModeS:
		pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<newdoc>"
			"<status>200</status>"
			"<doc>"
			"<code>3751521</code>"
			"<parentnr>" + parent + "</parentnr>"
			"<supplier>Franzeluta SA</supplier>"
			"<date>" + date.toString() + "</date>"
			"<comment>" + comment + "</comment>"
			"<closed>false</closed>"
			"<canceled>false</canceled>"
			"<locked>false</locked>"
			"<scanmode>simple</scanmode>"
			"</doc>"
			"</newdoc>"
			, "", receiver, slot);
		prescaningCode = true;
		break;
	case RecWarehModeS:
		pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<newdoc>"
			"<status>200</status>"
			"<doc>"
			"<code>3751531</code>"
			"<parentnr>18847309</parentnr>"
			"<supplier>Sectia Uscate</supplier>"
			"<date>08.11.2019</date>"
			"<comment></comment>"
			"<closed>false</closed>"
			"<canceled>false</canceled>"
			"<locked>false</locked>"
			"<scanmode>simple</scanmode>"
			"</doc>"
			"</newdoc>"
			, "", receiver, slot);
		prescaningCode = true;
		break;
	case RecOrdSldCorrModeS:
		pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<newdoc>"
			"<status>200</status>"
			"<doc>"
			"<code>3751532</code>"
			"<parentnr>18847282</parentnr>"
			"<supplier>Forward International SRL</supplier>"
			"<date>08.11.2019</date>"
			"<comment></comment>"
			"<closed>false</closed>"
			"<canceled>false</canceled>"
			"<locked>false</locked>"
			"<scanmode>simple</scanmode>"
			"</doc>"
			"</newdoc>"
			, "", receiver, slot);
		break;
	}
}

void StaticTestingDataEngine::recSubmit(const QString& code, const QString& num, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::recSubmit");
    if (code.isEmpty())
        return;
	switch (currentmode)
	{
	case RecModeS:
	case RecWarehModeS:
		if (prescaningCode)
		{
			pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
				"<item>"
				"<status>200</status>"
				"<nn>" + code + "</nn>"
				"<richdata>"
				"<![CDATA["
				"<div style=\"text-align: left\">Пожалуйста введите код накладной.</div>"
				"]]>"
				"</richdata>"
				"</item>", "", receiver, slot);
			return;
		}
		else
		{
			switch (currentmode)
			{
			case RecModeS:
				pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
					"<item>"
					"<status>200</status>"
					"<nn>AA123455</nn>"
					"<richdata>"
					"<![CDATA["
					"<div style=\"background: #d0f0c0\"><center>Товар/Объект</center></div>"
					"<div style=\"text-decoration: underline\">"
					"<b>Штрихкод:</b>"
					"<span>" + code + "</span>"
					"</div>"
					"<div>"
					"<b>Наименование:</b>"
					"<span><b>FRANZELUTA Franzela EXTRA c/s amb.taiat 430g </b></span>"
					"</div>"
					"<br />"
					"<div>"
					"<b>Цена без НДС:</b>"
					"<span><b>4.34</b></span>"
					"</div>"
					"<br>"
					"<div>"
					"<b>Код:</b>"
					"<span>114157</span>"
					"</div>"
					"<div>"
					"<b>Кол-во:</b>"
					"<span>" + num + "</span>"
					"</div>"
					"<div style=\"text-decoration: underline\">"
					"<b>Уже принято:</b>"
					"<span>4</span>"
					"</div>"
					"]]>"
					"</richdata>"
					"</item>", "", receiver, slot);
				docListEmulation << "<result>"
					"<barcode>" + code + "</barcode>"
					"<qty>" + num + "</qty>"
					"<title>Item: " + code + "</title>"
					"</result>";
				return;
			case RecWarehModeS:
				pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
					"<item>"
					"<status>200</status>"
					"<nn>AA123455</nn>"
					"<richdata>"
					"<![CDATA["
					"<div style=\"background: #d0f0c0\"><center>Товар/Объект</center></div>"
					"<div style=\"text-decoration: underline\">"
					"<b>Штрихкод:</b>""<span>" + code + "</span>"
					"</div>"
					"<div>"
					"<b>Наименование:</b>"
					"<span><b>FRANZELUTA Franzela ORASENEASCA PLUS amb.taiat 0.40kg </b></span>"
					"</div>"
					"<br />"
					"<div>"
					"<b>Цена без НДС:</b>"
					"<span><b>4.34</b></span>"
					"</div>"
					"<br>"
					"<div>"
					"<b>Код:</b>"
					"<span>73644</span>"
					"</div>"
					"<div>"
					"<b>Кол-во:</b>"
					"<span>" + num + "</span>"
					"</div>"
					"]]>"
					"</richdata>"
					"</item>"
					, "", receiver, slot);
				docListEmulation << "<result>"
					"<barcode>" + code + "</barcode>"
					"<qty>" + num + "</qty>"
					"<title>Item: " + code + "</title>"
					"</result>";
				return;
			}
		}
	}
}

void StaticTestingDataEngine::recSubmitExpDates(const QString& code, const QString& expDatesString, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::recSubmitExpDates");
}

void StaticTestingDataEngine::recTestingInfo(QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::recTestingInfo");
}

void StaticTestingDataEngine::docGetMultipleVersions(const QString& barcode, QObject* receiver, const char* slot)
{
}

void StaticTestingDataEngine::getWarehousesList(const QString& text, const bool hasOrd, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::getWarehousesList");
	switch (currentmode)
	{
	case RecWarehModeS:
		pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<suppliers>"
			"<status>200</status>"
			"<supplier>"
			"<code>124556</code>"
			"<name>Sectia Uscate</name>"
			"<orders>0</orders>"
			"</supplier>"
			"<announcement>"
			"<define>places = warehouses</define>"
			"</announcement>"
			"</suppliers>"
			, "", receiver, slot);
		break;
	}
}
void StaticTestingDataEngine::makeRequest(DataRequest* dr)
{
	detrace_METHCALL("StaticTestingDataEngine::makeRequest");
}

void StaticTestingDataEngine::delayOut()
{
#ifdef DEBUG
	detrace_METHEXPL("delayOut, pushing: " << pendingResponse.first << pendingResponse.second);
#endif
	if (!pendingResponse.first.isEmpty())
	{
		emit responseArrived(pendingResponse.first, pendingResponse.second);

		QMetaObject::invokeMethod(receiverCoords.first, receiverCoords.second,
			Q_ARG(QString, pendingResponse.first), Q_ARG(QString, pendingResponse.second));
	}
}

void StaticTestingDataEngine::setUrl(QString)
{
}

void StaticTestingDataEngine::recSubmit(const QString& code, const QString& num, const QString& parameters, QObject* receiver, const char* slot)
{
	detrace_METHCALL("StaticTestingDataEngine::recSubmitParams");
	switch (currentmode)
	{
	case RecOrdSldCorrModeS:
		pushQuery("<?xml version=\"1.0\" encoding=\"CP1251\"?>"
			"<item>"
			"<status>200</status>"
			"<nn></nn>"
			"<richdata>"
			"<![CDATA["
			"<div style=\"background: #d0f0c0\"><center>Товар/Объект</center></div>"
			"<div style=\"text-decoration: underline\">"
			"<b>Штрихкод:</b>"
			"<span>5941132016420</span>"
			"</div>"
			"<div>"
			"<b>Наименование:</b>"
			"<span><b>DR.OETKER Vanilie 8g</b></span>"
			"</div>"
			"<br />"
			"<div>"
			"<b>Цена без НДС:</b>"
			"<span><b>2.1</b></span>"
			"</div>"
			"<br>"
			"<div>"
			"<b>Код:</b>"
			"<span>5730</span>"
			"</div>"
			"<div>"
			"<b>Кол-во:</b>"
			"<span>50</span>"
			"</div>"
			"<div style=\"text-decoration: underline\">"
			"<b>Уже принято:</b>"
			"<span>33</span>"
			"</div>"
			"]]>"
			"</richdata>"
			"</item>"
			, "", receiver, slot);
		docListEmulation << "<result>"
			"<barcode>" + code + "</barcode>"
			"<qty>" + num + "</qty>"
			"<title>Item: " + code + "</title>"
			"</result>";
		break;
	}
}

StaticModes makeStaticModeFromRequizites(const QString& mode, const QString& submode)
{
	switch (mode.length())
	{
	case 9:
		if (mode == "inventory")
		{
			switch (submode.length())
			{
			case 0:
			default:
				return InvModeS;
			}
		}
		break;
	case 7:
		if (mode == "receipt")
		{
			switch (submode.length())
			{
			case 21:
				if (submode == "receiptfromwarehouses")
					return RecWarehModeS;
				break;
			case 22:
				if (submode == "ordersandsldcorrection")
					return RecOrdSldCorrModeS;
				break;
			case 0:
			default:
				return RecModeS;
			}
		}
	}
	return NoneModeS;
}
