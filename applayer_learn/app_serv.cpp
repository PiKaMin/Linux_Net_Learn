#include <iostream>
#include "tcp_socket.hpp"
#include "proto.h"
#include <thread>
typedef struct info info;

using std::endl;
using std::cout;
using std::cin;
using std::string;

void thr_start(TcpSocket clntSock){
  for(;;){
    info msg;
    int recvSz = recv(clntSock.GetFd(), &msg, sizeof msg, 0);
    if (recvSz < 0){
      cout << "recv err" << endl;
      clntSock.Close();
      return ;
    }
    if (recvSz == 0){
      clntSock.Close();
      return;
    }

    cout << msg.num1 << "+" << msg.num2 << "="  << msg.num1 + msg.num2 << endl;
    
  }
}

int main(){
  TcpSocket tcp;
  tcp.Socket();
  string ip = "192.168.30.145";
  tcp.Bind(ip, 9090);
  tcp.Listen(5);

  while(1){
    string clntIp;
    uint16_t clntPort;
    TcpSocket clntSock;
    tcp.Accept(clntSock, &clntIp, &clntPort);
    cout << "Connect clnt" << clntIp << " ---- " << clntPort << endl;

    std::thread calc(thr_start, clntSock);
    calc.detach();

  }
  tcp.Close();
  return 0;
}
