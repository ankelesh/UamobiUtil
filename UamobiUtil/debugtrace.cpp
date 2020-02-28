#include "debugtrace.h"
#include <QFile>
#include <QDir>
#include <cstdlib>
#include <QDateTime>


struct detracestream
{
	QFile ofile;
	QTextStream ostream;
	QString outstring;
	QTextStream sout;
	detracestream()
		: ofile(), ostream(), outstring(), sout() {}
	detracestream(const char* path)
		: ofile(path), ostream(&ofile), outstring("detrace:\r\n"),
		sout(&outstring) {
		ofile.open(QIODevice::WriteOnly | QIODevice::Text);
		if (!ofile.isOpen())
			throw std::exception("file not open!");
		ostream.setDevice(&ofile);
	};
	detracestream(const detracestream& s)
		: ofile(s.ofile.fileName()), ostream(&ofile), outstring(s.outstring), sout(&outstring)
	{
		ofile.open(QIODevice::WriteOnly | QIODevice::Text);
		if (!ofile.isOpen())
			throw std::exception("file not open!");
		ostream.setDevice(&ofile);
	}
	detracestream& operator=(const detracestream& other)
	{
		ofile.setFileName(other.ofile.fileName());
		ofile.open(QIODevice::WriteOnly | QIODevice::Text);
		if (!ofile.isOpen())
			throw std::exception("file not open!");
		ostream.setDevice(&ofile);
		outstring = other.outstring;
		sout.setString(&outstring);
		return *this;
	}
};




struct impl
{
	enum OutStreams { everything, error, netrequests };
	detracestream streams[3];
	DebugPriority priorityLvl; // main priority
	DebugPriority msgPriorityLvl; // priority of incoming message
	OutputMode omode; // mode of output, check enum
	void (debugtrace::* outmethod)(const QString& str); // pointer to current
	QVector<void(debugtrace::*)(const QString & str)> omode_united;
	QString qDebugFullMessageHolder; // string to avoid new lines in qDebug()
	bool notMixing;

	impl(DebugPriority priority, OutputMode mode,
		void (debugtrace::* ometh)(const QString& str), bool notMix)
		:
		streams(),
		priorityLvl(priority), msgPriorityLvl(detr_supply::notImportantMessage),
		omode(mode), outmethod(ometh), omode_united(), qDebugFullMessageHolder(), notMixing(notMix)
	{
		streams[0] = detracestream(FOUTPATH);
		streams[1] = detracestream(ERROPATH);
		streams[2] = detracestream(NETOPATH);

	}

};




void debugtrace::printToFile(const QString& str)
{
	if (isValid())
	{
		switch (pimpl->msgPriorityLvl)
		{
		case netrequestSent:
		case netresponseReceived:
			if (!pimpl->notMixing)
			{
				pimpl->streams[impl::everything].ostream << str.left(60);
				pimpl->streams[impl::everything].ostream.flush();
			}
			pimpl->streams[impl::netrequests].ostream << str;
			pimpl->streams[impl::netrequests].ostream.flush();
			break;
		case netErrorPossible:
			pimpl->streams[impl::netrequests].ostream << str.left(60);
			pimpl->streams[impl::netrequests].ostream.flush();
		case errorPossible:
			if (!pimpl->notMixing)
			{
				pimpl->streams[impl::everything].ostream << str;
				pimpl->streams[impl::everything].ostream.flush();
			}
			pimpl->streams[impl::error].ostream << str;
			pimpl->streams[impl::error].ostream.flush();
		default:
			pimpl->streams[impl::everything].ostream << str;
			pimpl->streams[impl::everything].ostream.flush();
		}
	}
}
void debugtrace::printToQDebug(const QString& str)
{
	if (isValid())
	{
		switch (pimpl->msgPriorityLvl)
		{
		case netrequestSent:
		case netresponseReceived:
			pimpl->qDebugFullMessageHolder += str.left(60);
			if (!pimpl->notMixing)
				pimpl->qDebugFullMessageHolder += str.left(60);
			break;
		case netErrorPossible:
			pimpl->qDebugFullMessageHolder += str.left(60);
		case errorPossible:
			pimpl->qDebugFullMessageHolder += str.left(60);
			if (!pimpl->notMixing)
				pimpl->qDebugFullMessageHolder += str.left(60);
			break;
		default:
			pimpl->qDebugFullMessageHolder += str.left(60);
		}
	}
}

