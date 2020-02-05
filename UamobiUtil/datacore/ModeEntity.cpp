#include "ModeEntity.h"
#include "legacy/legacy.h"
#include "networking/dataupdateengine-http.h"
#include "widgets/utils/GlobalAppSettings.h"
ModeEntity::ModeEntity(QString Name, QString Mode, QString Submode)
	: AbsRecEntity(UniformXmlObject::Mode), name(Name), mode(Mode), submode(Submode)
{
}

void ModeEntity::drop()
{
	name.clear();
	mode.clear();
	submode.clear();
}

bool ModeEntity::isEmpty()
{
	return name.isEmpty() && mode.isEmpty();
}

bool ModeEntity::fromUniXml(const UniformXmlObject& o)
{
	if (o.myOID() == UniformXmlObject::Mode)
	{
		if (o.mySize() == 1)
		{
			name = o.value("modeName");
		}
		else
		{
			name = o.value("captionMode");
			mode = o.value("modeName");
			QPair<QString, QString > mpair = legacy::splitLegacyMode(mode);
			mode = mpair.first;
			submode = mpair.second;
			if (name.isEmpty() && !mode.isEmpty())
				name = mode;
		}
		if (mode.isEmpty() || name.isEmpty())
			throw InitializationError("modeName", o.value("modeName"));
		return true;
	}
	return false;
}

QString ModeEntity::makeTitle() const
{
	return name;
}

IdInt ModeEntity::extractId() const
{
	return 0;
}

bool ModeEntity::deepCompare(const AbsRecEntity* another) const
{
	const ModeEntity* temp = dynamic_cast<const ModeEntity*>(another);
	if (temp == Q_NULLPTR)
		return false;
	return (class_id == another->myType()) && (temp->mode == mode);
}

AbsRecEntity* ModeEntity::fabricate() const
{
	return new ModeEntity(*this);
}

bool ModeEntity::sortingCompare(const AbsRecEntity* another) const
{
	return this > another;
}

bool ModeEntity::sortingCompare(const QSharedPointer<AbsRecEntity> another) const
{
	return this > &(*( another));
}

bool ModeEntity::useAssociatedNetworkSendMethod(QStringList& arguments, RequestAwaiter* awaiter) const
{
	AppNetwork->execQueryByTemplate(QueryTemplates::selectMode, mode, submode, awaiter);
	return true;
}

bool ModeEntity::useAssociatedNetworkGetMethod(QStringList& arguments, RequestAwaiter* awaiter) const
{
	AppNetwork->execQueryByTemplate(QueryTemplates::modeList, AppSettings->language, awaiter);
	return true;
}
