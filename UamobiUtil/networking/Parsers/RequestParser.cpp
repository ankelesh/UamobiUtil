#include "RequestParser.h"
#include "networking/Parsers/SimpleLinearParsers.h"
#include "networking/Parsers/SimpleResponseParsers.h"
#include "networking/Parsers/DynamicLinearParsers.h"
#include "networking/Parsers/NamedAttributesResponseParsers.h"
#include "legacy/legacy.h"
#include <QtXml/QDomDocument>
//#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

namespace parse_uniresults_functions {
	UserProfilesResult parse_uniresults_functions::parse_user_profiles(uniform_parse_result& ures)
	{
		UserProfilesResult temp;
		if (ures.alternative_result == 1)
		{
#ifdef DEBUG
			//detrace_METHEXPL("manually found");
#endif

			temp.manually = true;
			return temp;
		}
		else if (ures.one_position_entries_quantity == 2 && ures.queriesResult.count() > 0)
		{
			temp.manually = false;
			if (ures.queriesResult.count() % ures.one_position_entries_quantity == 0) {
				UserProfile up;
				temp.profiles.reserve(ures.queriesResult.count() / ures.one_position_entries_quantity);
				for (int i = 0; i < ures.queriesResult.count() / ures.one_position_entries_quantity; ++i)
				{
					up.name = ures.queriesResult.at(i * ures.one_position_entries_quantity);
					up.login = ures.queriesResult.at(i * ures.one_position_entries_quantity + 1);
					temp.profiles.push_back(up);
#ifdef DEBUG
					//detrace_CYCLEEXPL("profile obtained: " << up.login << up.name);
#endif
				}
				return temp;
			}
		}
		return UserProfilesResult();
	}

	modesResponse parse_uniresults_functions::parse_modes(uniform_parse_result& ures)
	{
#ifdef DEBUG
		//detrace_METHEXPL(showHeap(ures));
#endif
		if (!queryLengthOkInResult(ures))
		{
			return modesResponse();
		}
		modesResponse temp;
		parsedMode pmode;
		QList<QString>::iterator start = ures.queriesResult.begin();
		if (ures.one_position_entries_quantity == 1)
		{
#ifdef DEBUG
			//detrace_METHEXPL("modes->single");
#endif
			while (start != ures.queriesResult.end())
			{
				parsedMode tpm;
				tpm.name = *start;
				tpm.mode = *start;
				tpm.submode = "";
				temp.push_back(tpm);
				++start;
			}
		}
		else
		{
#ifdef DEBUG
			//detrace_METHEXPL("modes->multi");
#endif
			while (start != ures.queriesResult.end())
			{
				parsedMode m;
				m.mode = *start;
				++start;
				m.name = *start;
				++start;
				if (m.name.isEmpty())
					m.name = m.mode;
#ifdef DEBUG
				//detrace_CYCLEEXPL("inserted: " << m.mode);
#endif
				temp.push_back(legacy::parseLegacyMode(m.name, m.mode));
			}
		}
		return temp;
	}

	placesResponse parse_uniresults_functions::parse_places(uniform_parse_result& ures)
	{
		if (!queryLengthOkInResult(ures))
		{
			return placesResponse();
		}
		placesResponse temp;
		temp.reserve(queryReservationSize(ures));
		parsedPlace pplace;
		QList<QString>::iterator start = ures.queriesResult.begin();
		while (start != ures.queriesResult.end())
		{
			pplace.code = *start;
			++start;
			if (start != ures.queriesResult.end())
			{
				pplace.name = *start;
				++start;
				temp.push_back(pplace);
			}
		}
		return temp;
	}

	QHash<QString, QString> parse_positional_responses(uniform_parse_result& ures)
	{
		QHash<QString, QString> temp;
		if (queryLengthOkInResult(ures))
		{
			for (int i = 0; i < ures.queriesResult.count(); i += 2)
			{
				temp[ures.queriesResult.at(i)] = ures.queriesResult.at(i + 1);
			}
		}
		return temp;
	}

	supplierResponse parse_suppliers(uniform_parse_result& ures)
	{
		supplierResponse temp;
		parsedSupplier psup;
		if (queryLengthOkInResult(ures))
		{
			temp.reserve(queryReservationSize(ures));
			for (int i = 0; i < ures.queriesResult.count(); i += 3)
			{
				psup.code = ures.queriesResult[i];
				psup.name = ures.queriesResult[i + 1];
				psup.orders = ures.queriesResult[i + 2];
				temp.push_back(psup);
			}
		}
		return temp;
	}

