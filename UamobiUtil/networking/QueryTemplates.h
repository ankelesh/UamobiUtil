#pragma once
#include <QHash>

namespace QueryTemplates
{
	enum QueryId
	{
		ping,						// no arguments required 
		userList,					// 1 arguments required 
		Login,						// 3 arguments required 
		LogOut,						// 0 arguments required 
		modeList,					// 1 arguments required 
		selectMode,					// 2 arguments required 
		placeList,					// 0 arguments required 
		selectPlace,				// 1 arguments required 
		lockDocument,				// 1 arguments required 
		unlockDocument,				// 1 arguments required 
		documentGetAllowedTypes,	// 0 arguments required 
		documentSetFilter,			// 1 arguments required 
		documentGetAllowedToCreate,	// 0 arguments required 
		documentNewParent,			// 1 arguments required 
		documentSearchItems,		// 2 arguments required 
		getItemInfo,				// 2 arguments required 
		documentGetResults,			// 1 arguments required 
		documentResultGetBox,		// 1 arguments required 
		docResultEditByBoxId,		// 4 arguments required 
		docResultDeleteItem,		// 1 arguments required 
		docUpdateParameters,		// 4 arguments required 
		docListSubdocuments,		// 0 arguments required 
		inventoryListParentDocs,	// 0 arguments required 
		listPhotoItems,				// 0 arguments required 
		inventoryNewDocument,		// 3 arguments required 
		inventoryAddItem,			// 3 arguments required 
		invAddItemExpDate,			// 2 arguments required 
		receiptListSuppliers,		// 2 arguments required 
		warehouseList,				// 2 arguments required
		receiptListOrders,			// 0 arguments required 
		receiptGetOrderInfo,		// 2 arguments required 
		receiptNewDocument,			// 3 arguments required 
		receiptAddItem,				// 2 arguments required 
		receiptAddItemExpDate,		// 2 arguments required 
		getFilterItem,				// 1 arguments required 
		documentDeleteAll,			// 0 arguments required 
		docDeleteByBarcode,			// 2 arguments required 
		recListTemplated,			// 1 arguments required 
		applyBarcodeFilter,			// 2 arguments required 
		receiptAddItemExpanded,		// 3 arguments required 
	};

	QHash<QueryId, QString> _initCache();
	int getQueryArguments(const QueryId);
	bool assertArgQuantity(int argc, QueryId id);
	extern QHash<QueryId, QString> queryCache;
	class OverloadableQuery
	{
	private:
		QueryId expectedQuery;
		QString overloadedQuery;
		int argumentListLength;
		QVector<int> emplaceValues;
		QStringList valuesForUnpacking;
		
		inline int _argNameToInt(const QString arg) const;
		inline const QString& assertRightQuery() const;
	public:
		explicit OverloadableQuery(QueryId id);
		explicit OverloadableQuery(QString oq);
		explicit OverloadableQuery(int argListLength, QueryId queryId);
		explicit OverloadableQuery(QStringList allValues, QStringList unpackedValues);
		OverloadableQuery(QueryId id, QStringList allValues, QStringList unpackedValues);
		OverloadableQuery(QString oq, QStringList allValues, 
			QStringList unpackedValues);
		OverloadableQuery(QString oq, QStringList unpacked);
		OverloadableQuery(QStringRef oq, QVector<QStringRef>& unpacked);

		QString& arg(QString& whereTo,QString& toEmplaceName, QString& toEmplaceValue) const;
		void setExpected(QueryId);
		void setFieldMapping(QStringList normal, QStringList unpacked);
		void setAllArgsFilter(QStringList allArgs);
		bool assertArgQuantity(int argc) const;
		bool isDefault() const;
		bool requiresAllArgs() const;
		QueryId getQID() const { return expectedQuery; };
		static const OverloadableQuery& defaultQuery();
		QString filterAndApply(QStringList arguments, QString ssid) const;
		
	};
}