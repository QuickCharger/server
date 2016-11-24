#ifndef _LOG_H_
#define _LOG_H_

#define GOOGLE_GLOG_DLL_DECL
#include "glog/logging.h"

void InitLog(char *argv[])
{
	// Initialize Google's logging library.    
	google::InitGoogleLogging(argv[0]);
	//google::SetLogDestination(google::INFO, (strFilePath + "//log").c_str());
	google::SetLogDestination(google::GLOG_ERROR, "./myInfo");
	//google::SetLogFilenameExtension(".txt");
	FLAGS_colorlogtostderr = true;

	// 设置 google::INFO 级别的日志存储路径和文件名前缀
	google::SetLogDestination(google::INFO, "INFO_");
	// 设置 google::WARNING 级别的日志存储路径和文件名前缀
	google::SetLogDestination(google::WARNING, ".");
	// 设置 google::ERROR 级别的日志存储路径和文件名前缀
	google::SetLogDestination(google::GLOG_ERROR, "./ERROR_");
	// 设置 google::FATAL 级别的日志存储路径和文件名前缀
	google::SetLogDestination(google::FATAL, "./FATAL_");

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
