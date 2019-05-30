#include <iostream>
#include "tcp_socket.hpp"
#include "proto.h"

using std::cin;
using std::endl;
using std::cout;
using std::string;

typedef struct info info;


int main(){
  TcpSocket tcp;
  tcp.Socket();
  string ip = "192.168.30.145";
  tcp.Connect(ip, 9090);

  for(;;){
    int num1 = 0;
    int num2 = 0;
    cout << "input num1: ";
    cin >> num1;
    cout << "input num2: ";
    cin >> num2;

    info msg;
    msg.num1 = num1;
    msg.num2 = num2;
    msg.op = '+';

    send(tcp.GetFd(), &msg, sizeof(msg), 0);
  }
  tcp.Close();
  return 0;
}
