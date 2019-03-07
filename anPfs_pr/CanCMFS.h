#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>

#include "spdlog/spdlog.h"
#include "spdlog//async.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog//fmt/fmt.h"

using logger = std::shared_ptr<spdlog::logger>;
class CanCMFS
{
public:
	CanCMFS();
	~CanCMFS();

	int start(const char* path, const char* postfix);

	//»’÷æ
	static logger& getlogger();
private:
	static void  event_handler(void * arg);

private:
	std::string path_;
	std::string postfix_;

	std::thread thread_;
};

