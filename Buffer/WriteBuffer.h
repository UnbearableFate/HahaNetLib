#pragma once
#include <string.h>

#include "Buffer.h"
class WriteBuffer : public Buffer {
   public:
    WriteBuffer() : Buffer() {}
    auto writeTo(int fd) -> long;
    auto saveToBuf(std::string str) -> void;
    ~WriteBuffer() {
        this->data.clear();
    }
};
