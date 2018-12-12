#include "log.h"
#include "CTime.h"

void InitLog(char *argv[])
{
	FLAGS_log_dir = "./log";
	std::string fileName = CTime::currentDate();
	
	google::InitGoogleLogging(argv[0]);
	google::SetLogDestination(google::INFO,		std::string(fileName + ".info").c_str());
	google::SetLogDestination(google::WARNING,	std::string(fileName + ".warn").c_str());
	google::SetLogDestination(google::GLOG_ERROR,	std::string(fileName + ".error").c_str());
	google::SetLogDestination(google::FATAL,	std::string(fileName + ".fatal").c_str());
	google::SetLogFilenameExtension(".log");

	// ���ü������ google::INFO ����־ͬʱ�������Ļ
	google::SetStderrLogging(google::INFO);
	FLAGS_max_log_size = 1000;
	FLAGS_colorlogtostderr = true;
	// ������־�����Ĭ��Ϊ30�룬�˴���Ϊ�������
	FLAGS_logbufsecs = 0;
	FLAGS_stop_logging_if_full_disk = true;
}

void CloseLog()
{
	google::ShutdownGoogleLogging();
}
