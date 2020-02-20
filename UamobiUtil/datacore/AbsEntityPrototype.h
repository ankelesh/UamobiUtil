#pragma once
#include "UniformXmlObject.h"
#include <QtCore/QSharedPointer>
#include <QtCore/qmetatype.h>
#include <exception>
#include "networking/RequestAwaiter.h"

/*
	This file contains abstract class for all entities used in application. 
	This class allows widgets to ignore entity type and be polymorthic without 
	losing type bounds.
*/


class AbsRecEntity
{
protected:
	// id used for emulation of dynamic casting
	int class_id;

	// attempts to get from object corresponding data
	virtual bool fromUniXml(const UniformXmlObject & o) = 0;
	// returns string which can be used as representation of this object
	virtual QString makeTitle() const = 0;
	// returns value that can be considered as id of this. If it has no id, you can return memory address
	virtual QString extractId() const = 0;
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

	// interfaces
	bool fromXmlObject(const UniformXmlObject& o);
	bool fromXmlObject(const XmlObject o);
	
	QString getTitle() const;
	QString getId() const;
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



/*
	next section is emulating dynamic_cast for platforms which has no dynamic casts.
*/



template < class T>
Records downcastRecords(const QVector<T>& v)
// casts records vector into basic class to allow inserting this vector in data model
{
	Records r;
	QVector<T>::const_iterator b = v.begin();
	while (b != v.end())
	{
		r << b++->staticCast<AbsRecEntity>();
	}
	return r;
}

template <class T>
T* upcastRecord(AbsRecEntity* e)
// performs upcast by creating prototype and using it's type as control value
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
// performs upcast by creating prototype and using it's type as control value. Not working (qualifiers)
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
// performs upcast using prototype's type as control value
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
// performs upcast of shared pointer
{
	if (e.isNull())
        return QSharedPointer<T>();
	T t;
	if (e->myType() == t.myType())
	{
		return e.staticCast<T>();
	}
    return QSharedPointer<T>();
}
template <class T>
QSharedPointer<T> upcastRecord(RecEntity e, QSharedPointer<T> prototype)
// performs upcast of shared pointer using prototype's type
{
	if (e.isNull())
        return QSharedPointer<T>();
	if (e->myType() == prototype->myType())
	{
		return e.staticCast<T>();
	}
    return QSharedPointer<T>();
}
Q_DECLARE_METATYPE(RecEntity);

class InitializationError : public std::exception
	// Initialization error is thrown when initialization is interrupted, but not denied
{
	// wince has no strings
#ifdef Q_OS_WINCE
public:
    InitializationError(QString field_name, QString value)
    {};
    virtual const char* what() const override { return "init error of record"; };
#else
private:
	std::string msg;
public:
	InitializationError(QString field_name, QString value)
		: msg("Error initializing entity with provided values ")
	{
		msg += (field_name + " : " + value).toStdString();
	};
    virtual const char* what() const override { return msg.c_str(); };
#endif
};

/*
	Next section contains class of response container. It stores homogenic vector of 
	templated enitities, vector of unparsed heterogenic entities and technical values.
	You can use it to extract from uniform xml objects real objects.
*/


template <class NetObject>
class NetRequestResponse
	// stores parsed homogenic entities and unparsed objects
{
public:
	// determines if request was parsed succesfully
	bool isError;
	// stores result which was considered alterative (not normal, but not wrong)
	// use this value if responses to the same request can be different
	int alternative_result;
	// stores error log
	QString errtext;
	// stores parsed objects
	QVector<QSharedPointer<NetObject>> objects;
	// stores unparsed objects as pointers to enhance performance. Warning - this can affect your long 
	// stored results
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
		// treats list as unknown objects and performs type checking. All non compatible objects
		// are moved into additional objects
	{
		alternative_result = ares;
		objects.clear();
		additionalObjects.clear();
		bool ok;
		for (int i = 0; i < objs.count(); ++i)
			// for each object
		{
			try
				//initialization may throw InitializationError
			{
				if (objs.at(i)->myOID() == prototype->myType())
					// compatibility check
				{
					ok = prototype->fromXmlObject(objs.at(i));
					// if initialization was correct, ok == true
					if (!ok)
					{
						isError = true;
						errtext = "initialization non-throwing error";
						return false;
					}
					// upcasting clone of prototype
					objects.push_back(QSharedPointer<NetObject>(static_cast<NetObject*>(prototype->clone())));
				}
				else
					// if check failed = push object to unparsed vector
				{
					additionalObjects.push_back(objs.at(i));
				}
			}
			catch (InitializationError & ie)
				// stop initialization
			{
				isError = true;
				errtext = ie.what();
				return false;
			}
		}
		return true;
	}
	bool fromHomogenicXmlObjects(const XmlObjects& objs, RecEntity prototype, int ares = 0)
		// treats list as homogenic objects and does not checks their type. Can be dangerous
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
// This specialization stops polymorthic response from casting parsing results
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
			// only difference between non-specialized and specialized versions
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
// This specialization stops polymorthic response from casting parsing results
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
				// only difference between non-specialized and specialized versions
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
