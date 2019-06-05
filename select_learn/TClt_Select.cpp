#include <iostream>
#include <cstdio>
#include <sys/types.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

using std::cin;
using std::cout;
using std::string;
using std::endl;

int main(){
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    cout << "socket error" << endl;
    return 1;
  }

  sockaddr_in servAddr;
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(9080);
  servAddr.sin_addr.s_addr = inet_addr("192.168.30.146");
  if (connect(sock, (sockaddr*)&servAddr, sizeof(sockaddr_in)) < 0){
    cout << "connect error" << endl;
    return 1;
  }

  string str;
  char buf[1024];
  while(1){
    cout << "input msg:";
    getline(cin, str);
    if (str == "q" || str == "Q"){
      cout << "exit client" << endl;
      break;
    }
    int sendSz = send(sock, str.c_str(), str.size() + 1, 0);
    if (sendSz < 0){
      cout << "sendSz error" << endl;
      return 1;
    }

    int recvSz = recv(sock, buf, 1024, 0);
    if (recvSz < 0){
      cout << "recv error" << endl;
      return 1;
    }

    printf("recv msg: %s\n", buf);

  }
  close(sock);
  return 0;
}
