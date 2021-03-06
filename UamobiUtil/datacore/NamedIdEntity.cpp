#include "NamedIdEntity.h"
#include "networking/dataupdateengine-http.h"
#include "widgets/utils/GlobalAppSettings.h"
NamedIdEntity::NamedIdEntity(int subclassId, QString i, QString nm)
	: AbsRecEntity(subclassId), id(i), name(nm)
{
}

QString NamedIdEntity::makeTitle() const
{
	return name;
}

QString NamedIdEntity::extractId() const
{
	return id;
}

bool NamedIdEntity::deepCompare(const AbsRecEntity* another) const
{
	return (class_id == another->myType()) && (another->getId() == getId());
}


bool NamedIdEntity::sortingCompare(const AbsRecEntity* another) const
{
		return getId() > another->getId();
}

bool NamedIdEntity::sortingCompare(const QSharedPointer<AbsRecEntity> another) const
{
		return getId() > another->getId();
}

bool NamedIdEntity::fromUniXml(const UniformXmlObject& o)
{
	return false;
}

AbsRecEntity* NamedIdEntity::fabricate() const
{
    return Q_NULLPTR;
}

void NamedIdEntity::overwriteTitle(QString new_title)
{
	name = new_title;
}

void NamedIdEntity::overwriteId(QString new_id)
{
	id = new_id;
}

PlaceEntity::PlaceEntity(QString Id, QString nm)
    : NamedIdEntity(UniformXmlObject::Place, Id, nm)
{
}

void PlaceEntity::adjustQueryAndUseGet(QueryTemplates::QueryId quid, RequestAwaiter* awaiter)
{
	if (quid != QueryTemplates::placeList)
		return;
	AppNetwork->execQueryByTemplate(quid, awaiter);
}

bool PlaceEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::Place)
	{
		id = o.value("code");
		name = o.value("name");
		if (id.isEmpty())
			throw InitializationError("e1", o.value("e1"));
		if (name.isEmpty())
			name = id;
		return true;
	}
	return false;
}

bool PlaceEntity::useAssociatedNetworkSendMethod(const QStringList& /*arguments*/, RequestAwaiter* awaiter) const
{
	AppNetwork->execQueryByTemplate(QueryTemplates::selectPlace, id, awaiter);
	return true;
}

bool PlaceEntity::useAssociatedNetworkGetMethod(const QStringList& /*arguments*/, RequestAwaiter* awaiter) const
{
	AppNetwork->execQueryByTemplate(QueryTemplates::placeList, awaiter);
	return true;
}

AbsRecEntity* PlaceEntity::fabricate() const
{
	return new PlaceEntity(id, name);
}


GroupEntity::GroupEntity(QString Id, QString nm)
    : NamedIdEntity(UniformXmlObject::Group, Id, nm)
{
}

bool GroupEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::Group)
	{
		id = o.value("e1");
		name = o.value("e2");
		if ( name.isEmpty())
			throw InitializationError("e1", o.value("e1"));
		return true;
	}
	return false;
}
bool GroupEntity::useAssociatedNetworkSendMethod(const QStringList& /*arguments*/, RequestAwaiter* awaiter) const
{
	AppNetwork->execQueryByTemplate(QueryTemplates::applyBarcodeFilter, "groups", id, awaiter);
	return true;
}
bool GroupEntity::useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const
{
	if (arguments.count() < 2) return false;
	AppNetwork->execQueryByTemplate(QueryTemplates::recListTemplated, "groups", 
		arguments.first(), awaiter);
	return true;
}
AbsRecEntity* GroupEntity::fabricate() const
{
	return new GroupEntity(id, name);
}
StillageEntity::StillageEntity(QString Id, QString nm)
    : NamedIdEntity(UniformXmlObject::Stillage, Id, nm)
{
}
bool StillageEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::Stillage)
	{
		id = o.value("e1");
		name = o.value("e2");
		if (name.isEmpty())
			throw InitializationError("e1", o.value("e1"));
		return true;
	}
	return false;
}

bool StillageEntity::useAssociatedNetworkSendMethod(const QStringList& /*arguments*/, RequestAwaiter* awaiter) const
{
	AppNetwork->execQueryByTemplate(QueryTemplates::applyBarcodeFilter, "stillages", id, awaiter);
	return true;
}

bool StillageEntity::useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const
{
	if (arguments.count() < 2) return false;
	AppNetwork->execQueryByTemplate(QueryTemplates::recListTemplated, "stillages",
		arguments.first(), awaiter);
	return true;
}

AbsRecEntity* StillageEntity::fabricate() const
{
	return new StillageEntity(id, name);
}

