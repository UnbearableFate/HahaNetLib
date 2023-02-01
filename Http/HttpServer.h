#pragma once
#include <boost/beast/core/error.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/url/src.hpp>
#include <iostream>
#include <sstream>

#include "../Net/TcpServer.h"

using namespace std;
namespace Bhttp = boost::beast::http;
using StringRespone = Bhttp::response<Bhttp::string_body>;
using ControllerType = StringRespone(string);
class HttpServer : public TcpServer {
   private:
    /* data */
    string resp2Str(Bhttp::response<Bhttp::string_body>& res) {
        stringstream ss;
        if (res.result_int() == 200) {
            ss << "HTTP/1.1 200 OK\n";
        }
        if (res.result_int() == 500) {
            ss << "HTTP/1.1 200 Internal Server Error\n";
        }

        for (auto& h : res.base()) {
            ss << h.name_string() << ": " << h.value() << "\n";
        }
        ss << "\n";
        ss << res.body();
        auto str = ss.str();
        return str;
    }

   protected:
    std::map<string, function<ControllerType>> simpleRoute;

   public:
    HttpServer(int p, int threadNum) : TcpServer(p, threadNum) {}
    ~HttpServer(){};
    void registerRoute(string subUrl, function<ControllerType> controller) {
        this->simpleRoute.try_emplace(subUrl, controller);
    }
    void requestHandle(TcpConnection& conn) {
        auto reqStr = conn.receiveMsg();
        Bhttp::request_parser<Bhttp::string_body> parser;
        boost::beast::error_code ec;
        boost::beast::net::const_buffer cbuffer(reqStr.data(), reqStr.size());
        parser.eager(true);
        parser.put(cbuffer, ec);
        if (parser.need_eof()) {
            parser.put_eof(ec);
        }
        auto pRes = parser.get();
        auto parsedBody = pRes.body();
        auto reqUrl = pRes.target();
        auto met = pRes.method();
        auto keepAlive = pRes.keep_alive();
        if (auto urlParseRes = boost::urls::parse_uri(reqUrl);
            urlParseRes.has_value()) {
            auto query = urlParseRes.value().query();
        }

        auto length = parser.content_length().get_value_or(0);
        auto body = reqStr.substr(reqStr.size() - length, length);
        for (const auto& e : pRes.base()) {
            cout << e.name_string() << " : " << e.value() << endl;
        }

        auto res = this->simpleRoute["/"](reqUrl.to_string());
        /*
StringRespone resp;
resp.result(200);
resp.body() = body;
resp.insert("Content-Type", "text/plain ; charset=utf-8");
resp.prepare_payload();
        */
        auto respStr = this->resp2Str(res);
        conn.sendMsg(respStr);
    }

    void establishConnectionHandle(TcpConnection& conn) {
        cout << "connected from " << conn.getClientAddr() << endl;
    }
};  // namespace TcpServer
