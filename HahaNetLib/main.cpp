#include<iostream>
#include<string>
#include<sstream>
#include<pthread.h>
#include<memory>
#include "TcpServer.h"
#include <iostream>
using namespace std;
class MyServer :public TcpServer {
public:
	MyServer(int p) :TcpServer(p) {
	}
	void requestHandle(TcpConnection& conn) {
		auto msg = conn.receiveMsg();
		cout << conn.getClientAddr()<<" : " << msg << endl;
		conn.sendMsg(msg);
	}

	void establishConnectionHandle(TcpConnection& conn) {
		cout << "connected from " << conn.getClientAddr() << endl;
	}
};


int main() {
	std::shared_ptr<MyServer> server = std::make_shared<MyServer>(9877);
	server->start();
}