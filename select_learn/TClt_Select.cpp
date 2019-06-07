#include <iostream>
#include <cstdio>
#include <sys/types.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include "DataProtocol.h"

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
  cout << "input login or logout" << endl;
  cout << "input q or Q exit" << endl;
  while(1){
    cout << "input msg:";
    getline(cin, str);
    if (str == "q" || str == "Q"){
      cout << "exit client" << endl;
      break;
    }
    else if (str == "login"){
      DataLogin login;
      strcpy(login.username, "yukina");
      strcpy(login.password, "yukina2333");
      int sendSz = send(sock, &login, sizeof(DataLogin), 0);

      DataLoginRes loginRes;
      int recvSz = recv(sock, &loginRes, sizeof(DataLoginRes), 0);
      printf("recv msg res: res = %d\n", loginRes.res);
    }
    else if (str == "logout"){
      DataLogout logout;
      strcpy(logout.username, "yukina");
      int sendSz = send(sock, &logout, sizeof(DataLogout), 0);

      DataLogoutRes logoutRes;
      int recvSz = recv(sock, &logoutRes, sizeof(DataLogoutRes), 0);
      printf("recv msg res: res = %d\n", logoutRes.res);

    }
    else {
      cout << "input error" << endl;
      continue;
    }
  }
  close(sock);
  return 0;
}
