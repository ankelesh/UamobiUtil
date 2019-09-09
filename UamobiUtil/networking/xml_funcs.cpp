#include <QDomDocument>
#ifdef GUI_APP
#include <QTextDocument>
#endif
#include <QRegExp>
#include <QDebug>
#include <QTextDecoder>

#include "xml_funcs.h"
#include "debugtrace.h"

namespace XmlFns
{

	static QDomDocument doc;

	QList<UserProfile> profilesList(const QString &xml)
	{
		QList<UserProfile> list;

		QString err;
		int line, col;
		if (!doc.setContent(xml, &err, &line, &col)) {
			//qDebug() << err << line << col << "\n" << xml;
			return list;
		}
		QDomNode data = doc.elementsByTagName("users").at(0);
		int size = data.childNodes().size();
		for (int i = 0; i < size; i++) {
			QDomElement el = data.childNodes().at(i).toElement();
			UserProfile u;
			u.login = el.elementsByTagName("login").at(0).toElement().text();
			u.name  = el.elementsByTagName("name") .at(0).toElement().text();
			if (!u.login.isEmpty() && !u.name.isEmpty())
				list << u;
		}
		return list;
	}

	QList<Place> placesList(const QString &xml)
	{
		QList<Place> list;

		if (!doc.setContent(xml))
			return list;
		QDomNode data = doc.elementsByTagName("places").at(0);
		int size = data.childNodes().size();
		for (int i = 0; i < size; i++) {
			QDomElement el = data.childNodes().at(i).toElement();
			Place pl;
			pl.code = el.elementsByTagName("code").at(0).toElement().text();
			pl.name = el.elementsByTagName("name").at(0).toElement().text();
			if (!pl.code.isEmpty() && !pl.name.isEmpty())
				list << pl;
		}
		return list;
	}

    QList<Mode> modesList( const QString &xml )
    {
        int line, col;
        QString err;
        QList<Mode> list;
        // если зафиксирована ошибка, то выйти;
        if( !doc.setContent( xml, &err, &line, &col ) ) {
            //qDebug() << err << line << col << "\n" << xml;
            return list;
        }
        //--
        QDomNode data = doc.elementsByTagName( "modes" ).at( 0 );
        int size = data.childNodes().size();
        for( int i = 0; i < size; i++ ) {
            Mode m;
            QDomElement el = data.childNodes().at( i ).toElement();
            //--
            m.name = el.elementsByTagName( "modeName" ).at( 0 ).toElement().text();
            m.caption = el.elementsByTagName( "captionMode" ) .at( 0 ).toElement().text();
            //--
            if( !m.name.isEmpty() ) {
                list << m;
            }
        }
        //--
        return list;
    }

    /*QSet<QString> modesList(const QString &xml)
	{
		QSet<QString> set;

		if (!doc.setContent(xml))
			return set;
		QDomNodeList data = doc.elementsByTagName("modes").at(0).toElement().elementsByTagName("mode");
		int size = data.size();
		for (int i = 0; i < size; i++) {
			QDomElement el = data.at(i).toElement();
			set.insert(el.toElement().text());
		}
		return set;
    }*/

	QString codeInfoHtml(const QString &xml, const QString &ifNotFound, bool *correct, CodeInfoSpecs *specs)
	{
		bool ok = false;
		QString err;
		int line, col;
		if (!doc.setContent(xml, &err, &line, &col)) {
			//qDebug() << xml << "\n" << err << line << col;
			if (correct)
				*correct = false;
#ifdef GUI_APP
			return "<center><b>"+QObject::tr("Reading code info failed.")+"</b></center>"
					"<hr>"
					"<div style=\"text-align:left\">Server sent: <br />"
                    + Qt::mightBeRichText(xml)+
					"</div>";
#else
			return "<center><b>" + QObject::tr("Reading code info failed.") + "</b></center>"
				"<hr>"
				"<div style=\"text-align:left\">Server sent: <br />"
				+ xml +
				"</div>";
#endif
		}
		int status = doc.elementsByTagName("status").at(0).toElement().text().simplified().toInt();
		if (status != 200) {
			if (status != 404) {
				ok = false;
				if (correct)
					*correct = ok;
				return QObject::tr("<center><b>Data error.</b></center>");
			} else {
				ok = true;
				if (correct)
					*correct = ok;
				return QObject::tr("<center><b>Data for %1 not found.</b></center>"
										 "<div style=\"text-align: center\">"
										 "Data for code '%1' not found, but information about item can be added to resolve this problem later."
										 "</div>").arg(ifNotFound);
			}
		}
		ok = true;
		if (specs) {
			QDomElement n = doc.elementsByTagName("special").at(0).toElement();
			if (n.elementsByTagName("enterseries").at(0).toElement().text() == "true")
				*specs |= MustEnterSeries;
			if (n.elementsByTagName("countable").at(0).toElement().text() == "false")
				*specs |= NotCountable;

		}
		if (correct)
			*correct = ok;
		return doc.elementsByTagName("richdata").at(0).toElement().text();
	}

