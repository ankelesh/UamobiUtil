#ifndef XML_FUNCS_H
#define XML_FUNCS_H

#include <QList>
#include <QSet>
#include <QHash>
#include <QFlags>
#include <QString>

#include "things.h"

class QDomDocument;

namespace XmlFns
{
	enum CodeInfoSpec {
		MustEnterSeries = 1,
		NotCountable = 2
	};

	Q_DECLARE_FLAGS(CodeInfoSpecs, CodeInfoSpec)

		bool parsErrors(const QString& xml, QString& errMsg);

	QList<UserProfile> profilesList(const QString& xml);
	QList<Place> placesList(const QString& xml);
	//QSet<QString>      modesList   (const QString &xml);
	QList<Mode> modesList(const QString& xml);
	QString codeInfoHtml(const QString& xml, const QString& ifNotFound, bool* ok = 0, CodeInfoSpecs* specs = 0);
	QString richdataHtml(const QString& xml, bool* ok = 0);
	QList<Document> documentsLists(const QString& xml, bool* ok = 0);
	Document documentInfo(const QString& xml, bool* allok, QString& errMsg);
	inline Document           documentInfo(const QString& xml, bool* ok = 0)
	{
		QString str;
		return documentInfo(xml, ok, str);
	}

	QList<DocResultItem> searchResult(const QString& xml, int* cntFrom = 0, int* cntTo = 0, bool* isLastPage = 0);
	QList<DocType>       invTypesList(const QString& xml);
	bool logIn(const QString& xml, QString& errMsg, QString& sessid);
	inline bool logIn(const QString& xml, QString& errMsg)
	{
		QString str;
		return logIn(xml, errMsg, str);
	}

	QString            printLabel(const QString& xml, bool* ok = 0);

	typedef QHash<QString, QString> HashGL;
	typedef QList<HashGL> GenericList;

	GenericList genericList(QDomDocument& doc,
		const QString& listRootTagName,
		const QString& listItemTagName,
		const QString& tag1 = QString(),
		const QString& tag2 = QString(),
		const QString& tag3 = QString(),
		const QString& tag4 = QString(),
		const QString& tag5 = QString(),
		const QString& tag6 = QString(),
		const QString& tag7 = QString(),
		const QString& tag8 = QString(),
		const QString& tag9 = QString());
	GenericList genericList(const QString& xml, const QString& listItemTagName,
		bool* ok = 0,
		const QString& tag1 = QString(),
		const QString& tag2 = QString(),
		const QString& tag3 = QString(),
		const QString& tag4 = QString(),
		const QString& tag5 = QString(),
		const QString& tag6 = QString(),
		const QString& tag7 = QString(),
		const QString& tag8 = QString(),
		const QString& tag9 = QString());
	QString getValByTagName(const QString& xml, QString tagNme);
	QList<QPair<QString, QString>> getMultipleCodes(const QString& xml);
}

Q_DECLARE_OPERATORS_FOR_FLAGS(XmlFns::CodeInfoSpecs)

#endif // XML_FUNCS_H
