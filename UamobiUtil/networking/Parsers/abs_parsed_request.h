#pragma once
#include <QtCore/QStringList>
#include "datacore/UniformXmlObject.h"

/*
		This file contains abstract parent for all request parsers. main idea of this object family is to make fully abstract parsing
		mechanism to make parsing simple. All children of this class are NOT universal - they are assuming that you already
		deducted request type. Deducing is placed in RequestParser.
			Idea of this object is pretending that all parsed request has same result type, so that you can process them in any way you want.
		In final realization, fully wrapped, you can call appropriate method of RequestParser to get fully formated values or to
		call deduction and then format the data.

*/



QString makeError(QDomDocument&);
bool isError(QDomDocument&);

class AbsResponseParser
{
protected:
	QDomDocument xmldoc;
	QString errtext;		//	raw error log
	bool success;			//	defines if parser was successfull.
	XmlObjects parseResult;
	int alternativeResult;

	virtual bool _doParsing() = 0;
	bool _stopWithError(const char *);
	bool run();
public:
	AbsResponseParser(QString& res, QString& err);	//	Parsing happens in constructor
	bool isSuccessfull();		//	wraps couldRead && noRequestErrors
	QString getErrors();		//	wraps virtual parseErrorText
	int isAlternative();
	void reset(QString& res, QString& err);
	XmlObjects& read();	//	returns parsed result
};

typedef QSharedPointer<AbsResponseParser> ResponseParser;