void debugtrace::printToString(const QString& str)
{
	if (isValid())
	{
		switch (pimpl->msgPriorityLvl)
		{
		case netrequestSent:
		case netresponseReceived:
			pimpl->streams[impl::netrequests].outstring += str;
			if (!pimpl->notMixing)
				pimpl->streams[impl::everything].outstring += str.left(60);
			break;
		case netErrorPossible:
			pimpl->streams[impl::netrequests].outstring += str;
		case errorPossible:
			pimpl->streams[impl::error].outstring += str;
			if (!pimpl->notMixing)
				pimpl->streams[impl::everything].outstring += str.left(60);
			break;
		default:
			pimpl->streams[impl::everything].outstring += str;
		}
	}
}

void debugtrace::printToAll(const QString& str)
{
	if (isValid())
	{
		pimpl->streams[impl::everything].outstring += str.left(60);
		pimpl->qDebugFullMessageHolder += str.left(60);
		pimpl->streams[impl::everything].ostream << str.left(60);
		pimpl->streams[impl::everything].ostream.flush();
		switch (pimpl->msgPriorityLvl)
		{
		case netrequestSent:
		case netresponseReceived:
			pimpl->streams[impl::netrequests].outstring += str;
			pimpl->qDebugFullMessageHolder += str;
			pimpl->streams[impl::netrequests].ostream << str;
			pimpl->streams[impl::netrequests].ostream.flush();
			break;
		case errorPossible:
			pimpl->streams[impl::error].outstring += str;
			pimpl->qDebugFullMessageHolder += str;
			pimpl->streams[impl::error].ostream << str;
			pimpl->streams[impl::error].ostream.flush();
			break;
		}
		qDebug() << str;
	}
}
void debugtrace::printToConsole(const QString& str)
{
	if (isValid())
	{
#ifdef Q_OS_WINCE
		std::cout << str.toAscii().constData();
#else
		std::cout << str.toStdString();
#endif
		std::cout.flush();
	}
}
void debugtrace::printToSome(const QString& str)
{
	if (isValid())
	{
		for (int i = 0; i < pimpl->omode_united.size(); ++i)
		{
			(*this.*(pimpl->omode_united.at(i)))(str);
		}
	}
}

void debugtrace::printToConsBuff(const QString& /*str*/)
{
}

bool debugtrace::isValid()
{
	if (pimpl->priorityLvl == all || pimpl->msgPriorityLvl <= pimpl->priorityLvl)
	{
		return true;
	}
	return false;
}

void debugtrace::flushQDebug()
{
	pimpl->qDebugFullMessageHolder.replace('\n', ' ');
	qDebug() << pimpl->qDebugFullMessageHolder;
	pimpl->qDebugFullMessageHolder.clear();
}

void debugtrace::flushBuffer()
{
}

debugtrace::debugtrace(DebugPriority priority, OutputMode mode,
	QVector<OutputMode> OutputTo, bool notMixing)
	: pimpl(new impl(priority, mode, &debugtrace::printToAll, notMixing))
{
	changeOutputMode(mode, OutputTo);
}

debugtrace& debugtrace::operator<<(const DebugPriority pri)
{
	pimpl->msgPriorityLvl = pri;
	return *this;
}

debugtrace& debugtrace::operator<<(const int intout)
{
	(*this.*pimpl->outmethod)(QString::number(intout));
	return *this;
}

