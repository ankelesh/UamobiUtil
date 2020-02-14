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
	virtual bool useAssociatedNetworkSendMethod(QStringList& arguments, RequestAwaiter* awaiter) const override;
	virtual bool useAssociatedNetworkGetMethod(QStringList& arguments, RequestAwaiter* awaiter) const override;
};
typedef QSharedPointer<ModeEntity> Mode;
typedef QVector<Mode> ModeList;



