#pragma once
#include<vector>
#include<cstddef>
#include<string>
#include<sys/socket.h>
#include<system_error>
#include<algorithm>
class Buffer
{
protected:
	static const size_t DEF_MAX_LEN;
	std::vector<char> data;
	size_t length;
	size_t beg;
	auto expansion() {
		if (beg + length >= data.size() / 2) {
			data.resize(std::max(data.size() * 2,DEF_MAX_LEN));
		}
	}
	auto reset() {
		data.clear();
		data.resize(DEF_MAX_LEN);
		beg = 0;
		length = 0;
	}
public:
	auto begin() {
		return data.begin()+beg;
	}

	auto end() {
		return this->begin() + this->length ;
	}
	auto getLength() {
		return this->length;
	}

	Buffer():data(2048),beg(0) {
	}

};

