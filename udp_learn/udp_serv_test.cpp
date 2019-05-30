#include <iostream>
#include <string>
#include "udp_socket.hpp"

using std::cout;
using std::endl;
using std::cin;

int main(){
  UdpSocket myServ;
  myServ.Socket();
  std::string ip = "192.168.30.144";
  myServ.Bind(ip, 14396);

  std::string recvStr;
  std::string recvIp;
  uint16_t recvPort;
  while(1) {
    myServ.Recv(recvStr, &recvIp, &recvPort);
    cout << "recv from " << recvIp << "--" << recvPort ; 
    cout << " : " << recvStr << endl;
  }
  myServ.Close();
  return 0;
}
