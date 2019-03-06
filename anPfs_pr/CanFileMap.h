#pragma once
#include <string>
#include <locale>
#include <sstream>

class CanFileMap
{
public:
	CanFileMap();
	~CanFileMap();
	CanFileMap(const char* fullpath, int fd);
	CanFileMap(const CanFileMap& obj);
	CanFileMap(CanFileMap&& obj);

	CanFileMap& operator=(const CanFileMap& obj);
	CanFileMap& operator=(CanFileMap&& obj);

	bool operator==(const CanFileMap& obj);
	bool operator<(const CanFileMap& obj);

	void push_data(const char* data, size_t len);
	size_t get_data_length() {
		return data_.length();
	}
private:
	std::string name_;
	int fd_;
	std::string data_;
};