debugtrace& debugtrace::operator<<(const long long int val)
{
	(*this.*pimpl->outmethod)(QString::number(val));
	return *this;
}

debugtrace& debugtrace::operator<<(const char* msg)
{
	QString temp(msg);
	(*this.*pimpl->outmethod)(temp);
	return *this;
}

debugtrace& debugtrace::operator<<(const QString& msg)
{
	(*this.*pimpl->outmethod)(msg);
	return *this;
}

debugtrace& debugtrace::operator<<(const bool value)
{
	(*this.*pimpl->outmethod)((value) ? "true" : "false");
	return *this;
}

debugtrace& debugtrace::operator<<(const double dout)
{
	(*this.*pimpl->outmethod)(QString::number(dout));
	return *this;
}

debugtrace& debugtrace::operator<<(const std::exception& ex)
{
	(*this.*pimpl->outmethod)(ex.what());
	return *this;
}
debugtrace& debugtrace::operator <<(const char c)
{
	if (pimpl->omode == qDeb || pimpl->omode == toall)
	{
		if (c == '\n')
		{
			flushQDebug();
			return *this;
		}
	}
	(*this.*pimpl->outmethod)(QString() + c);
	return *this;
}

debugtrace& debugtrace::operator<<(quint64 num)
{
	(*this.*pimpl->outmethod)(QString::number(num));
	return *this;
}

void debugtrace::changeOutputMode(const OutputMode mode, QVector<OutputMode>v)
{
	switch (mode)
	{
	case toall:
		pimpl->outmethod = &debugtrace::printToAll;
		break;
	case file:
		pimpl->outmethod = &debugtrace::printToFile;
		break;
	case qStr:
		pimpl->outmethod = &debugtrace::printToString;
		break;
	case qDeb:
		pimpl->outmethod = &debugtrace::printToQDebug;
		break;
	case Cons:
		pimpl->outmethod = &debugtrace::printToConsole;
		break;
	case buffConsole:
		pimpl->outmethod = &debugtrace::printToConsBuff;
		break;
	case some_united:
		pimpl->outmethod = &debugtrace::printToSome;
		for (int i = 0; i < v.count(); ++i)
		{
			switch (v[i])
			{
			case file:
				pimpl->omode_united.push_back(&debugtrace::printToFile);
				break;
			case qStr:
				pimpl->omode_united.push_back(&debugtrace::printToString);
				break;
			case qDeb:
				pimpl->omode_united.push_back(&debugtrace::printToQDebug);
				break;
			case Cons:
				pimpl->omode_united.push_back(&debugtrace::printToConsole);
				break;
			case buffConsole:
				pimpl->omode_united.push_back(&debugtrace::printToConsBuff);
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}
QString debugtrace::getCurrentString()
{
	return pimpl->streams[impl::everything].outstring;
}

debugtrace* debugtrace::_instanse = Q_NULLPTR;
debugtrace* debugtrace::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new debugtrace(all, toall);
	}
	return _instanse;
}

debugtrace& debugtrace::getObject()
{
	return *(instanse());
}

void debugtrace::init(DebugPriority priority, OutputMode mode, QVector<OutputMode> OutputTo, bool nM)
{

#ifdef Q_OS_WINCE
	QDir outdir(QDir::currentPath());
	if (!outdir.exists("UNALogs"))
		outdir.mkdir("UNALogs");
#else
#ifdef Q_OS_WIN
	QDir outdir(QDir::currentPath());
	if (!outdir.exists("UNALogs"))
		outdir.mkdir("UNALogs");
#endif
#endif
	if (_instanse != Q_NULLPTR)
	{
		delete _instanse;
	}
	_instanse = new debugtrace(priority, mode, OutputTo, nM);
}

unsigned long long int makeMsgId()
{
	quint64 id = QDateTime::currentMSecsSinceEpoch();
	id <<= 8;
	quint8 randomnum(rand());
	id += randomnum;
	return id;
}
