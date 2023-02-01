#pragma once
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/string_body.hpp>
#include <map>
#include <string>

#include "HttpHeader.h"
using namespace std;
class HttpRequest {
   private:
    HttpHeader header;
    map<string, string> query;
    string url;

   public:
    HttpRequest(
        boost::beast::http::message<true, boost::beast::http::string_body,
                                    boost::beast::http::fields>
            msg)
        : header(msg.base()) {
        this->url = static_cast<string>(msg.target());
    };
    ~HttpRequest();
};