	ordersResponse parse_orders_list(uniform_parse_result& ures)
	{
		ordersResponse temp;
		parsedOrder pord;
		if (queryLengthOkInResult(ures))
		{
			temp.reserve(queryReservationSize(ures));
			for (int i = 0; i < ures.queriesResult.count(); i += 3)
			{
				pord.code = ures.queriesResult[i];
				pord.title = ures.queriesResult[i + 1];
				pord.text = ures.queriesResult[i + 2];
				temp.push_back(pord);
			}
		}
		return temp;
	}

	TypicalResponse parse_richtext(uniform_parse_result& ures)
	{
		TypicalResponse temp;
		if (ures.queriesResult.count() == 1)
		{
#ifdef DEBUG
				detrace_METHEXPL("richtext parsed" << showHeap(ures));
#endif
			temp.errors = ures.queriesResult.at(0);
			temp.resp = true;
		}
		else
		{
#ifdef DEBUG
			detrace_METHEXPL("richtext not parsed: q" << ures.one_position_entries_quantity );
#endif
			temp.resp = false;
		}
		return temp;
	}

	Document parse_document_response(uniform_parse_result& ures)
	{
		Document temp;
		if (queryLengthOkInResult(ures))
		{
			QHash<QString, QString> temphash;
			for (int i = 0; i < ures.queriesResult.count(); i += 2)
			{
				temphash[ures.queriesResult.at(i)] = ures.queriesResult.at(i + 1);
			}
			temp.docId = temphash["code"];
			temp.comment = temphash["comment"];
			temp.dateStr = temphash["date"];
			temp.parentNr = temphash["parentnr"];
			temp.supplier = temphash["supplier"];
			temp.cancelled = (temphash["canceled"] == "true");
			temp.closed = (temphash["closed"] == "true");
			temp.locked = (temphash["locked"] == "true");
			return temp;
		}
		return Document();
	}
	CombinedNamedLinearResponse parse_item_info(uniform_parse_result& ures)
	{
		CombinedNamedLinearResponse temp;
#ifdef DEBUG
		detrace_METHEXPL(showHeap(ures));
#endif
		if (((ures.queriesResult.count()%3) == 0))
		{
			for (int i = 0; i < ures.queriesResult.count(); i+= 3)
			{
				if (ures.queriesResult.at(i).isEmpty()) {
					temp.named.insert(ures.queriesResult.at(i+1), ures.queriesResult.at(i + 2));
				}
				else
				{
					temp.linear.push_back(QPair<QString, QString>(ures.queriesResult.at(i + 1), ures.queriesResult.at(i + 2)));
				}
			}
		}
		return temp;
	}

	searchResponse parse_search_response(uniform_parse_result& ures)
	{
		searchResponse temp;
		if (ures.one_position_entries_quantity == 2 && ures.alternative_result == 3)
		{
			temp.from = ures.queriesResult.at(0);
			temp.to = ures.queriesResult.at(1);
			temp.last = ((ures.queriesResult.at(2) == "true") ? true : false);
			temp.values.reserve((ures.queriesResult.count() - 3)/2);
			for (int i = ures.alternative_result; i < ures.queriesResult.count(); i += ures.one_position_entries_quantity)
			{
				temp.values.push_back(parsedItemSimplified
				(   ures.queriesResult.at(i),
					ures.queriesResult.at(i+1)
				));
			}
		}
		return temp;
	}

