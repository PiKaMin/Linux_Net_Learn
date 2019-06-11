#include <iostream>
#include <cstdio>
#include <sys/types.h>          
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include <sys/select.h>
#include <sys/time.h>
#include <thread>
#include "DataProtocol.h"

using std::cin;
using std::cout;
using std::string;
using std::endl;

// 处理send
int DeulEvent(int sock){
  DataHeader header = {0, TYPE_ERROR};
  int recvSz = recv(sock, &header, sizeof(DataHeader), MSG_PEEK);
  if (recvSz <= 0){
    cout << "server closed, exit" << endl;
    return -1;
  }
  switch(header.type){
  case TYPE_LOGIN_RESULT :{
                            DataLoginRes loginRes;
                            int recvSz = recv(sock, &loginRes, sizeof(DataLoginRes), 0);
                            printf("recv msg res: res = %d\n", loginRes.res);

                          }
                          break;
  case TYPE_LOGOUT_RESULT : {
                              DataLogoutRes logoutRes;
                              int recvSz = recv(sock, &logoutRes, sizeof(DataLogoutRes), 0);
                              printf("recv msg res: res = %d\n", logoutRes.res);
                            }
                            break;
  case TYPE_NEW_CLIENT : {
                           DataNewClient dataClient;
                           int recvSz = recv(sock, &dataClient, sizeof(DataNewClient), 0);
                           printf("New Client join:ip = %s, port=%d\n"
                                  , dataClient.ip, dataClient.port );

                         }
                         break;
  }

  return 0;
}

bool g_isRun = true;
// 开启一个线程处理键盘录入, 并recv
void InputThread(int sock){

  string str;
  cout << "input login or logout" << endl;
  cout << "input q or Q exit" << endl;
  while(1){
    cout << "input msg:";
    getline(cin, str);
    if (str == "q" || str == "Q"){
      cout << "exit client" << endl;
      g_isRun = false;
      return ;
    }
    else if (str == "login"){
      DataLogin login;
      strcpy(login.username, "yukina");
      strcpy(login.password, "yukina2333");
      send(sock, &login, sizeof(DataLogin), 0);
    }
    else if (str == "logout"){
      DataLogout logout;
      strcpy(logout.username, "yukina");
      send(sock, &logout, sizeof(DataLogout), 0);
    }
    else {
      cout << "input error" << endl;
    }
  }
}

int main(){
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0){
    cout << "socket error" << endl;
    return 1;
  }

  sockaddr_in servAddr;
  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(9080);
  servAddr.sin_addr.s_addr = inet_addr("192.168.79.140");
  if (connect(sock, (sockaddr*)&servAddr, sizeof(sockaddr_in)) < 0){
    cout << "connect error" << endl;
    return 1;
  }

  fd_set fdRead;
  FD_ZERO(&fdRead);
  std::thread th(InputThread, sock);
  th.detach();
  while(g_isRun){
    FD_SET(sock, &fdRead);
    timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    int ret = select(sock + 1, &fdRead, nullptr, nullptr, &timeout);
    // cout << "Do other things..." << endl;
    if (ret < 0){
      cout << "select error";
      break;
    }
    if (FD_ISSET(sock, &fdRead)){
      if(DeulEvent(sock) == -1 ){
        break;
      }
    }

  }
  close(sock);
  return 0;
}
