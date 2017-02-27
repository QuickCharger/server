#ifndef _LOG_H_
#define _LOG_H_

#define GOOGLE_GLOG_DLL_DECL
#undef ERROR
#include "glog/logging.h"

void InitLog(char *argv[])
{
	FLAGS_max_log_size = 300;
	FLAGS_log_dir = ".\\log";
	time_t tCurrent = time(nullptr);
	struct tm *pTime = localtime(&tCurrent); 
	char chTime[100];
	strftime(chTime, sizeof(chTime), "%Y_%m_%d %H_%M_%S", pTime);
	std::string strTime = chTime;
	std::string LOG_INFO_FILE = FLAGS_log_dir + strTime + ".info";
	std::string LOG_WARNING_FILE = FLAGS_log_dir + strTime + ".warn";
	std::string LOG_ERROR_FILE = FLAGS_log_dir + strTime + ".error";
	std::string LOG_FATAL_FILE = FLAGS_log_dir + strTime + ".fatal";
	google::SetLogFilenameExtension(".log");
	FLAGS_logbufsecs = 0;

	google::InitGoogleLogging(argv[0]);
	FLAGS_colorlogtostderr = true;
	google::SetLogDestination(google::INFO, LOG_INFO_FILE.c_str());
	google::SetLogDestination(google::WARNING, LOG_WARNING_FILE.c_str());
	google::SetLogDestination(google::GLOG_ERROR, LOG_ERROR_FILE.c_str());
	google::SetLogDestination(google::FATAL, LOG_FATAL_FILE.c_str());

	// 设置级别高于 google::INFO 的日志同时输出到屏幕
	google::SetStderrLogging(google::INFO);
	// 缓冲日志输出，默认为30秒，此处改为立即输出
	FLAGS_logbufsecs = 0;
	// 最大日志大小为 100MB
	FLAGS_max_log_size = 100;
	// 当磁盘被写满时，停止日志输出
	//FLAGS_stop_logging_if_full_disk = true;
}

void CloseLog()
{
	google::ShutdownGoogleLogging();
}

#undef GOOGLE_GLOG_DLL_DECL

#endif
