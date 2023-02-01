#pragma once
#include <boost/beast/http/message.hpp>
#include <map>
#include <string>
using namespace std;

class HttpHeader {
   private:
    /* use string in ver 0.1 */
    string ContentType;
    string ContentLength;
    string Host;
    string UserAgent;
    string Accept;
    string AcceptEncoding;
    string Connection;
    map<string, string> otherHeaderElements;

   public:
    HttpHeader(
        boost::beast::http::header<true, boost::beast::http::fields> para) {
        for (const auto& key : para) {
            auto keyStr = static_cast<string>(key.name_string());
            switch (keyStr) {
                case /* constant-expression */:
                    /* code */
                    break;

                default:
                    break;
            }
        }
    }
    ~HttpHeader();
};