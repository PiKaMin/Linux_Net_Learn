#include <iostream>
#include <string>
#include "udp_socket.hpp"
using std::cout;
using std::endl;
using std::cin;

int main(){
  UdpSocket udpClnt;
  udpClnt.Socket();
  std::string ip = "192.168.30.144";
  while(1){
    std::string msg;
    cout << "input msg: " ;
    fflush(stdout);
    cin >> msg;
    
    udpClnt.Send(msg, ip, 14396);
  }
  udpClnt.Close();
  return 0;
}
