#pragma once
#include "Buffer.h"
class ReadBuffer: public Buffer
{
public:
	ReadBuffer():Buffer() {}
	auto readFrom(int fd) -> auto {
		this->expansion();
		this->length += recv(fd, data.data() + beg, data.size() - beg, 0);
		return this->length;
	}

	auto getStringFromBuf() -> std::string {
		auto res = std::string(data.begin(), data.begin() + length);
		this->reset();
		return res;
	}
};

