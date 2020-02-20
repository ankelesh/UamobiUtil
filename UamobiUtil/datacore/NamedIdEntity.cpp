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

PlaceEntity::PlaceEntity(QString id, QString nm)
	: NamedIdEntity(UniformXmlObject::Place, id, nm)
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

bool PlaceEntity::useAssociatedNetworkSendMethod(QStringList& /*arguments*/, RequestAwaiter* awaiter) const
{
	AppNetwork->execQueryByTemplate(QueryTemplates::selectPlace, id, awaiter);
	return true;
}

bool PlaceEntity::useAssociatedNetworkGetMethod(QStringList& /*arguments*/, RequestAwaiter* awaiter) const
{
	AppNetwork->execQueryByTemplate(QueryTemplates::placeList, awaiter);
	return true;
}

AbsRecEntity* PlaceEntity::fabricate() const
{
	return new PlaceEntity(id, name);
}


GroupEntity::GroupEntity(QString id, QString nm)
	: NamedIdEntity(UniformXmlObject::Group, id, nm)
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
bool GroupEntity::useAssociatedNetworkSendMethod(QStringList& /*arguments*/, RequestAwaiter* awaiter) const
{
	AppNetwork->execQueryByTemplate(QueryTemplates::applyBarcodeFilter, "groups", id, awaiter);
	return true;
}
bool GroupEntity::useAssociatedNetworkGetMethod(QStringList& arguments, RequestAwaiter* awaiter) const
{
	if (arguments.count() < 2) return false;
	AppNetwork->execQueryByTemplate(QueryTemplates::recListTemplated, "groups", 
		arguments.first(), arguments.at(1), awaiter);
	return true;
}
AbsRecEntity* GroupEntity::fabricate() const
{
	return new GroupEntity(id, name);
}
StillageEntity::StillageEntity(QString id, QString nm)
	: NamedIdEntity(UniformXmlObject::Stillage, id, nm)
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

bool StillageEntity::useAssociatedNetworkSendMethod(QStringList& /*arguments*/, RequestAwaiter* awaiter) const
{
	AppNetwork->execQueryByTemplate(QueryTemplates::applyBarcodeFilter, "stillages", id, awaiter);
	return true;
}

bool StillageEntity::useAssociatedNetworkGetMethod(QStringList& arguments, RequestAwaiter* awaiter) const
{
	if (arguments.count() < 2) return false;
	AppNetwork->execQueryByTemplate(QueryTemplates::recListTemplated, "stillages",
		arguments.first(), arguments.at(1), awaiter);
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
	return QString::number((long long int)this);
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

bool UserEntity::useAssociatedNetworkSendMethod(QStringList& arguments, RequestAwaiter* awaiter) const
{
	if (arguments.isEmpty())
		return false;
	AppNetwork->execQueryOutsideSession(QueryTemplates::Login, login, arguments.first(),
		QString::number(VERSION), awaiter);
	return true;
}

bool UserEntity::useAssociatedNetworkGetMethod(QStringList& arguments, RequestAwaiter* awaiter) const
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

int DocTypeEntity::extractEnumerable() const
{
	if (isSelected)
		return 1;
	return 0;
}

bool DocTypeEntity::useAssociatedNetworkGetMethod(QStringList& /*arguments*/, RequestAwaiter* awaiter) const
{
	sendGetRequest(awaiter);
	return true;
}

BarcodeEntity::BarcodeEntity(QString code, QString name)
	: NamedIdEntity(UniformXmlObject::Barcode, code, name)
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