	QString richdataHtml(const QString &xml, bool *ok)
	{
		QString err;
		int col, line;
		if (!doc.setContent(xml, &err, &line, &col)) {
			//qDebug() << xml << "\n" << line << col;
			if (ok)
				*ok = false;
			return "";
		}

		if (ok)
			*ok = doc.elementsByTagName("status").at(0).toElement().text() == "200";

		return doc.elementsByTagName("richdata").at(0).toElement().text();

	}

	QList<Document> documentsLists(const QString &xml, bool *ok)
	{
		QList<Document> list;

		QString err;
		int col, line;
		if (!doc.setContent(xml, &err, &line, &col)) {
			//qDebug() << xml << "\n" << line << col;
			if (ok)
				*ok = false;
			return list;
		}

		QDomNode data = doc.documentElement();
		int size = data.childNodes().size();
		for (int i = 0; i < size; i++) {
			QDomElement el = data.childNodes().at(i).toElement();
			if (el.tagName() != "doc")
				continue;

			Document doc;
			doc.locked    = el.elementsByTagName("locked")  .at(0).toElement().text() == "true";
			doc.closed    = el.elementsByTagName("closed")  .at(0).toElement().text() == "true";
			doc.cancelled = el.elementsByTagName("canceled").at(0).toElement().text() == "true";
			doc.docId     = el.elementsByTagName("code")    .at(0).toElement().text();
			doc.parentNr  = el.elementsByTagName("parentnr").at(0).toElement().text();
			doc.dateStr   = el.elementsByTagName("date")    .at(0).toElement().text();
			doc.supplier  = el.elementsByTagName("supplier").at(0).toElement().text();
			list << doc;
		}
		if (ok)
			*ok = data.toElement().elementsByTagName("status").at(0).toElement().text() == "200";
		return list;
	}

	QList<DocResultItem> searchResult(const QString &xml, int *cntFrom, int *cntTo, bool *isLastPage)
	{
		QList<DocResultItem> list;

		QString err;
		int lnum = -1, cnum = -1;
		if (!doc.setContent(xml, &err, &lnum, &cnum)) {
			//qDebug() << xml;
			//qDebug() << err << lnum << cnum;
			return list;
		}
		QDomNode data = doc.elementsByTagName("searchresult").at(0);
		if (cntFrom && cntTo) {
			QDomElement page = data.toElement().elementsByTagName("page").at(0).toElement();
			*cntFrom = page.elementsByTagName("from").at(0).toElement().text().toInt();
			*cntTo   = page.elementsByTagName("to")  .at(0).toElement().text().toInt();
			if (isLastPage)
				*isLastPage = page.elementsByTagName("last").at(0).toElement().text() == "true";
		}
		QDomNodeList results = data.toElement().elementsByTagName("result");
		int size = results.size();
		for (int i = 0; i < size; i++) {
			QDomElement el = results.at(i).toElement();
			DocResultItem res;
			res.code    = el.elementsByTagName("code") .at(0).toElement().text();
			res.title   = el.elementsByTagName("title").at(0).toElement().text();
			res.num     = el.elementsByTagName("num")  .at(0).toElement().text().toFloat();
			list << res;
		}
		return list;
	}

	QList<DocType> invTypesList(const QString &xml)
	{
		QList<DocType> list;

		if (!doc.setContent(xml))
			return list;
		QDomNode data = doc.elementsByTagName("doctypes").at(0);
		int size = data.childNodes().size();
		for (int i = 0; i < size; i++) {
			QDomElement el = data.childNodes().at(i).toElement();
			if (el.tagName() != "type")
				continue;
			list << DocType(
						  el.elementsByTagName("code") .at(0).toElement().text(),
						  el.elementsByTagName("title").at(0).toElement().text()
						  );
		}
		return list;
	}

	Document documentInfo(const QString &xml, bool *allok, QString &errMsg)
	{
		Document i;

		QString err;
		int line, col;
		if (!doc.setContent(xml, &err, &line, &col)) {
			if (allok)
				*allok = false;
			//qDebug() << Q_FUNC_INFO << xml << err << line << col;
			return i;
        }

		QDomElement el = doc.documentElement();
		if (allok) {
			*allok = el.elementsByTagName("status") .at(0).toElement().text() == "200";
			errMsg = el.elementsByTagName("message").at(0).toElement().text();
        }
		i.locked    = el.attribute("locked")   == "true";
		i.cancelled = el.attribute("canceled") == "true";
		i.closed    = el.attribute("closed")   == "true";
		i.inspect   = el.attribute("inspect")  == "true";
		i.dateStr   = el.elementsByTagName("date")    .at(0).toElement().text();
		i.comment   = el.elementsByTagName("comment") .at(0).toElement().text();
		i.docId     = el.elementsByTagName("code")    .at(0).toElement().text();
		i.parentNr  = el.elementsByTagName("parentnr").at(0).toElement().text();
		i.supplier  = el.elementsByTagName("supplier").at(0).toElement().text();
		/*QString mode = el.elementsByTagName("scanmode").at(0).toElement().text();
		if (mode == "continuous")
			i.scanMode = AbstractScanWidget::SM_Continous;
		else if (mode == "autosubmit")
			i.scanMode = AbstractScanWidget::SM_ContinousWithAutosubmit;
		else
			i.scanMode = AbstractScanWidget::SM_Simple;*/
		*allok = !i.parentNr.isEmpty() && !i.docId.isEmpty();
		if (!*allok && errMsg.isEmpty()) {
			errMsg = el.firstChildElement("error").text();
		}
		return i;
	}

