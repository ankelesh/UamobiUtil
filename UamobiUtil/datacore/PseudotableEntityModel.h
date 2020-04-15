#pragma once
#include "networking/things.h"


class PseudotableEntityModel : public DataEntityListModel
{
	Q_OBJECT
protected:
	int columns;

	inline int _flatIndex(int col, int row) const;
public:
	PseudotableEntityModel( int Columns = 2, QObject* parent = Q_NULLPTR) : DataEntityListModel(parent), columns(Columns) {}
	// Inherited from QAbstractListModel;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
#ifdef QT_VERSION5X
	virtual void mapClickToEntity(const QModelIndex& index) override;
#else
	virtual void mapClickToEntity(QModelIndex index) override;
#endif

};