	doclistResponse parse_document_listed(uniform_parse_result& ures)
	{
		doclistResponse temp;
		if (ures.one_position_entries_quantity == 6 && ures.alternative_result == 3)
		{
			temp.from = ures.queriesResult.at(0);
			temp.to = ures.queriesResult.at(1);
			temp.last = ((ures.queriesResult.at(2) == "true") ? true : false);
			temp.values.reserve((ures.queriesResult.count() - 3) / 6);
			for (int i = ures.alternative_result; i < ures.queriesResult.count(); i += ures.one_position_entries_quantity)
			{
				temp.values.push_back(parsedItem(
					ures.queriesResult.at(i),
					ures.queriesResult.at(i+1),
					ures.queriesResult.at(i+2),
					ures.queriesResult.at(i+3),
					ures.queriesResult.at(i+4),
					ures.queriesResult.at(i+5)
				));
			}
		}
		return temp;
	}

#define DEBUG
	documentsResponse  parse_documents(uniform_parse_result& ures)
	{
		documentsResponse temp;
#ifdef DEBUG
		detrace_METHEXPL(showHeap(ures));
#endif
		if (queryLengthOkInResult(ures))
		{
			temp.reserve(queryReservationSize(ures));
			QList<QString>::iterator start = ures.queriesResult.begin();
			while (start != ures.queriesResult.end())
			{
				temp.push_back(parsedDocument(*start, *(start + 1), *(start + 2), *(start + 3)));
				start += 4;
			}
			return temp;
		}
		return temp;
	}

	docFilterResponse parse_filter_list(uniform_parse_result& ures)
	{
		docFilterResponse temp;
		if (queryLengthOkInResult(ures))
		{
			temp.reserve(queryReservationSize(ures));
			QList<QString>::iterator start = ures.queriesResult.begin();
			while (start != ures.queriesResult.end())
			{
				temp.push_back(parsedDocType(*start, *(start + 1), *(start + 2)));
				start += ures.one_position_entries_quantity;
			}
		}
		return temp;
	}

	bool isSimpliest(QString& res)
	{
		if (res.contains("<status>"))
			return true;
		return false;
	}

	bool parse_uniresults_functions::isUserProfiles(QString& res)
	{
		if (res.contains("<users>"))
		{
			return true;
		}
		else if (res.contains("<user>"))
		{
			return true;
		}
		return false;
	}

	bool parse_uniresults_functions::isLoginResponse(QString& res)
	{
		if (res.contains("<login>"))
			return true;
		return false;
	}

	bool isPositionalResponse(QString& res)
	{
		if (res.contains("<s>"))
			return true;
		return false;
	}

	bool parse_uniresults_functions::isPlaceList(QString& res)
	{
		if (res.contains("<place>"))
			return true;
		return false;
	}

	bool parse_uniresults_functions::isModeList(QString& res)
	{
		if (res.contains("<mode>"))
			return true;
		return false;
	}
	bool isSuppliersList(QString& res)
	{
		if (res.contains("<supplier>"))
			return true;
		return false;
	}
	bool isOrdersList(QString& res)
	{
		if (res.contains("<order>"))
			return true;
		return false;
	}
}
namespace RequestParser {
	reqtypes RequestParser::deduceRequestType(QString& res)
	{
		if (isUserProfiles(res))
			return reqtypes::login;
		if (isLoginResponse(res))
			return reqtypes::login_R;
		if (isModeList(res))
			return reqtypes::modes;
		if (isPlaceList(res))
			return reqtypes::places;
		if (isPositionalResponse(res))
			return reqtypes::positional;
		if (isSuppliersList(res))
			return reqtypes::suppliers;
		if (isOrdersList(res))
			return reqtypes::orders;
		// this must be last because it treats packet as "all ok" response string. This string contains in ANY successfull response.
		if (isPositionalResponse(res))
			return reqtypes::simpliest;
		return reqtypes::None;
	}

	abs_parsed_request* RequestParser::makeParser(QString& res, QString& errtext, reqtypes type)
	{
		if (type == None)
		{
			type = deduceRequestType(res);
		}
		switch (type)
		{
		case reqtypes::login:
			return new UserListParser(res, errtext);
		case reqtypes::login_R:
			return new LoginResponseParser(res, errtext);
		case reqtypes::modes:
			return new ModeListParser(res, errtext);
		case reqtypes::places:
			return new PlacesListParser(res, errtext);
		case reqtypes::suppliers:
			return new SuppliersListParser(res, errtext);
		default:
			return Q_NULLPTR;
		}
	}

	UserProfilesResult RequestParser::interpretAsLogin(QString& res, QString& errtext)
	{
		UserListParser parser(res, errtext);
		if (parser.isSuccessfull())
		{
#ifdef DEBUG
			//detrace_METHEXPL("Parsing succesfull");
#endif
			return parse_user_profiles(parser.read());
		}
		else
		{
#ifdef DEBUG
			//detrace_METHEXPL("parsing failed");
#endif
			UserProfilesResult temp;
			temp.manually = true;
			return temp;
		}
	}

