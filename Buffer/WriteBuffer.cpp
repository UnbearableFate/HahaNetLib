#include "WriteBuffer.h"

#include <errno.h>
#include <string.h>

auto WriteBuffer::writeTo(int fd) -> long {
    std::unique_lock<std::mutex> lock(mtx);
    if (auto okLength = send(fd, this->beginPtr(), this->length, 0); okLength > 0) {
        this->beg += okLength;
        this->length -= okLength;
    } else {
        return okLength;
    }
    if (this->length == 0) {
        this->reset();
    }
    return this->length;
}

auto WriteBuffer::saveToBuf(std::string str) -> void {
    std::unique_lock<std::mutex> lock(mtx);
    if (this->blankSpaceSize() > str.size()) {
        memcpy(this->endPtr(), str.data(), str.size());
    } else {
        data.insert(this->end(), str.begin(), str.end());
    }
    this->length += str.size();
}
