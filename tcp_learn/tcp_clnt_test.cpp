#include <iostream>
#include <string>
#include "tcp_socket.hpp"


using std::string;
using std::cout;
using std::endl;
using std::cin;

int main(){
  TcpSocket tcp;
  tcp.Socket();
  string ip = "192.168.30.145";
  tcp.Connect(ip, 14396);
  
  while(1) {
    string msg;
    getline(cin, msg);
    tcp.Send(msg);

    string resp;
    tcp.Recv(resp);
    cout << "recv msg: " << msg << endl;
  }

  tcp.Close();
  return 0;
}
