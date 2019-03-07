#include "stdafx.h"
#include "CanCMFS.h"

logger& CanCMFS::getlogger() {
	static logger g_anlog;
	if (!g_anlog) {

		std::string logpath(R"(D:\MyTest\2018_C++\anPfs\logs\cmfs)");
		logpath += ".log";

		//是否已启动日志线程池?
		auto tp = spdlog::thread_pool();
		if (!tp) {
			spdlog::init_thread_pool(65536, 1);
		}
		g_anlog = spdlog::daily_logger_mt<spdlog::async_factory>("cmfs", logpath);

		g_anlog->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^---%L---%$] [%t] %v");
		g_anlog->set_level(spdlog::level::trace);
		g_anlog->flush_on(spdlog::level::info);

	}

	return g_anlog;
}

CanCMFS::CanCMFS()
{
}


CanCMFS::~CanCMFS()
{
	if (thread_.joinable()) {
		thread_.join();
	}
}

int CanCMFS::start(const char* path, const char* postfix) {
	int r = 0;
	path_ = std::move(std::string(path));
	postfix_ = std::move(std::string(postfix));

	CanCMFS::getlogger()->info("CanCMFS::start({})", path);

	if (!thread_.joinable()) {
		thread_= std::thread(CanCMFS::event_handler, this);
	}
	
	return r;
}

#define BUFFER_LEN	(1024)
void CanCMFS::event_handler(void * arg) {
	CanCMFS * that = static_cast<CanCMFS*>(arg);
	CString fn(that->path_.c_str());
	fn += R"(\*)";
	fn += that->postfix_.c_str();

	CFileFind ff;

	BOOL b = ff.FindFile(fn);
	UINT perlen = 0;
	while (b) {
		b = ff.FindNextFile();

		if (ff.IsDirectory() || ff.IsDots()) continue;

		CFile cf;
		std::string data;
		char buffer[BUFFER_LEN];
		if (cf.Open(ff.GetFilePath(), CFile::OpenFlags::modeRead)) {
			do {
				perlen = cf.Read(buffer, BUFFER_LEN);

				if (perlen)
					data += std::string(buffer, perlen);
			} while (perlen == BUFFER_LEN);

			if (data.length()) {
				std::string tmp = ff.GetFilePath().operator LPCSTR();
				tmp += "2";

				std::ofstream file(tmp, std::ios::out);
				file << data;
				file.close();

				CanCMFS::getlogger()->info("CanCMFS::event_handler({}) completed.", tmp);
			}
			
		}
		cf.Close();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}