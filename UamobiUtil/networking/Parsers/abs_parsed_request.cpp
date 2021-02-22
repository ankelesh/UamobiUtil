#include "abs_parsed_request.h"
#include <QtCore/QString>
#include <QtCore/QDataStream>
#include "submodules/UNAQtCommons/widgets/UtilityElements/ExtendedDialogs.h"
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
#ifndef QStringLiteral
#define QStringLiteral(A) QString::fromLatin1(A, sizeof(A)-1)
#endif

QPair<QString, QString> makeError(QDomDocument& xmldoc)
{
	QDomNodeList tlist = xmldoc.elementsByTagName("status");
	QString errtext;
	if (tlist.count() < 1)
	{
		return qMakePair(QStringLiteral("No status code"), QString());
	}
	else
	{
		int code = tlist.at(0).toElement().text().toInt();
		switch (code)
		{
		case 200:
			errtext = "possibly fake error, check logs";
			break;
		case 403:
			errtext = "Forbidden request";
			break;
		case 404:
			errtext = "Data not exists";
			break;
		case 500:
			errtext = "Internal error";
			break;
		default:
			errtext = "Unknown error code: "
				+ tlist.at(0).toElement().text();
			break;
		}
	}
	tlist = xmldoc.elementsByTagName("message");
	if (tlist.count() > 0)
		errtext = tlist.at(0).toElement().text();
	tlist = xmldoc.elementsByTagName("stack");
	if (tlist.count() > 0)
		return qMakePair(errtext, tlist.at(0).toElement().text());
	return qMakePair(errtext,QString());
}
bool isError(QDomDocument& xmldoc)
{
	QDomNodeList tlist = xmldoc.elementsByTagName("status");
	if (tlist.count() < 1)
	{
		return true;
	}
	if (tlist.at(0).toElement().text().toInt() != 200)
	{
		return true;
	}
	return false;
}

bool assertAndShowError(QWidget* parent, ResponseParser p, QString header, bool extraConditions, QString extraMsg, QString extraStack)
{
	if (p.isNull())
		return false;
	if (!p->isSuccessfull() && extraConditions)
	{
#ifdef DEBUG
		detrace_NRESPERR(p->getErrors());
#endif
		if (extraMsg.isEmpty())
		{
			ErrorMessageDialog::showErrorInfo(header, p->getErrors(), false, p->getStack());
		}
		else
		{
			ErrorMessageDialog::showErrorInfo(header, extraMsg, false, (extraStack.isEmpty())?  p->getStack(): extraStack);
		}
		return true;
	}
	return false;
}

bool AbsResponseParser::_stopWithError(const char* estr)
{
	xmldoc.clear();
	errtext += estr;
	return false;
}

bool AbsResponseParser::run()
{
	return _doParsing();
}

AbsResponseParser::AbsResponseParser(QString& res, QString& err)
	: xmldoc(), errtext(err), success(false), parseResult(), alternativeResult(0)
{
	if (res.isEmpty())
	{
		errtext = "Empty result!";
		return;
	}
	xmldoc.setContent(res);
	if (isError(xmldoc))
	{
		QPair<QString, QString> res = makeError(xmldoc);
		errtext = res.first;
		errstack = res.second;
	}	
}

bool AbsResponseParser::isSuccessfull()
{
	return success;
}

QString AbsResponseParser::getErrors()
{
	return errtext;
}

QString AbsResponseParser::getStack()
{
	return errstack;
}

int AbsResponseParser::isAlternative()
{
	return alternativeResult;
}

void AbsResponseParser::reset(QString& res, QString& err)
{
	xmldoc.setContent(res);
	errtext = err;
	if (isError(xmldoc))
	{
		QPair<QString, QString> pres = makeError(xmldoc);
		errtext += pres.first;
		errstack += pres.second;
	}
	else
	{
		success = _doParsing();
	}
}

XmlObjects& AbsResponseParser::read()
{
    return parseResult;
}

AbsResponseParser::~AbsResponseParser()
{

}
