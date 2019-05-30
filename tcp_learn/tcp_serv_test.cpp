#include <iostream>
#include <string>
#include "tcp_socket.hpp"

using std::cout;
using std::cin;
using std::string;
using std::endl;

int main(){
  TcpSocket tcp;

  tcp.Socket();
  string ip = "192.168.30.145";
  tcp.Bind(ip, 14396);
  tcp.Listen(5);


  while(1) {
    TcpSocket newSock;
    string clntIp;
    uint16_t clntPort;
    tcp.Accept(newSock, &clntIp, &clntPort);
    cout << "Connect: (" << clntIp << "--" << clntPort << ")" << endl;

    for(;;){
      string msg;
      if (!newSock.Recv(msg)){
        newSock.Close();
        break;
      }
      cout << "recv: " << msg << endl;

      newSock.Send(msg);
    }


  }

  tcp.Close();
  return 0;
}
