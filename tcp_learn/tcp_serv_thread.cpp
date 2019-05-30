#include <iostream>
#include <string>
#include <pthread.h>
#include "tcp_socket.hpp"

using std::cout;
using std::cin;
using std::string;
using std::endl;

void* thread_run(void* arg){
  TcpSocket* newSock = (TcpSocket* )arg;
  for(;;){
    string msg;
    if (!newSock->Recv(msg)){
      newSock->Close();
      break;
    }
    cout << "recv: " << msg << endl;

    newSock->Send(msg);
  }
  cout << "A client quit" << endl;
  return NULL;
}

int main(){
  TcpSocket tcp;

  tcp.Socket();
  string ip = "192.168.30.145";
  tcp.Bind(ip, 14396);
  tcp.Listen(5);


  while(1) {
    TcpSocket* newSock = new TcpSocket;
    string clntIp;
    uint16_t clntPort;
    tcp.Accept(*newSock, &clntIp, &clntPort);
    cout << "Connect: (" << clntIp << "--" << clntPort << ")" << endl;

    pthread_t tid;
    pthread_create(&tid, NULL, thread_run, (void* )newSock);
    pthread_detach(tid);

  }

  tcp.Close();
  return 0;
}
