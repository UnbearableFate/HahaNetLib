#include "WriteBuffer.h"

auto WriteBuffer::writeTo(int fd)->size_t {
	if (auto okLength = send(fd, this->beginPtr(), this->length, 0); okLength > 0) {
		this->beg += okLength;
		this->length -= okLength;
	}
	else
	{
		throw std::runtime_error("send error");
	}
	if (this->length == 0) {
		this->reset();
	}
	return this->length;
}

auto WriteBuffer::saveToBuf(std::string str)->void {
	if (this->blankSpaceSize() > str.size()) {
		memcpy(this->endPtr(), str.data(), str.size());
	} else {
		data.insert(this->end(), str.begin(), str.end());
	}
	this->length += str.size();
}
