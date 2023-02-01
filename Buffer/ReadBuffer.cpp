#include "ReadBuffer.h"

auto ReadBuffer::readFromSocket(int fd) -> size_t {
	std::array<char, EXTRA_DATA_LEN> extraData;
	iovec iovecObj[2];
	iovecObj[0].iov_base = &*this->end();
	iovecObj[0].iov_len = this->blankSpaceSize();
	iovecObj[1].iov_base = extraData.data();
	iovecObj[1].iov_len = EXTRA_DATA_LEN;

	msghdr msghdrObj;
	msghdrObj.msg_iov = iovecObj;
	msghdrObj.msg_iovlen = 2;
	auto n = recvmsg(fd, &msghdrObj, 0);
	if (n < 0) {
		throw std::runtime_error("recvmsg error");
	}

	if (n > this->blankSpaceSize())
	{
		this->data.insert(data.end(), extraData.data(), extraData.data() + n - this->blankSpaceSize());
	}
	this->length += n;
	return this->length;
}

auto ReadBuffer::getStringFromBuf() -> std::string {
	auto res = std::string(this->begin(), this->end());
	this->reset();
	return res;
}
