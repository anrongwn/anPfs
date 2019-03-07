#include "stdafx.h"
#include "CanFileMap.h"


CanFileMap::CanFileMap():fd_(-1)
{
}

CanFileMap::CanFileMap(const char* fullpath, int fd) :name_(std::string(fullpath)), fd_(fd) {

}

CanFileMap::~CanFileMap()
{
}

CanFileMap::CanFileMap(const CanFileMap& obj) {
	name_ = obj.name_;
	fd_ = obj.fd_;
	data_ = obj.data_;
}
CanFileMap::CanFileMap(CanFileMap&& obj) : name_(std::move(obj.name_)), fd_(obj.fd_), data_( std::move(obj.data_) ){

}

CanFileMap& CanFileMap::operator=(const CanFileMap& obj) {
	if (this != &obj) {
		name_ = obj.name_;
		fd_ = obj.fd_;
		data_ = obj.data_;
	}

	return (*this);
}
CanFileMap& CanFileMap::operator=(CanFileMap&& obj) {
	if (this != &obj) {
		name_ = std::move(obj.name_);
		fd_ = obj.fd_;
		data_ = std::move(obj.data_);
	}

	return (*this);
}

bool CanFileMap::operator==(const int fd) {
	return (fd_ == fd);
}

bool CanFileMap::operator==(const CanFileMap& obj) {
	bool r = false;

	if ((name_ == obj.name_) && (fd_ == obj.fd_)) {
		r = true;
	}

	return r;
}

bool CanFileMap::operator<(const CanFileMap& obj) {
	bool r = false;
	if ((name_ < obj.name_) && (fd_ < obj.fd_)) {
		r = true;
	}
	return r;
}

void CanFileMap::push_data(const char* data, size_t len) {
	data_ += std::string(data, len);
}

int CanFileMap::output() {
	std::string tmp = name_;
	tmp += "1";

	std::ofstream file(tmp, std::ios::out);
	file << data_;
	//file.flush();
	file.close();

	return 0;
}