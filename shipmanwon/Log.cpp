#include "stdafx.h"
#include "Log.h"
#include "Poco/ConsoleChannel.h"

Poco::Logger& CLog::fileLogger = Poco::Logger::create("FileLogger",
	new Poco::FileChannel("test.log"), Poco::Message::PRIO_INFORMATION);

void CLog::initialize()
{

}



void CLog::SendErrorLogMessage(const std::string msg)
{
	std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();

	std::string current_time_string = time_point_to_string(currentTime);
	// ���ýð�����
	fileLogger.getChannel()->setProperty(Poco::FileChannel::PROP_TIMES, "local");
	// ���忡��¥�ð������߰�
	fileLogger.getChannel()->setProperty(Poco::FileChannel::PROP_ARCHIVE, "timestamp");

	// ��������
	fileLogger.getChannel()->setProperty(Poco::FileChannel::PROP_COMPRESS, "true");
	// �ϴ����η����̼�
	fileLogger.getChannel()->setProperty(Poco::FileChannel::PROP_ROTATION, "daily");
	// 5�ϰ�����
	fileLogger.getChannel()->setProperty(Poco::FileChannel::PROP_PURGEAGE, "5days");
	
	std::string send_message = current_time_string +"||  log message : " + msg ;

	fileLogger.error(send_message);

	//�ð� ����
	//FileLogger.setProperty(Poco::FileChannel::PROP_TIMES, "local");

	//��¥ �ð� ���� �߰�
	//FileLogger.setProperty(Poco::FileChannel::PROP_ARCHIVE, "timestamp");

	//����
	//FileLogger.error(msg);
}

std::string CLog::time_point_to_string(std::chrono::system_clock::time_point &tp)
{
	using namespace std;
	using namespace std::chrono;

	auto ttime_t = system_clock::to_time_t(tp);
	auto tp_sec = system_clock::from_time_t(ttime_t);
	milliseconds ms = duration_cast<milliseconds>(tp - tp_sec);

	std::tm * ttm = localtime(&ttime_t);

	char date_time_format[] = "%Y.%m.%d-%H.%M.%S";

	char time_str[] = "yyyy.mm.dd.HH-MM.SS.fff";

	strftime(time_str, strlen(time_str), date_time_format, ttm);

	string result(time_str);
	result.append(".");
	result.append(to_string(ms.count()));

	return result;
}