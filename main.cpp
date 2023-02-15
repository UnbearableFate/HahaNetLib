#include "MediaService/HttpFlvService.h"
#include "Net/TcpServer.h"

int main() {
    // std::shared_ptr<HttpFlvService> server = std::make_shared<HttpFlvService>();
    // server->start();
    HttpFlvService server{};
    server.start();
}