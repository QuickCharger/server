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

	// ���� google::INFO �������־�洢·�����ļ���ǰ׺
	google::SetLogDestination(google::INFO, "INFO_");
	// ���� google::WARNING �������־�洢·�����ļ���ǰ׺
	google::SetLogDestination(google::WARNING, ".");
	// ���� google::ERROR �������־�洢·�����ļ���ǰ׺
	google::SetLogDestination(google::GLOG_ERROR, "./ERROR_");
	// ���� google::FATAL �������־�洢·�����ļ���ǰ׺
	google::SetLogDestination(google::FATAL, "./FATAL_");

	// ���ü������ google::INFO ����־ͬʱ�������Ļ
	google::SetStderrLogging(google::INFO);
	// ������־�����Ĭ��Ϊ30�룬�˴���Ϊ�������
	FLAGS_logbufsecs = 0;
	// �����־��СΪ 100MB
	FLAGS_max_log_size = 100;
	// �����̱�д��ʱ��ֹͣ��־���
	//FLAGS_stop_logging_if_full_disk = true;
}

void CloseLog()
{
	google::ShutdownGoogleLogging();
}

#undef GOOGLE_GLOG_DLL_DECL

#endif
