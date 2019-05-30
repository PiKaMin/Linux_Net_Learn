#include <iostream>
#include <string>
#include "tcp_socket.hpp"
#include <signal.h>
#include <sys/wait.h>

using std::cout;
using std::cin;
using std::string;
using std::endl;
void mychild(int  sig){
  while(waitpid(-1, NULL, WNOHANG) > 0);
}


int main(){
  TcpSocket tcp;

  tcp.Socket();
  string ip = "192.168.30.145";
  tcp.Bind(ip, 14396);
  tcp.Listen(5);

  signal(SIGCHLD, mychild);
  while(1) {
    TcpSocket newSock;
    string clntIp;
    uint16_t clntPort;
    tcp.Accept(newSock, &clntIp, &clntPort);
    cout << "Connect: (" << clntIp << "--" << clntPort << ")" << endl;

    pid_t pid = fork();
    if (pid < 0){
      cout << "fork error" << endl;
    }
    else if (pid == 0){
      for(;;){
        string msg;
        if (!newSock.Recv(msg)){
          newSock.Close();
          break;
        }
        cout << "recv: " << msg << endl;

        newSock.Send(msg);
      }

    } 
    else {
      newSock.Close();
    }


  }

  tcp.Close();
  return 0;
}
