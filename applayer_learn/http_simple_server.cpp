#include <iostream>
#include <string>
#include "tcp_socket.hpp"
#include <sstream>

using std::string;
using std::cout;
using std::endl;

int main(){
  TcpSocket tcp;
  tcp.Socket();
  string ip = "192.168.30.145";
  tcp.Bind(ip ,9090);
  tcp.Listen(5);

  for(;;){
    TcpSocket newSock;
    string cIp;
    uint16_t cPort;
    tcp.Accept(newSock, &cIp, & cPort);

    cout << "Connct client: " << cIp << "--" << cPort  << endl;   
    string req;
    newSock.Recv(req);
    cout << "Request: [ " << req << "]" << endl;

    string body = "<html><body><h1>Hello World!!</h1></body></html>"; // reponse 正文
    std::stringstream ss;
    ss << "HTTP/1.1 200 OK\r\n"; //HTTP response 首行
    ss << "Content-Type: text/html\r\n";
    ss << "Content-Length: " << body.size() << "\r\n";
    ss << "\r\n";
    string header = ss.str();

    newSock.Send(header);
    newSock.Send(body);
    newSock.Close();

  }

  tcp.Close();
  return 0;
}