	TypicalResponse RequestParser::interpretAsLoginResponse(QString& res, QString& errtext)
	{
		TypicalResponse resp;
		LoginResponseParser parser(res, errtext);
		if (parser.isSuccessfull())
		{
			uniform_parse_result AfterParsing = parser.read();
			resp.resp = AfterParsing.request_status == 200;
			if (resp.resp == false)
			{
				resp.errors = AfterParsing.queriesResult.join(" ");
			}
		}
		return resp;
	}

	TypicalResponse interpretAsSimpliestResponse(QString& res, QString& errtext)
	{
		TypicalResponse resp;
		SimpliestResponceParser parser(res, errtext);
		if (parser.isSuccessfull())
		{
			resp.resp = true;
		}
		else
		{
			resp.resp = false;
			resp.errors = parser.getErrors();
		}
		return resp;
	}

	modesResponse RequestParser::interpretAsModeList(QString& res, QString& errtext)
	{
		ModeListParser parser(res, errtext);
		if (parser.isSuccessfull())
		{
			return parse_modes(parser.read());
		}
		else
		{
			return modesResponse();
		}
	}

	placesResponse RequestParser::interpretAsPlaceList(QString& res, QString& errtext)
	{
		PlacesListParser parser(res, errtext);
		if (parser.isSuccessfull())
		{
			return parse_places(parser.read());
		}
		return placesResponse();
	}
	PositionalResponse interpretAsPositionalResponse(QString& res, QString& errtext)
	{
		PositionalResponse temp;
		ModeSelectResponseParser parser(res, errtext);
		detrace_METHEXPL(showHeap(parser.read()));
		if (parser.isSuccessfull())
		{
			temp.success = true;
			temp.values = parse_positional_responses(parser.read());
		}
		else
		{
			temp.success = false;
		}
		return temp;
	}
	supplierResponse interpretAsSupplierList(QString& res, QString& errtext)
	{
		SuppliersListParser parser(res, errtext);
		if (parser.isSuccessfull())
		{
			return parse_suppliers(parser.read());
		}
		return supplierResponse();
	}
	ordersResponse interpretAsOrdersList(QString& res, QString& errtext)
	{
		OrdersListParser parser(res, errtext);
		if (parser.isSuccessfull())
		{
			return parse_orders_list(parser.read());
		}
		return ordersResponse();
	}
	TypicalResponse interpretAsRichtextResponse(QString& res, QString& errtext)
	{
		TypicalResponse resp;
		RichtextResponseParser parser(res, errtext);
		if (parser.isSuccessfull())
		{
			return parse_richtext(parser.read());
		}
		return TypicalResponse();
	}
	Document interpretAsDocumentResponse(QString& res, QString& errtext)
	{
		DocumentCreationResponseParser parser(res, errtext);
		if (parser.isSuccessfull())
		{
			return parse_document_response(parser.read());
		}
		return Document();
	}
	CombinedNamedLinearResponse interpretAsItemInfo(QString& res, QString& errtext)
	{
		ItemInfoResponseParser parser(res, errtext);
		if (parser.isSuccessfull())
		{
			return parse_item_info(parser.read());
		}
		return CombinedNamedLinearResponse();
	}
	searchResponse interpretAsSearchResponse(QString& res, QString& errtext)
	{
		ItemSimplifiedParser parser(res, errtext);
		if (parser.isSuccessfull())
		{
			return parse_search_response(parser.read());
		}
		return searchResponse();
	}
	doclistResponse interpretAsListedDocument(QString& res, QString& errtext)
	{
		ExpandedItemListParser parser(res, errtext);
		if (parser.isSuccessfull())
		{
			return parse_document_listed(parser.read());
		}
		return doclistResponse();
	}
	documentsResponse interpretAsDocumentsList(QString& res, QString& errtext)
	{
		DocumentParser parser(res, errtext);
		if (parser.isSuccessfull())
		{
			return parse_documents(parser.read());
		}
		return documentsResponse();
	}
	docFilterResponse interpretAsDocFilterList(QString& res, QString& errtext)
	{
		DocTypeFiltersParser parser(res, errtext);
		if (parser.isSuccessfull())
		{
			return parse_filter_list(parser.read());
		}
		return docFilterResponse();
	}
}