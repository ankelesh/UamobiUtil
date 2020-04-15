#pragma once
#include <QtCore/QStringList>
#include <QtCore/QDate>
#include <QtCore/QObject>
#include "datacore/NamedIdEntity.h"
#include "datacore/ModeEntity.h"
#include "datacore/EntitledEntity.h"
#include "datacore/FullItemEntity.h"
#include "datacore/FullDocumentEntity.h"
#include "datacore/InputControlEntity.h"
#include <QtCore/qabstractitemmodel.h>
/*
	this file includes all entities used in application and common data model for representation
	polymorthic lists. If you dont need all this - use entities home files.

*/

class DataEntityListModel : public QAbstractListModel
	// This data model is used for fully dynamical data entity model. Use right delegate to show data in view.
	// You can place in this model even polymorthic data, just install suitable delegate for it.
{
	Q_OBJECT
protected:
	// real data storage
	Records innerList;
	QVector<int> heights;
public:
	// Roles for data entity operations
	enum ExtendedRoles {
		// Search role returns polymorthic pointer to call ->compare
		SearchRole = Qt::UserRole + 1,
		// Quantity view is used to return quantity of items. Not used in this model
		QuantityView,
		// DataCopyRole returns full copy of an object to avoid changing model
		DataCopyRole,
		DirectAccess,
		HeightAccess
	};

    DataEntityListModel(QObject* parent) : QAbstractListModel(parent), innerList(), heights(){}
	DataEntityListModel(const Records& data, QObject* parent = Q_NULLPTR);
	// Inherited from QAbstractListModel
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	// dataoperation methods
	//post-constructor
    void insertData(const Records& data);
	void insertData(const Records& data, const QVector<int>& heights);
	// deletes data entity in the model by it's index or pointer
	void removeDataEntity(const QModelIndex&);
	void removeDataEntity(RecEntity);
	// replaces data entity by it's id. This method is used only if you have a copy with the same id and other data.
	void replaceDataEntity(RecEntity);
	// empties model
	void reset();
public slots:
	//If index was valid - emits signal with pointer to clicked entity.
#ifdef QT_VERSION5X
    virtual void mapClickToEntity(const QModelIndex& index);
#else
    virtual void mapClickToEntity(QModelIndex index);
#endif
	void lookForEntity(const RecEntity);
signals:
	// delivers pointer to data entity. Warning - changing this entity by pointer will affect model.
	void dataEntityClicked(RecEntity);
};
