#include <fcntl.h>
#include <unistd.h>

#include <string>

#include "../Http/HttpRequest.h"
#include "../Net/TcpServer.h"
const int BUFF_SIZE = 4000000;
void func(int videoFd, TcpConnection& conn) {
    if (!conn.isConnnected()) {
        close(videoFd);
        return;
    }
    string buf(BUFF_SIZE, '\0');
    auto n = read(videoFd, buf.data(), BUFF_SIZE);
    if (n > 0) {
        conn.sendMsg(buf.substr(0, n));
        conn.sendAsyncFunc(std::bind(func, videoFd, std::ref(conn)));
        sleep(1);
    } else {
        close(videoFd);
    }
}

class HttpFlvService : public TcpServer {
   private:
    inline static const std::string videoRootPath = "/mnt/f/Video";

   public:
    void requestHandle(TcpConnection& conn) {
        constexpr char http_headers[] =
            "HTTP/1.1 200 OK\r\n"
            "Access-Control-Allow-Origin: * \r\n"
            "Content-Type: video/x-flv\r\n"
            "Content-Length: -1\r\n"
            "Connection: Keep-Alive\r\n"
            "Expires: -1\r\n"
            "Pragma: no-cache\r\n"
            "\r\n";
        int http_headers_len = strlen(http_headers);

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
        req.parseUrl();

        auto videoFd = open((videoRootPath + req.getUrl()).data(), O_RDONLY);
        conn.sendMsg(http_headers);
        conn.sendAsyncFunc(std::bind(func, videoFd, std::ref(conn)));
    }
    HttpFlvService() : TcpServer(9877, 2){};
    ~HttpFlvService(){};
};