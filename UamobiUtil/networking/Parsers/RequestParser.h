#pragma once
#include <QtCore/QString>
#include <QtCore/QHash>
#include "networking/things.h"
#include "abs_parsed_request.h"
#include <QtCore/QVector>

/*
		This file contains parsers that are concentrated on already parsed results, which are stored in
		uniform container. main idea of this uniform container is to allow user to define which parser will be used for primary
		parsing and how to parse results, i.e. there is no hard binding to representation, raw results can be processed
		without these functions. Main goal of these functions is to transform result into particular form.

		Ideal usage:
		You use interprets when you are sure what result is here
		You use makeParser when you not sure what result came
		You manually create parser and then use it's result when you don't want to waste time on deducing
		You check packet with simpliestresponse parser to be sure this IS a right packet and then use it's
		string representation to parse it on your own.

*/

namespace parse_uniresults_functions
{
	enum reqtypes { None, login, login_R, modes, places, positional, simpliest, suppliers, orders };

	struct UserProfilesResult
	{
		QVector<UserProfile> profiles;
		bool manually;
	};
	struct TypicalResponse
	{
		QString errors;
		bool resp;
	};
	struct PositionalResponse
	{
		QHash<QString, QString> values;
		bool success;
	};
	struct PairedResponse
	{
		QString primaryResult;
		QString secondaryResult;
	};
	template <class C>
	struct PagedResponse
	{
		QString from;
		QString to;
		bool last;
		QVector<C> values;
	};
	struct CombinedNamedLinearResponse
	{
		QHash<QString, QString> named;
		QVector<QPair<QString, QString> > linear;
	};
	typedef QVector<parsedMode> modesResponse;
	typedef QVector<parsedPlace> placesResponse;
	typedef QVector<parsedSupplier> supplierResponse;
	typedef QVector<parsedOrder> ordersResponse;
	typedef PagedResponse<parsedItemSimplified> searchResponse;
	typedef PagedResponse<parsedItem> doclistResponse;
	typedef QVector<parsedDocument> documentsResponse;
	typedef QVector<parsedDocType> docFilterResponse;

	UserProfilesResult parse_user_profiles(uniform_parse_result& ures);
	modesResponse parse_modes(uniform_parse_result& ures);
	placesResponse parse_places(uniform_parse_result& ures);
	QHash<QString, QString> parse_positional_responses(uniform_parse_result& ures);
	supplierResponse parse_suppliers(uniform_parse_result& ures);
	ordersResponse parse_orders_list(uniform_parse_result& ures);
	TypicalResponse parse_richtext(uniform_parse_result& ures);
	Document parse_document_response(uniform_parse_result& ures);
	CombinedNamedLinearResponse parse_item_info(uniform_parse_result& ures);
	searchResponse parse_search_response(uniform_parse_result& ures);
	doclistResponse parse_document_listed(uniform_parse_result& ures);
	documentsResponse parse_documents(uniform_parse_result& ures);
	docFilterResponse parse_filter_list(uniform_parse_result& ures);

	// deprecated, should make better
	bool isSimpliest(QString& res);
	bool isUserProfiles(QString& res);
	bool isLoginResponse(QString& res);
	bool isPositionalResponse(QString& res);
	bool isPlaceList(QString& res);
	bool isModeList(QString& res);
	bool isSuppliersList(QString& res);
	bool isOrdersList(QString& res);
}

namespace RequestParser
{
	using namespace parse_uniresults_functions;
	reqtypes deduceRequestType(QString& res);
	abs_parsed_request* makeParser(QString& res, QString& errtext, reqtypes type = None);
	UserProfilesResult interpretAsLogin(QString& res, QString& errtext);
	TypicalResponse interpretAsLoginResponse(QString& res, QString& errtext);
	TypicalResponse interpretAsSimpliestResponse(QString& res, QString& errtext);
	modesResponse interpretAsModeList(QString& res, QString& errtext);
	placesResponse interpretAsPlaceList(QString& res, QString& errtext);
	PositionalResponse interpretAsPositionalResponse(QString& res, QString& errtext);
	supplierResponse interpretAsSupplierList(QString& res, QString& errtext);
	ordersResponse interpretAsOrdersList(QString& res, QString& errtext);
	TypicalResponse interpretAsRichtextResponse(QString& res, QString& errtext);
	Document interpretAsDocumentResponse(QString& res, QString& errtext);
	CombinedNamedLinearResponse interpretAsItemInfo(QString& res, QString& errtext);
	searchResponse interpretAsSearchResponse(QString& res, QString& errtext);
	doclistResponse interpretAsListedDocument(QString& res, QString& errtext);
	documentsResponse interpretAsDocumentsList(QString& res, QString& errtext);
	docFilterResponse interpretAsDocFilterList(QString& res, QString& errtext);
};
namespace interpretsPointers
{
	using parse_uniresults_functions::placesResponse;
	using parse_uniresults_functions::supplierResponse;
	using parse_uniresults_functions::ordersResponse;
	using parse_uniresults_functions::PositionalResponse;
	using parse_uniresults_functions::TypicalResponse;

	typedef placesResponse(*interpretAsPlaceLike)(QString&, QString&);
	typedef supplierResponse(*interpretAsSupplierLike)(QString&, QString&);
	typedef ordersResponse(*interpretAsOrdersLike)(QString&, QString&);
	typedef PositionalResponse(*interpretAsPositionalLike)(QString&, QString&);
	typedef TypicalResponse(*interpretAsTypicalLike)(QString&, QString&);
}
