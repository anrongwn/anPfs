#pragma once
#include <string>
#include <locale>
#include <sstream>
#include <fstream>

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
	bool operator==(const int fd);

	void push_data(const char* data, size_t len);
	size_t get_data_length() {
		return data_.length();
	}
	
	int output();
private:
	std::string name_;
	int fd_;
	std::string data_;
};