	bool logIn(const QString &xml, QString &errMsg, QString &sessid)
	{
		QString err;
		if (!doc.setContent(xml, &err)) {
			errMsg = QObject::tr("Unknown internal error.")+"\n"+err;
			return false;
		}
		QDomElement el = doc.elementsByTagName("login").at(0).toElement();
		sessid = el.elementsByTagName("session").at(0).toElement().text();
		errMsg = el.elementsByTagName("message").at(0).toElement().text();
		return el.elementsByTagName("status").at(0).toElement().text() == "200";
	}

	QString printLabel(const QString &xml, bool *ok)
	{
		if (!doc.setContent(xml))
			return QString();

		QDomElement el = doc.documentElement();
		QString code   = el.elementsByTagName("code").at(0).toElement().text();
		if (ok)
			*ok = (el.elementsByTagName("status").at(0).toElement().text() == "200")
					&& !code.isEmpty();
		return code;
	}

	GenericList
	genericList(QDomDocument &doc,
					const QString &listRootTagName,
					const QString &listItemTagName,
					const QString &tag1,
					const QString &tag2,
					const QString &tag3,
					const QString &tag4,
					const QString &tag5,
					const QString &tag6,
					const QString &tag7,
					const QString &tag8,
					const QString &tag9)
	{
		GenericList result;
		QList<QString> tagsList;
		tagsList << tag1 << tag2 << tag3 << tag4 << tag5 << tag6 << tag7 << tag8 << tag9;
		for (int i = tagsList.size()-1; i>-1; i--)
			if (tagsList.at(i).isEmpty())
				tagsList.removeAt(i);

		QDomElement listroot;
		if (listRootTagName.isEmpty())
			listroot	= doc.documentElement();
		else
			listroot = doc.documentElement().elementsByTagName(listRootTagName).at(0).toElement();
		QDomNode n        = listroot.firstChild();
		while (!n.isNull()) {
			if (n.nodeName() != listItemTagName) {
				n = n.nextSibling();
				continue;
			}
			HashGL hash;
			QDomElement el = n.toElement();
			if (tagsList.size())
				foreach (QString tag, tagsList)
					hash.insert(tag, el.elementsByTagName(tag).at(0).toElement().text());
			else
				hash.insert("value", el.text());
			n = n.nextSibling();
			result << hash;
		}
		return result;
	}

	GenericList
	genericList(const QString &xml, const QString &listItemTagName,
					bool *ok,
					const QString &tag1,
					const QString &tag2,
					const QString &tag3,
					const QString &tag4,
					const QString &tag5,
					const QString &tag6,
					const QString &tag7,
					const QString &tag8,
					const QString &tag9)
	{
		QString err;
		int line, col;
		if (!doc.setContent(xml, &err, &line, &col)) {
			//qDebug() << xml << "\n" << err << line << col;
			if (ok)
				*ok = false;
			return GenericList();
		}
		if (ok)
			*ok = true;
        return genericList(doc, QString(), listItemTagName, tag1, tag2, tag3, tag4, tag5, tag6, tag7, tag8, tag9);
    }

    bool parsErrors(const QString &xml,QString &errMsg)
    {
        QString err;
        if (!doc.setContent(xml, &err)) {
            errMsg = QObject::tr("Unknown internal error.")+"\n"+err;
            return false;
        }
        QDomElement el = doc.elementsByTagName("status").at(0).toElement();
        if(el.text().compare("404") == 0)
        {
            QDomElement el = doc.elementsByTagName("message").at(0).toElement();
                errMsg = el.text();
            return true;
        }
        else return false;
    }
    QString getValByTagName(const QString &xml,QString tagNme)
    {
        if (!doc.setContent(xml))
            return QString();
        return doc.elementsByTagName(tagNme).at(0).toElement().text();
    }
    QList<QPair<QString,QString>> getMultipleCodes(const QString & xml)
    {
                                  detrace_METHCALL("getMultipleCodes")
         QList<QPair<QString, QString> > li;
         QPair<QString, QString> entry;
         QDomDocument doc;

         doc.setContent(xml);
         if (doc.elementsByTagName("state").at(0).toElement().text() == "S"){
            QDomNodeList dmndl = doc.elementsByTagName("oneentry");
            for (int i = 0; i < dmndl.count(); ++i)
            {
                entry.first = dmndl.at(i).childNodes().at(0).toElement().text();
                entry.second = dmndl.at(i).childNodes().at(1).toElement().text();
                detrace_METHEXPL("entry node value: " << dmndl.at(i).toElement().text())
                li.push_back(entry);
            }
         }
         else
        {
           entry.first = doc.elementsByTagName("bcoderesult").at(0).toElement().text();
           li.push_back(entry);
        }
         return li;

    }
}
