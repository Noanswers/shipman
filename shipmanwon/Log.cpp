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
//	fileLogger.error(msg);

	//�ð� ����
	//FileLogger.setProperty(Poco::FileChannel::PROP_TIMES, "local");

	//��¥ �ð� ���� �߰�
	//FileLogger.setProperty(Poco::FileChannel::PROP_ARCHIVE, "timestamp");

	//����
	//FileLogger.error(msg);
}
