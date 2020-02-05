#pragma once
#include <QtCore/QString>
#include <QtCore/QHash>
#include "networking/things.h"
#include "abs_parsed_request.h"
#include <QtCore/QVector>
#include "datacore/AbsEntityPrototype.h"
#include "DynamicLinearParsers.h"
#include "SimpleLinearParsers.h"
#include "SimpleResponseParsers.h"
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


namespace RequestParser
{
	template <class NetResp>
	NetRequestResponse<NetResp> parseResponse(QSharedPointer<AbsResponseParser>& parser)
	{
		
		if (parser->isSuccessfull())
		{
			QSharedPointer<NetResp> prototype(new NetResp());
			XmlObjects result = parser->read();
			NetRequestResponse<NetResp> finalRes;
			finalRes.fromHeterogenicXmlObjects(result, prototype, parser->isAlternative());
			return finalRes;
		}
		else
			return NetRequestResponse<NetResp>(parser->getErrors());
	};
	NetRequestResponse<AbsRecEntity> parseResponse(QSharedPointer<AbsResponseParser>& parser, RecEntity prototype);
}


