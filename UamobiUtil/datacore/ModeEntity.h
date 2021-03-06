#pragma once
#include "AbsEntityPrototype.h"



class ModeEntity : public AbsRecEntity
{
public:
	QString name;	//	name
	QString mode;	//	mode - larger part
	QString submode;	//	submode - sometimes is 0
	ModeEntity(QString Name = "", QString Mode = "", QString Submode = "");
	void drop();
	bool isEmpty();
protected:
	// Inherited via AbsRecEntity
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual QString makeTitle() const override;
	virtual QString extractId() const override;
	virtual bool deepCompare(const AbsRecEntity* another) const override;
	virtual AbsRecEntity* fabricate() const override;
	virtual bool sortingCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const QSharedPointer<AbsRecEntity> another) const override;
    virtual bool useAssociatedNetworkSendMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;
    virtual bool useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const override;

	// Inherited via AbsRecEntity
	virtual void overwriteTitle(QString new_title) override;
	virtual void overwriteId(QString new_id) override;
};
typedef QSharedPointer<ModeEntity> Mode;
typedef QVector<Mode> ModeList;



