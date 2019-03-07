#include "stdafx.h"
#include "CanUVFS.h"
#include "CanReadReq.h"
std::list<CanFileMap> CanUVFS::s_fml_;

CanUVFS::CanUVFS()
{
}


CanUVFS::~CanUVFS()
{
}

void CanUVFS::close_cb(uv_handle_t* handle) {

}
void CanUVFS::on_walk(uv_handle_t* handle, void* arg) {
	if (!uv_is_closing(handle)) {
		uv_close(handle, nullptr);
	}
}

void CanUVFS::event_handler(void * arg) {
	CanUVFS * that = static_cast<CanUVFS*>(arg);

	if (std::atomic_exchange(&that->flag_, true)) return;

	//main run message loop
	int more = 0;
	while (that->flag_) {
		more = uv_run(that->loop_, UV_RUN_NOWAIT);

		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (more) continue;
	}

	//fully close handle
	uv_walk(that->loop_, CanUVFS::on_walk, nullptr);
	uv_run(that->loop_, UV_RUN_DEFAULT);
	do {
		more = uv_loop_close(that->loop_);
		if (UV_EBUSY == more) {
			uv_run(that->loop_, UV_RUN_NOWAIT);
		}
	} while (more);

}

int CanUVFS::start(const char* path, const char* postfix) {
	int r = 0;
	path_ = std::move(std::string(path));
	postfix_ = std::move(std::string(postfix));

	if (nullptr == loop_) {
		loop_ = uv_default_loop();
	}

	r = uv_thread_create(&engine_, CanUVFS::event_handler, this);
	if (0 == r) {
		//开始扫描路径
		uv_fs_t * req = (uv_fs_t *)malloc(sizeof uv_fs_t);
		if (req) {
			uv_req_set_data((uv_req_t*)req, this);
		}
		
		r = uv_fs_scandir(loop_, req, path_.c_str(), 0, CanUVFS::on_fs_scandir);
	}

	return r;
}

int CanUVFS::stop() {
	int r = 0;
	if (!std::atomic_exchange(&this->flag_, false)) return r;

	//针对自启uv_run
	if ((loop_) && (engine_)) {
		uv_stop(loop_);
	}

	uv_thread_join(&engine_);
	loop_ = nullptr;

	return r;
}

void CanUVFS::on_fs_scandir(uv_fs_t* scandired_req) {
	CanUVFS * that = static_cast<CanUVFS*>(scandired_req->data);

	int r = 0;
	uv_dirent_t dir = { 0x00 };
	if (scandired_req->result) {

		while (UV_EOF != uv_fs_scandir_next(scandired_req, &dir)) {
			//读文件
			if (UV_DIRENT_FILE == dir.type) {
				uv_fs_t * open_req = (uv_fs_t *)malloc(sizeof uv_fs_t);;
				uv_req_set_data((uv_req_t*)open_req, that);
				
				std::string fullname(scandired_req->path);
				fullname += R"(\)";
				fullname += dir.name;
				r = uv_fs_open(scandired_req->loop, open_req, fullname.c_str(), O_RDONLY, 0, CanUVFS::on_fs_open);
			}
		}
	}
	
	uv_fs_req_cleanup(scandired_req);
	free(scandired_req);
}

//static char buffer[1024] = { 0x00 };
void CanUVFS::on_fs_open(uv_fs_t* opened_req) {
	CanUVFS * that = static_cast<CanUVFS*>(opened_req->data);

	int r = 0;
	if (opened_req->result!=-1) {
		CanReadReq * read_req = new CanReadReq();// (uv_fs_t *)malloc(sizeof uv_fs_t);
		//uv_req_set_data((uv_req_t*)read_req, opened_req);
		uv_buf_t buf = uv_buf_init(read_req->data2, DATA2_LEN);
		r = uv_fs_read(opened_req->loop, read_req, opened_req->result, &buf, 1, -1, CanUVFS::on_fs_read);

		//CanFileMap fm(opened_req->path, opened_req->result);
		CanUVFS::s_fml_.emplace_back(CanFileMap(opened_req->path, opened_req->result));
	}

	uv_fs_req_cleanup(opened_req);
	free(opened_req);

}

void CanUVFS::on_fs_read(uv_fs_t* readed_req) {
	//uv_fs_t * open_req = static_cast<uv_fs_t*>(readed_req->data);

	int r = 0;
	if (readed_req->result < 0) {
		
	}
	else if (readed_req->result == 0) {
		uv_fs_t close_req;
		uv_fs_close(readed_req->loop, &close_req, readed_req->file.fd, NULL);

		//测试输出
		auto iter = std::find(std::begin(s_fml_), std::end(s_fml_), readed_req->file.fd);
		if (iter != s_fml_.end()) {
			iter->output();
		}
	}
	else if (readed_req->result > 0) {
		//
		auto iter = std::find(std::begin(s_fml_), std::end(s_fml_), readed_req->file.fd);
		if (iter != s_fml_.end()) {
			iter->push_data(readed_req->fs.info.bufs->base, readed_req->result);
		}
		
		
		//再读
		//uv_fs_t * read_req = (uv_fs_t *)malloc(sizeof uv_fs_t);;
		CanReadReq * read_req = new CanReadReq();
		read_req->loop = readed_req->loop;
		read_req->file.fd = readed_req->file.fd;

		uv_buf_t buf = uv_buf_init(read_req->data2, DATA2_LEN);
		r = uv_fs_read(read_req->loop, read_req, readed_req->file.fd, &buf, 1, -1, CanUVFS::on_fs_read);
		
	}

	//清除
	uv_fs_req_cleanup(readed_req);
	//free(readed_req);
	delete readed_req;
}