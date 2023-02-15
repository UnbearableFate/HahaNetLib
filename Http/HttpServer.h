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
#include "HttpRequest.h"

using namespace std;
namespace Bhttp = boost::beast::http;

using StringRespone = Bhttp::response<Bhttp::string_body>;

using ControllerType = StringRespone(string);
using SimpleControllerType = string(string);
using CustomControllerType = bool(TcpConnection&);

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
    std::map<string, function<SimpleControllerType>> stringRoute;

   public:
    HttpServer(int p, int threadNum) : TcpServer(p, threadNum) {}
    ~HttpServer(){};

    void registerRoute(string subUrl, function<ControllerType> controller) {
        this->simpleRoute.try_emplace(subUrl, controller);
    }
    void registerRoute(string subUrl, function<SimpleControllerType> controller) {
        this->stringRoute.try_emplace(subUrl, controller);
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
        HttpRequest req(parser.get(), &parser);
        conn.setKeepAlive(req.keep_alive());
        req.parseUrl();
        if (this->stringRoute.contains("/")) {
            auto res = this->stringRoute["/"](req.getUrl());
            conn.sendMsg(res);
        }
        if (this->simpleRoute.contains("/")) {
            auto res = this->simpleRoute["/"](req.getUrl());
            auto respStr = this->resp2Str(res);
            conn.sendMsg(respStr);
        }
    }

    void establishConnectionHandle(TcpConnection& conn) {
        cout << "connected from " << conn.getClientAddr() << endl;
    }
};  // namespace TcpServer
