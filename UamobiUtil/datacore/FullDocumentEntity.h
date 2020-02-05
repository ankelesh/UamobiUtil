#pragma once
#include "AbsEntityPrototype.h"



class FullDocumentEntity : public AbsRecEntity
{
public:
	QString docId;
	QString parentNr; 	
	QString dateStr; 	
	QString comment; 	
	QString supplier; 	
	bool inspect; 	
	bool closed; 	
	bool cancelled; 	
	bool locked;

	FullDocumentEntity(QString docid = QString(), QString parentnr = QString(),
		QString dateStr = QString(), QString comment = QString(),
		QString supplier = QString(), bool inspect = false, bool closed = false,
		bool cancelled = false, bool locked = false);
protected:
	// Inherited via AbsRecEntity
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual QString makeTitle() const override;
	virtual IdInt extractId() const override;
	virtual bool deepCompare(const AbsRecEntity* another) const override;
	virtual AbsRecEntity* fabricate() const override;
	virtual bool sortingCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const QSharedPointer<AbsRecEntity> another) const override;
};

typedef QSharedPointer<FullDocumentEntity> FullDocument;
typedef QVector<FullDocument> FDocList;