UserEntity::UserEntity(QString Name, QString Login)
	: AbsRecEntity(UniformXmlObject::User), name(Name), login(Login)
{
}

void UserEntity::sendLoginRequest(const QString& password, RequestAwaiter* awaiter) const
{
    QStringList t;
    t<<password;
    useAssociatedNetworkSendMethod(t, awaiter);
}

bool UserEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::User)
	{
		login = o.value("login");
		name = o.value("name");
		if (login.isEmpty() || name.isEmpty())
			throw InitializationError("login", " empty ");
		return true;
	}
	return false;
}

QString UserEntity::makeTitle() const
{
	return name;
}

QString UserEntity::extractId() const
{
    return QString::number(reinterpret_cast<long long int>(this));
}

bool UserEntity::deepCompare(const AbsRecEntity* another) const
{
	const UserEntity* temp = upcastRecord<UserEntity>(another, this);
	if (temp == Q_NULLPTR)
		return false;
	return (login.contains(temp->login));
}

AbsRecEntity* UserEntity::fabricate() const
{
	return new UserEntity(*this);
}

bool UserEntity::sortingCompare(const AbsRecEntity* another) const
{
	return this > another;
}

bool UserEntity::sortingCompare(const QSharedPointer<AbsRecEntity> another) const
{
	return this > &(*another);
}

bool UserEntity::useAssociatedNetworkSendMethod(const QStringList& arguments, RequestAwaiter* awaiter) const
{
	if (arguments.isEmpty())
		return false;
	AppNetwork->execQueryOutsideSession(QueryTemplates::Login, login, arguments.first(),
        QString::number(double(VERSION)), awaiter);
	return true;
}

bool UserEntity::useAssociatedNetworkGetMethod(const QStringList& arguments, RequestAwaiter* awaiter) const
{
	if (arguments.isEmpty())
	{
		AppNetwork->execQueryOutsideSession(QueryTemplates::userList, "", awaiter);
	}
	else
	{
		AppNetwork->execQueryOutsideSession(QueryTemplates::userList, "&place=" + arguments.first(), awaiter);
	}
	return true;
}

void UserEntity::overwriteTitle(QString new_title)
{
	name = new_title;
}

void UserEntity::overwriteId(QString new_id)
{
}

DocTypeEntity::DocTypeEntity(QString Id, QString Name, bool selected)
	: NamedIdEntity(UniformXmlObject::Doctype, Id, Name), isSelected(selected)
{
}

void DocTypeEntity::sendGetRequest(RequestAwaiter* awaiter)
{
	AppNetwork->execQueryByTemplate(QueryTemplates::documentGetAllowedTypes, awaiter);
}

void DocTypeEntity::sendFilterList(QString ids, RequestAwaiter* awaiter)
{
	AppNetwork->execQueryByTemplate(QueryTemplates::documentSetFilter, ids, awaiter);
}

bool DocTypeEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::Doctype)
	{
		id = o.value("id");
		name = o.value("name");
		isSelected = !(o.value("filtered-out") == "true");
		if (id.isEmpty())
			throw InitializationError("id", " empty ");
		if (name.isEmpty())
			name = id;
		return true;
	}
	return false;
}

AbsRecEntity* DocTypeEntity::fabricate() const
{
	return new DocTypeEntity(*this);
}

double DocTypeEntity::extractEnumerable() const
{
	if (isSelected)
		return 1;
	return 0;
}

bool DocTypeEntity::useAssociatedNetworkGetMethod(const QStringList& /*arguments*/, RequestAwaiter* awaiter) const
{
	sendGetRequest(awaiter);
	return true;
}

BarcodeEntity::BarcodeEntity(QString code, QString Name)
    : NamedIdEntity(UniformXmlObject::Barcode, code, Name)
{
}

bool BarcodeEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::Barcode)
	{
		id = o.value("code");
		name = o.value("name");
		if (id.isEmpty())
			throw InitializationError("id", " empty ");
		if (name.isEmpty())
			name = id;
		return true;
	}
	return false;
}

AbsRecEntity* BarcodeEntity::fabricate() const
{
	return new BarcodeEntity(*this);
}

InvoiceEntity::InvoiceEntity(QString code, QString Name)
    : NamedIdEntity(UniformXmlObject::Invoice, code, Name)
{
}

bool InvoiceEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::Invoice)
	{
		id = o.value("code");
		name = o.value("title");
		if (id.isEmpty())
			throw InitializationError("id", " empty ");
		if (name.isEmpty())
			name = id;
		return true;
	}
	return false;
}

AbsRecEntity* InvoiceEntity::fabricate() const
{
	return new InvoiceEntity(*this);
}
