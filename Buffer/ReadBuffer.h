#pragma once
#include <array>
#include <exception>

#include "Buffer.h"
#include "sys/socket.h"
#include "sys/uio.h"

constexpr int EXTRA_DATA_LEN = 65536;
class ReadBuffer : public Buffer {
   public:
    ReadBuffer() : Buffer() {}
    auto readFromSocket(int fd) -> size_t;
    auto getStringFromBuf() -> std::string;
    ~ReadBuffer() {
        this->data.clear();
    }
};
