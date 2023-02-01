#include <fcntl.h>
#include <pthread.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Http/HttpServer.h"

const size_t MAX_FILE_SIZE = 20000000;
class MvStreamerApp : public HttpServer {
   private:
    const string videoPath = "/mnt/f/Video/kimidoko";
    StringRespone errorRespone() {
        StringRespone res;
        res.body() = "error";
        res.result(500);
        res.prepare_payload();
        return res;
    }

   public:
    StringRespone getFullVideo(string name) {
        int videoFd = open((this->videoPath + name).data(), O_RDONLY);

        if (videoFd < 0) {
            return this->errorRespone();
        }
        string vdata(MAX_FILE_SIZE, '\0');
        StringRespone res;
        if (auto n = read(videoFd, vdata.data(), MAX_FILE_SIZE); n < 0) {
            return this->errorRespone();
        } else {
            res.body() = vdata.substr(0, n);
        }
        res.insert("Access-Control-Allow-Origin", "*");
        res.insert("Server", "hlsServer");
        if (name == "/index.m3u8") {
            res.insert("Content-Type", "application/vnd.apple.mpegurl; charset=utf-8");
            res.insert("Connection", " keep-alive");
            res.insert("Keep-Alive", "timeout=30, max=100");
        } else {
            res.insert("Content-Type", "video/mp2t; charset=utf-8");
            res.insert("Connection", " close");
            res.insert("Keep-Alive", "timeout=30, max=100");
        }

        res.result(200);
        res.prepare_payload();
        return res;
    }

    MvStreamerApp(int port, int threadNum) : HttpServer(port, threadNum) {
        this->registerRoute("/", std::bind(&MvStreamerApp::getFullVideo, this, std::placeholders::_1));
    };
};

int main() {
    std::shared_ptr<HttpServer> server = std::make_shared<MvStreamerApp>(9877, 3);
    server->start();
}