#pragma once
#include "networking/Parsers/abs_parsed_request.h"
#include "UniformXmlObject.h"
#include <QSharedPointer>
#include <qmetatype.h>
#include <exception>
#include "networking/RequestAwaiter.h"
typedef long long int IdInt;

class AbsRecEntity
{
protected:
	int class_id;

	// attempts to get from object corresponding data
	virtual bool fromUniXml(const UniformXmlObject & o) = 0;
	// returns string which can be used as representation of this object
	virtual QString makeTitle() const = 0;
	// returns value that can be considered as id of this. If it has no id, you can return memory address
	virtual IdInt extractId() const = 0;
	// performs type and equality comparation
	virtual bool deepCompare(const AbsRecEntity* another) const = 0;
	// constructs copy of the object
	virtual AbsRecEntity* fabricate() const = 0;
	// these methods are used to sort items in the models. It must be a valid comparator. 
	// You can compare memory addresses if you have nothing to compare.
	virtual bool sortingCompare(const AbsRecEntity* another) const = 0;
	virtual bool sortingCompare(const QSharedPointer<AbsRecEntity> another) const = 0;
	// extracts value that can be used in counting models. Not required for normal functioning.
	virtual int extractEnumerable() const;
	// sends request that is associated with default sending operation. Arguments are usually ignored
	virtual bool useAssociatedNetworkSendMethod(QStringList& arguments, RequestAwaiter* awaiter) const;
	// sends request that is associated with acquiring list of corresponding objects.
	virtual bool useAssociatedNetworkGetMethod(QStringList& arguments, RequestAwaiter* awaiter) const;
public:
	explicit AbsRecEntity(const int class_id = 0);
	bool fromXmlObject(const UniformXmlObject& o);
	bool fromXmlObject(const XmlObject o);
	
	QString getTitle() const;
	IdInt getId() const;
	int myType() const;
	bool sendAssociatedGetRequest(QStringList& arguments, RequestAwaiter* awaiter) const;
	bool sendAssociatedPostRequest(QStringList& arguments, RequestAwaiter* awaiter) const;
	int getAttachedNumber() const;
	bool isSame(AbsRecEntity* another) const;
	AbsRecEntity* clone() const;
	bool isHigher(AbsRecEntity* another) const;
	bool isHigher(QSharedPointer<AbsRecEntity> another) const;
};
typedef QSharedPointer<AbsRecEntity> RecEntity;
typedef QVector<QSharedPointer<AbsRecEntity> > Records;

template < class T>
Records downcastRecords(const QVector<T>& v)
{
	Records r;
	QVector<T>::const_iterator b = v.begin();
	while (b != v.end())
	{
		r << b->staticCast<AbsRecEntity>();
	}
	return r;
}

template <class T>
T* upcastRecord(AbsRecEntity* e)
{
	if (e == Q_NULLPTR)
		return Q_NULLPTR;
	T t;
	if (e->myType() == t.myType())
	{
		return static_cast<T*>(e);
	}
	return Q_NULLPTR;
}

template <class T>
T* upcastRecord(const AbsRecEntity* e)
{
	if (e == Q_NULLPTR)
		return Q_NULLPTR;
	T t;
	if (e->myType() == t.myType())
	{
		return static_cast<T*>(e);
	}
	return Q_NULLPTR;
}
template <class T>
const T* upcastRecord(const AbsRecEntity* e, const AbsRecEntity* prototype)
{
	if (e == Q_NULLPTR)
		return Q_NULLPTR;
	if (e->myType() == prototype->myType())
	{
		return static_cast<const T*>(e);
	}
	return Q_NULLPTR;
}
template <class T>
QSharedPointer<T> upcastRecord(RecEntity e)
{
	if (e.isNull())
		return Q_NULLPTR;
	T t;
	if (e->myType() == t.myType())
	{
		return e.staticCast<T>();
	}
	return Q_NULLPTR;
}
template <class T>
QSharedPointer<T> upcastRecord(RecEntity e, QSharedPointer<T> prototype)
{
	if (e.isNull())
		return Q_NULLPTR;
	if (e->myType() == prototype->myType())
	{
		return e.staticCast<T>();
	}
	return Q_NULLPTR;
}
Q_DECLARE_METATYPE(RecEntity);

class InitializationError : public std::exception
	// Initialization error is thrown when initialization is interrupted, but not denied
{
private:
	std::string msg;
public:
	InitializationError(QString field_name, QString value)
		: msg("Error initializing entity with provided values ")
	{
		msg += (field_name + " : " + value).toStdString();
	};
	virtual const char* what() const noexcept override { return msg.c_str(); };
};

