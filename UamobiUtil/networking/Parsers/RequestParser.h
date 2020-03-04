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
		this file contains not parsers, but interpretation functions, which are conver

*/


namespace RequestParser
{
	template <class NetResp>
    NetRequestResponse<NetResp> parseResponse(QSharedPointer<AbsResponseParser> parser)
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
	template<>
    inline NetRequestResponse<AbsRecEntity> parseResponse<AbsRecEntity>(QSharedPointer<AbsResponseParser> parser)
	{
		return PolyResponse(parser->getErrors() + " USING WRONG PARSING TEMPLATE!!!!!!");
	}
    NetRequestResponse<AbsRecEntity> parseResponse(QSharedPointer<AbsResponseParser> parser, RecEntity prototype);
	PolyResponse parseResponse(PolyResponse resp, RecEntity prototype);
}


