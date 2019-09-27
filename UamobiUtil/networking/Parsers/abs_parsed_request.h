#pragma once
#include <QtCore/QStringList>


/*
		This file contains abstract parent for all request parsers. main idea of this object family is to make fully abstract parsing 
		mechanism to make parsing simple. All children of this class are NOT universal - they are assuming that you already
		deducted request type. Deducing is placed in RequestParser. 
			Idea of this object is pretending that all parsed request has same result type, so that you can process them in any way you want.
		In final realization, fully wrapped, you can call appropriate method of RequestParser to get fully formated values or to
		call deduction and then format the data.

*/

// Type of parsing, usually it helps to conversion functions to understand, does this result hold any value they can use
enum request_parse_type {login, mode, query_result, linear_result, positional_result};



struct uniform_parse_result  // This is highly abstract result of parsing. You can wrap it with non-abstract RequestParser to obtain formatted values
	//or use it in raw view
{
	request_parse_type type;		//	type of request
	QStringList queriesResult;		//	values in heap. To select them you must use one_position_entries
	int request_status;			//	status of request. 200 - ok
	int one_position_entries_quantity;	//	maximum of values per one entry. Always use this value as MINIMUM border. Example: if in one notation <mode> holds 1 value and 
	// in other - 2, you must always count 2 to get next <mode> entry
	int alternative_result;		//	alternative result is kind of non-error result making change target widget. Like <manually> in login.
	uniform_parse_result(request_parse_type = linear_result);
};

bool queryLengthOkInResult(uniform_parse_result& ures);
int queryReservationSize(uniform_parse_result& ures);



class abs_parsed_request
{
protected:
	QString result;			///	Raw result data
	QString errtext;		//	raw error log
	bool success;			//	defines if parser was successfull.
	uniform_parse_result parseres;	//	result to return
public:
	abs_parsed_request(QString& res, QString &err);	//	Parsing happens in constructor
	bool isSuccessfull();		//	wraps couldRead && noRequestErrors
	QString getErrors();		//	wraps virtual parseErrorText
	uniform_parse_result read();	//	returns parsed result
private:
	virtual bool couldRead() =0;		//	overload to provide value for isSuccesfull. This should return false in case when you found error in packet
	virtual bool noRequestErrors() = 0;		//	overload to provide value for isSuccesfull. Request is succesfull when both values are good
	virtual QString parseErrorText() = 0;	//	overload to provide text of errors and for request errors, and for parsing errors
};
