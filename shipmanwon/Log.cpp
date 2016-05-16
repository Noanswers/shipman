#include "stdafx.h"
#include "Log.h"
#include "Poco/ConsoleChannel.h"

Poco::Logger& CLog::fileLogger = Poco::Logger::create("FileLogger",
	new Poco::FileChannel("C:\\Users\\ino\\Documents\\shipman_with\\test.log"), Poco::Message::PRIO_INFORMATION);

void CLog::initialize()
{

}

void CLog::SendErrorLogMessage(const std::string msg)
{
	// 로컬시간지정
	fileLogger.getChannel()->setProperty(Poco::FileChannel::PROP_TIMES, "local");
	// 저장에날짜시간문자추가
	fileLogger.getChannel()->setProperty(Poco::FileChannel::PROP_ARCHIVE, "timestamp");

	// 압축있음
	fileLogger.getChannel()->setProperty(Poco::FileChannel::PROP_COMPRESS, "true");
	// 일단위로로테이션
	fileLogger.getChannel()->setProperty(Poco::FileChannel::PROP_ROTATION, "daily");
	// 5일간보존
	fileLogger.getChannel()->setProperty(Poco::FileChannel::PROP_PURGEAGE, "5days");
//	fileLogger.error(msg);

	//시간 지정
	//FileLogger.setProperty(Poco::FileChannel::PROP_TIMES, "local");

	//날짜 시간 문자 추가
	//FileLogger.setProperty(Poco::FileChannel::PROP_ARCHIVE, "timestamp");

	//에러
	//FileLogger.error(msg);
}
