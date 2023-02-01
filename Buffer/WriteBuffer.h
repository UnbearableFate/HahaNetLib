#pragma once
#include "Buffer.h"
#include <string.h>
class WriteBuffer:public Buffer 
{
public:
	WriteBuffer() :Buffer() {}
	auto writeTo(int fd)->size_t;
	auto saveToBuf(std::string str)->void;
};

