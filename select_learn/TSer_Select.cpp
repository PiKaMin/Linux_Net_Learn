#include <iostream>
#include <cstdio>
#include <sys/types.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

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

  char buf[1024] = {};
  while(1){
    int recvSz = recv(clntSock, buf, 1024, 0);
    if (recvSz < 0){
      cout << "recv error" << endl;
      return 1;
    }
    else if (recvSz == 0){
      cout << "client closed" << endl;
      close(clntSock);
      break;
    }
    printf("recv msg: %s\n", buf);

    send(clntSock, buf, recvSz, 0);
  }


  close(servSock);
  return 0;
}
