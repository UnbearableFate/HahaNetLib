#pragma once
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/url/src.hpp>
#include <map>
#include <string>

#include "HttpRequestHeader.h"
using namespace std;
namespace Bhttp = boost::beast::http;
using BoostHttpRequestType = boost::beast::http::message<true, boost::beast::http::string_body, boost::beast::http::fields>;
class HttpRequest {
   private:
    HttpRequestHeader header;
    map<string, string> query;
    string url;
    string body;
    boost::urls::url_view parsedUrl;
    bool keepAlive;

   public:
    string getUrl() {
        return url;
    }
    bool keep_alive() {
        return this->keepAlive;
    }
    HttpRequest(BoostHttpRequestType &request, const Bhttp::request_parser<Bhttp::string_body> *parser) : header(request.base()) {
        this->url = request.target().to_string();
        this->keepAlive = request.keep_alive();
        this->header.ContentLength = parser->content_length().get_value_or(0);
        this->body = request.body();
    };

    void parseUrl() {
        if (auto urlParseRes = boost::urls::parse_uri(this->url);
            urlParseRes.has_value()) {
            this->parsedUrl = urlParseRes.value();
        }
    }
    ~HttpRequest(){};
};
