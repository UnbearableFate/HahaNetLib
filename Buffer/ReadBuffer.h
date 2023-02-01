#pragma once
#include "Buffer.h"
#include "sys/socket.h"
#include "sys/uio.h"
#include<array>
#include<exception>

constexpr int EXTRA_DATA_LEN = 65536;
class ReadBuffer : public Buffer
{
public:
	ReadBuffer() :Buffer() {}
	auto readFromSocket(int fd)->size_t;
	auto getStringFromBuf()->std::string;
};
