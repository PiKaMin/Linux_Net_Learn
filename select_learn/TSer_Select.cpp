#include <iostream>
#include <cstdio>
#include <sys/types.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "DataProtocol.h"

using std::cout;
using std::endl;

int main(){
  int servSock = socket(AF_INET, SOCK_STREAM, 0);
  if (servSock < 0){
    cout << "socket error" << endl;
    return 1;
  }

  sockaddr_in sockAddr = {};
  sockAddr.sin_family = AF_INET;
  sockAddr.sin_addr.s_addr = INADDR_ANY;
  sockAddr.sin_port = htons(9080);
  if (bind(servSock, (sockaddr*)&sockAddr, sizeof(sockaddr_in)) < 0){
    cout << "bind error"; 
    return 1;
  }

  if (listen(servSock, 5) < 0){
    cout << "listen error" <<  endl;
    return 1;
  }

  int clntSock;
  sockaddr_in clntAddr = {};
  socklen_t clntLen = sizeof(clntAddr);
  clntSock = accept(servSock, (sockaddr*)&clntAddr
                    ,&clntLen);
  if (clntSock < 0){
    cout << "accept error" << endl;
    return 1;
  }

  printf("Connect: Ip = %s, Port = %d\n"
         ,inet_ntoa(clntAddr.sin_addr)
         , ntohs(clntAddr.sin_port));

  while(1){
    DataHeader header = {0, TYPE_ERROR};
    int recvSz = recv(clntSock, &header, sizeof(DataHeader), MSG_PEEK);
    if (recvSz <= 0){
      cout << "client closed, exit" << endl;
      close(clntSock);
      break;
    }
    switch(header.type){
    case TYPE_LOGIN :{
                       DataLogin login;
                       int recvSz = recv(clntSock, &login, sizeof(DataLogin), 0);
                       printf("recv msg: username = %s, password = %s\n"
                              ,login.username, login.password );

                       DataLoginRes loginRes;
                       send(clntSock, &loginRes, sizeof(DataLoginRes), 0);
                     }
                     break;
    case TYPE_LOGOUT : {
                         DataLogout logout;
                         int recvSz = recv(clntSock, &logout, sizeof(DataLogout), 0);
                         printf("username = %s logout\n", logout.username);

                         DataLogoutRes logoutRes;
                         send(clntSock, &logoutRes, sizeof(DataLogoutRes), 0);
                       }
                       break;
    }
  }


  close(servSock);
  return 0;
}
