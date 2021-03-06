#pragma once
#include "uv.h"
#include <string>
#include <memory>
#include <thread>
#include <atomic>
#include <chrono>
#include <list>
#include <algorithm>
#include "CanFileMap.h"

#include "spdlog/spdlog.h"
#include "spdlog//async.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog//fmt/fmt.h"

using logger = std::shared_ptr<spdlog::logger>;
class CanUVFS
{
public:
	CanUVFS();
	~CanUVFS();

	int start(const char* path, const char* postfix);
	int stop();

	using FM_LIST = std::list<CanFileMap>;

	//日志
	static logger& getlogger();
private:
	static void  event_handler(void * arg);


	static void close_cb(uv_handle_t* handle);
	static void on_walk(uv_handle_t* handle, void* arg);

	static void on_fs_scandir(uv_fs_t* scandired_req);
	static void on_fs_open(uv_fs_t* opened_req);
	static void on_fs_read(uv_fs_t* readed_req);

	//文件内容映射列表
	static FM_LIST s_fml_;

	

private:
	std::atomic_bool flag_ = { ATOMIC_FLAG_INIT };
	uv_thread_t engine_ = { nullptr };
	uv_loop_t * loop_ = { nullptr };

	std::string path_;
	std::string postfix_;


};

