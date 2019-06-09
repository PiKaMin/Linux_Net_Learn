#include <iostream>
#include <cstdio>
#include <sys/types.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "DataProtocol.h"
#include <sys/select.h>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

int DeulEvent(int clntSock){
  DataHeader header = {0, TYPE_ERROR};
  int recvSz = recv(clntSock, &header, sizeof(DataHeader), MSG_PEEK);
  if (recvSz <= 0){
    cout << "client closed, exit" << endl;
    close(clntSock);
    return -1;
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
  return 0;
}

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

  int maxSock = servSock;
  fd_set fdRead;
  fd_set copyRead;
  FD_ZERO(&fdRead);
  FD_ZERO(&copyRead);

  FD_SET(servSock, &fdRead);
  while(1){
    copyRead = fdRead;


    int selectRet = select(maxSock + 1, &copyRead, NULL, NULL, NULL);
    if (selectRet < 0){
      cout << "select error" << endl;
      break;
    }

    if (FD_ISSET(servSock, &copyRead)){
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

      FD_SET(clntSock, &fdRead);
      if (clntSock > maxSock){
        maxSock = clntSock;
      }
    }

    for (int i = 0; i < maxSock + 1; i++){
      if (i != servSock && FD_ISSET(i, &copyRead)){
        int ret = DeulEvent(i);
        if (ret == -1){
          FD_CLR(i, &fdRead);
        }
      }

    }

  }

  close(servSock);
  return 0;
}
