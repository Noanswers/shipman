#pragma once
#include "Poco/Logger.h"
#include "Poco/Message.h"
#include "Poco/FileChannel.h"
#include "Singleton.h"
#include "Poco/ConsoleChannel.h"

//static Poco::Logger& FileLogger;

class CLog : public CSingleton<CLog>
{

public:
	CLog()
	{

	};
	~CLog()
	{
	};

	void initialize();
	void SendErrorLogMessage(const std::string msg);
	static Poco::Logger& fileLogger;
private:
	

};

