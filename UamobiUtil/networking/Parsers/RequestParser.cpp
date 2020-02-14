#include "RequestParser.h"
#include "networking/Parsers/SimpleLinearParsers.h"
#include "networking/Parsers/SimpleResponseParsers.h"
#include "networking/Parsers/DynamicLinearParsers.h"
#include "legacy/legacy.h"
#include <QtXml/QDomDocument>
#define DEBUG
#ifdef DEBUG
#include "debugtrace.h"
#endif

namespace RequestParser
{
	NetRequestResponse<AbsRecEntity> parseResponse(QSharedPointer<AbsResponseParser>& parser, RecEntity prototype)
	{
		if (parser->isSuccessfull())
		{
			XmlObjects result = parser->read();
			NetRequestResponse<AbsRecEntity> finalRes;
			finalRes.fromHeterogenicXmlObjects(result, prototype, parser->isAlternative());
			return finalRes;
		}
		else
			return NetRequestResponse<AbsRecEntity>(parser->getErrors());
	}
}
