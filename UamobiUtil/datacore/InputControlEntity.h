#pragma once
#include "AbsEntityPrototype.h"
#include "submodules/UNAQtCommons/widgets/ControlsWidgets/abs_control.h"



class InputControlEntity : public AbsRecEntity
{
public:

	QString name;
	abs_control::ControlTypes type;
	QString defaultValue;
	InputControlEntity(QString nm = QString(), QString Ctype = QString(), QString dv = QString());
	InputControlEntity(QString nm, int Ctype, QString dv);
	static abs_control::ControlTypes intToType(const int cval);
	static abs_control::ControlTypes guessType(const QString&);
	static QString foldType(const int);
protected:
	// Inherited via AbsRecEntity
	virtual bool fromUniXml(const UniformXmlObject& o) override;
	virtual QString makeTitle() const override;
	virtual QString extractId() const override;
	virtual bool deepCompare(const AbsRecEntity* another) const override;
	virtual AbsRecEntity* fabricate() const override;
	virtual bool sortingCompare(const AbsRecEntity* another) const override;
	virtual bool sortingCompare(const QSharedPointer<AbsRecEntity> another) const override;
	virtual double extractEnumerable() const override;
};
typedef QSharedPointer<InputControlEntity> InputControl;
typedef QVector<InputControl> IControlList;
