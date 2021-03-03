#include "FullDocumentEntity.h"

FullDocumentEntity::FullDocumentEntity(QString docid, QString parentnr, 
	QString datestr, QString comm, QString supp,
	bool insp, bool clos, bool cancel, bool lcked)
	: AbsRecEntity(UniformXmlObject::Document), docId(docid),
	parentNr(parentnr),dateStr(datestr),comment(comm),
	supplier(supp), inspect(insp), closed(clos), cancelled(cancel),
	locked(lcked)
{
}

bool FullDocumentEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::Document)
	{
		docId = o.value("code");
		parentNr = o.value("parentnr");
		dateStr = o.value("date");
		comment = o.value("comment");
		supplier = o.value("supplier");
		inspect = o.value("inspect") == "true";
		closed = o.value("closed") == "true";
		cancelled = o.value("cancelled") == "true";
		locked = o.value("locked") == "true";
		if (docId.isEmpty() && parentNr.isEmpty())
			throw InitializationError("nocode", o.value("code"));
		return true;
	}
	return false;
}

QString FullDocumentEntity::makeTitle() const
{
	return docId;
}

QString FullDocumentEntity::extractId() const
{
	return docId;
}

bool FullDocumentEntity::deepCompare(const AbsRecEntity* another) const
{
	if (myType() == another->myType())
	{
		return docId == static_cast<const FullDocumentEntity*>(another)->docId;
	}
	return false;
}

AbsRecEntity* FullDocumentEntity::fabricate() const
{
	return new FullDocumentEntity(*this);
}

bool FullDocumentEntity::sortingCompare(const AbsRecEntity* another) const
{
	return getId() > another->getId();
}

bool FullDocumentEntity::sortingCompare(const QSharedPointer<AbsRecEntity> another) const
{
	return getId() > another->getId();
}

void FullDocumentEntity::overwriteTitle(QString new_title)
{
	docId = new_title;
}

void FullDocumentEntity::overwriteId(QString new_id)
{
	docId = new_id;
}
