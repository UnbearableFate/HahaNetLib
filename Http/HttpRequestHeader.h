#pragma once
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <map>
#include <string>
using namespace std;

using BoostRequestHeader = boost::beast::http::header<true, boost::beast::http::fields>;
class HttpRequestHeader {
   private:
    /* use string in ver 0.1 */
    BoostRequestHeader self;
    string ContentType;
    int ContentLength;
    string Host;
    string UserAgent;
    string Accept;
    string AcceptEncoding;
    string Connection;
    map<string, string> otherHeaderElements;

   public:
    friend class HttpRequest;
    HttpRequestHeader(BoostRequestHeader header) {
        this->self = header;
        for (const auto& ele : header) {
            auto name = ele.name_string();
            if (name == "Content-Type") {
                this->ContentType = ele.value().to_string();
                continue;
            }

            if (name == "Host") {
                this->Host = ele.value().to_string();
                continue;
            }

            if (name == "User-Agent") {
                this->UserAgent = ele.value().to_string();
                continue;
            }

            if (name == "Accept") {
                this->Accept = ele.value().to_string();
                continue;
            }

            if (name == "Accept-Encoding") {
                this->AcceptEncoding = ele.value().to_string();
                continue;
            }

            if (name == "Connection") {
                this->Connection = ele.value().to_string();
                continue;
            }
            this->otherHeaderElements.try_emplace(name.to_string(), ele.value().to_string());
        }
    }
};