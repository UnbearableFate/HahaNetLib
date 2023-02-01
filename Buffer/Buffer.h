#pragma once
#include <sys/socket.h>

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <system_error>
#include <vector>
class Buffer {
   protected:
    static const size_t DEF_MAX_LEN;
    std::vector<char> data;
    size_t
        length;  // length of data which can be read by user or send to socket
    size_t beg;  // chars before beg have been sent or read by user,
    auto reset() {
        // data.clear();
        beg = 0;
        length = 0;
    }

   public:
    auto begin() { return data.begin() + beg; }

    auto beginPtr() { return data.data() + beg; }

    auto end() { return this->begin() + this->length; }

    auto endPtr() { return this->beginPtr() + this->length; }
    auto getLength() { return this->length; }

    size_t blankSpaceSize() { return this->data.end() - this->end(); }

    Buffer() : data(2048), beg(0) {}
};
