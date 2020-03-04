#pragma once
#include <QtCore/QStringList>
#include "datacore/UniformXmlObject.h"
#include <QVector>

/*
		This file contains parent class for all parsers required in application. It 
		checks object for errors and initializes xml document object.
*/


// extracts error from document. Usually it is described with <message><stack> tags
QString makeError(QDomDocument&);
// checks if document contains common error descriptions via status codes
bool isError(QDomDocument&);

class AbsResponseParser
{
protected:
	// document used to extract data
	QDomDocument xmldoc;
	QString errtext;		//	raw error log
	bool success;			//	defines if parser was successfull.
	// uniform objects represent data like json ones
	XmlObjects parseResult;
	// these results are used when parser is not interrupted 
	int alternativeResult;

	// common interface for parsing
	virtual bool _doParsing() = 0;
	// simple stop with inserting message into errtext
	bool _stopWithError(const char *);
	// deprecated - starts parser
	bool run();
public:
	AbsResponseParser(QString& res, QString& err);	//	Parsing happens in constructor
	bool isSuccessfull();		//	wraps couldRead && noRequestErrors
	QString getErrors();		//	returns errtext
	int isAlternative();        // returns altres
	void reset(QString& res, QString& err);  // postconstructor
	XmlObjects& read();	//	returns parsed result
    virtual ~AbsResponseParser();
};

typedef QSharedPointer<AbsResponseParser> ResponseParser;