template <class NetObject>
class NetRequestResponse
{
public:
	bool isError;
	int alternative_result;
	QString errtext;
	QVector<QSharedPointer<NetObject>> objects;
	XmlObjects additionalObjects;

	explicit NetRequestResponse()
		: isError(false), alternative_result(0), errtext(), objects(), additionalObjects() {};
	explicit NetRequestResponse(QString etext)
		: isError(true), alternative_result(0), errtext(etext), objects(), additionalObjects() {};
	explicit NetRequestResponse(QVector<QSharedPointer<NetObject>>& o)
		: isError(false), alternative_result(0), errtext(), objects(o), additionalObjects()
	{

	}
	NetRequestResponse(int ares, QVector<QSharedPointer<NetObject>>& o = QVector<QSharedPointer<NetObject>>())
		: isError(false), alternative_result(ares), errtext(), objects(o), additionalObjects()
	{

	}
	NetRequestResponse(bool ie, int ares, QString etext, QVector<QSharedPointer<NetObject>> o)
		: isError(ie), alternative_result(ares), errtext(etext), objects(o), additionalObjects()
	{

	}

	bool isNormal() const
	{
		return !isError && alternative_result == 0;
	}
	bool isEmpty() const
	{
		return objects.isEmpty();
	}
	bool isNormalAndNotEmpty() const {
		return isNormal() && !isEmpty();
	}
	bool fromHeterogenicXmlObjects(const XmlObjects& objs, RecEntity prototype, int ares = 0)
	{
		alternative_result = ares;
		objects.clear();
		additionalObjects.clear();
		bool ok;
		for (int i = 0; i < objs.count(); ++i)
		{
			try
			{
				if (objs.at(i)->myOID() == prototype->myType())
				{
					ok = prototype->fromXmlObject(objs.at(i));
					if (!ok)
					{
						isError = true;
						errtext = "initialization non-throwing error";
						return false;
					}
					objects.push_back(QSharedPointer<NetObject>(static_cast<NetObject*>(prototype->clone())));
				}
				else
				{
					additionalObjects.push_back(objs.at(i));
				}
			}
			catch (InitializationError & ie)
			{
				isError = true;
				errtext = ie.what();
				return false;
			}
		}
		return true;
	}
	bool fromHomogenicXmlObjects(const XmlObjects& objs, RecEntity prototype, int ares = 0)
	{
		alternative_result = ares;
		bool ok = true;
		for (int i = 0; i < objs.count(); ++i)
		{
			try
			{
				ok = prototype->fromXmlObject(objs.at(i));
				if (!ok)
				{
					return false;
				}
				objects.push_back(QSharedPointer<NetObject>(static_cast<NetObject*>(prototype->clone())));
			}
			catch (InitializationError & ie)
			{
				isError = true;
				errtext = ie.what();
				return false;
			}
		}
		return true;
	}
	
};

typedef NetRequestResponse<AbsRecEntity> PolyResponse;

template <>
bool NetRequestResponse<AbsRecEntity>::fromHomogenicXmlObjects(const XmlObjects& objs, RecEntity prototype, int ares)
{
	alternative_result = ares;
	bool ok = true;
	for (int i = 0; i < objs.count(); ++i)
	{
		try
		{
			ok = prototype->fromXmlObject(objs.at(i));
			if (!ok)
			{
				return false;
			}
			objects.push_back(RecEntity(prototype->clone()));
		}
		catch (InitializationError & ie)
		{
			isError = true;
			errtext = ie.what();
			return false;
		}
	}
	return true;
}
template<>
bool NetRequestResponse<AbsRecEntity>::fromHeterogenicXmlObjects(const XmlObjects& objs, RecEntity prototype, int ares)
{
	alternative_result = ares;
	objects.clear();
	additionalObjects.clear();
	bool ok;
	for (int i = 0; i < objs.count(); ++i)
	{
		try
		{
			if (objs.at(i)->myOID() == prototype->myType())
			{
				ok = prototype->fromXmlObject(objs.at(i));
				if (!ok)
				{
					isError = true;
					errtext = "initialization non-throwing error";
					return false;
				}
				objects.push_back(QSharedPointer<AbsRecEntity>(prototype->clone()));
			}
			else
			{
				additionalObjects.push_back(objs.at(i));
			}
		}
		catch (InitializationError & ie)
		{
			isError = true;
			errtext = ie.what();
			return false;
		}
	}
	return true;
}