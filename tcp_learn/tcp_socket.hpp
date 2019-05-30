#include <iostream>
#include <sys/types.h>          
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>

using std::string;
using std::cout;
using std::endl;

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;

class TcpSocket{
public:
  TcpSocket():_fd(-1){}
  ~TcpSocket(){
  }
public:
  bool Socket(){
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1){
      cout << "Socket err" << endl;
      return false;
    }
    return true;
  }

  bool Close() {
    close(_fd);
    cout << "close fd: " << _fd << endl;
    return true;
  }
  bool Bind(string& ip, uint16_t port){
    sockaddr_in bindAddr;
    bindAddr.sin_family = AF_INET;
    bindAddr.sin_port = htons(port);
    bindAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (bind(_fd, (sockaddr*)&bindAddr, sizeof bindAddr) == -1){
      cout << "Bind err" << endl;
      return false;
    }
    return true;
  }

  bool Listen(int num){
    if (listen(_fd, num) == -1){
      cout << "listen err" << endl;
      return false;
    }
    return true;
  }

  bool Accept(TcpSocket& newSock, string* ip = nullptr, uint16_t* port = nullptr){
    sockaddr_in peerAddr;
    socklen_t sockLen = sizeof peerAddr;
    int newFd = accept(_fd, (sockaddr*)&peerAddr, &sockLen);
    if (newFd < 0){
      cout << "accept err" << endl;
      return false;
    }
    newSock.SetFd(newFd);
    
    if (ip != nullptr){
        *ip = inet_ntoa(peerAddr.sin_addr);
    }

    if (port != nullptr) {
      *port = ntohs(peerAddr.sin_port);
    }

    return true;
  }

  bool Connect(string& ip, uint16_t port){
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    servAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (connect(_fd, (sockaddr*)&servAddr, sizeof servAddr) == -1){
      cout << "Connect() error" << endl;
      return false;
    }

    return true;
  }

  bool Recv(string& msg){
    char buf[4096] = {0};
    int recvSize = recv(_fd, buf, 4096, 0);
    if (recvSize < 0){
      cout << "recv() err" << endl;
      return false;
    }

    if (recvSize == 0){
      return false;
    }

    msg.assign(buf, recvSize);
    return true;
  }

  bool Send(string& msg){
    int ret = send(_fd, msg.c_str(), msg.size(), 0);
    
    if (ret < 0){
      cout << "send() err" << endl;
      return false;
    }

    return true;
  }

public:
  void SetFd(int fd){
    _fd = fd;
  }

  int GetFd(){
    return _fd;
  }
private:
  int _fd;
};

