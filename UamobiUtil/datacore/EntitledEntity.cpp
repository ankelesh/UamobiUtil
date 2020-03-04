#include "EntitledEntity.h"
#include "networking/dataupdateengine-http.h"

EntitledEntity::EntitledEntity(int subclassId, QString c, QString t, QString tx)
	: AbsRecEntity(subclassId), code(c), title(t), text(tx)
{
}


QString EntitledEntity::makeTitle() const
{
	return title;
}

QString EntitledEntity::extractId() const
{
	return code;
}

bool EntitledEntity::deepCompare(const AbsRecEntity* another) const
{
	const EntitledEntity* temp = dynamic_cast<const EntitledEntity*>(another);
	if (temp == Q_NULLPTR)
		return false;
	return (class_id == another->myType()) && (temp->code == code);
}


bool EntitledEntity::sortingCompare(const AbsRecEntity* another) const
{
	return getId() > another->getId();
}

bool EntitledEntity::sortingCompare(const QSharedPointer<AbsRecEntity> another) const
{
	return getId() > another->getId();
}

SupplierEntity::SupplierEntity(QString c, QString t, QString tx)
	: EntitledEntity(UniformXmlObject::Supplier, c, t, tx)
{
}

bool SupplierEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::Supplier)
	{
		code = o.value("code");
		title = o.value("name");
		text = o.value("orders");
		if (code.isEmpty() && title.isEmpty())
			throw InitializationError("nocode", o.value("code"));
		return true;
	}
	return false;
}

AbsRecEntity* SupplierEntity::fabricate() const
{
	return new SupplierEntity(*this);
}

bool SupplierEntity::useAssociatedNetworkSendMethod(const QStringList& /*arguments*/, RequestAwaiter* /*awaiter*/) const
{
	return false;
}

bool SupplierEntity::useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const
{
	if (arguments.count() < 2)
		return false;
	AppNetwork->execQueryByTemplate(QueryTemplates::receiptListSuppliers,arguments.at(0) , arguments.at(1) ,awaiter);
	return true;
}


OrderEntity::OrderEntity(QString c, QString t, QString tx)
	: EntitledEntity(UniformXmlObject::Order, c,t,tx)
{
}

bool OrderEntity::useAssociatedNetworkSendMethod(const QStringList& arguments, RequestAwaiter* awaiter) const
{
	AppNetwork->execQueryByTemplate(QueryTemplates::receiptGetOrderInfo, code, arguments.at(0),
		awaiter);
	return true;
}

bool OrderEntity::useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const
{
	if (arguments.isEmpty())
		return false;
	AppNetwork->execQueryByTemplate(QueryTemplates::receiptListOrders, "supplier=", arguments.first(), awaiter);
	return true;
}

bool OrderEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::Order)
	{
		code = o.value("code");
		title = o.value("title");
		text = o.value("text");
		if (code.isEmpty() && title.isEmpty())
			throw InitializationError("nocode", o.value("code"));
		return true;
	}
	return false;
}

AbsRecEntity* OrderEntity::fabricate() const
{
	return new OrderEntity(*this);
}

LesserDocumentEntity::LesserDocumentEntity(QString c, QString t, QString tx, QString dt)
	: EntitledEntity(UniformXmlObject::LesserDocument, c, t, tx), doctype(dt)
{

}

bool LesserDocumentEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::LesserDocument)
	{
		code = o.value("cod");
		title = o.value("title");
		text = o.value("text");
		doctype = o.value("doctype");
		if (code.isEmpty() && title.isEmpty())
			throw InitializationError("nocode", o.value("code"));
		return true;
	}
	return false;
}

AbsRecEntity* LesserDocumentEntity::fabricate() const
{
	return new LesserDocumentEntity(*this);
}

bool LesserDocumentEntity::useAssociatedNetworkGetMethod(const QStringList& /*arguments*/, RequestAwaiter* awaiter) const
{
	AppNetwork->execQueryByTemplate(QueryTemplates::inventoryListParentDocs, awaiter);
	return true;
}
