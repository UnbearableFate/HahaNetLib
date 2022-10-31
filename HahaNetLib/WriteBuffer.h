#pragma once
#include "Buffer.h"
class WriteBuffer:public Buffer 
{
public:
	WriteBuffer() :Buffer() {}
	auto writeTo(int fd) {
		do
		{
			if (int okLength = send(fd, data.data() + beg, this->length, 0); okLength > 0) {
				this->beg += okLength;
				this->length -= okLength;
			}
			else
			{
				throw std::runtime_error("send error");
			}
		} while (this->length > 0);
		this->reset();
	}
	
	auto saveToBuf(std::string str) {
		this->expansion();
		data.insert(this->end(), str.begin(), str.end());
		this->length += str.size();
	}
};

