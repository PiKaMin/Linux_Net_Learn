// 实现以UdpSocket类封装udp常用操作
#include <iostream>
#include <sys/types.h>        
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>


using std::cout;
using std::endl;

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

class UdpSocket{
public:

  UdpSocket() : _sock(-1){
    
  }
  ~UdpSocket(){}

  bool Socket(){
    _sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (_sock == -1){
      cout << "socket() error" << endl;
      return false;
    }
    return true;
  }

  bool Bind(std::string& ip, uint16_t port){
    sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip.c_str());
    
    int ret = bind(_sock, (sockaddr* )&addr, sizeof(addr));
    if (ret == -1){
      cout << "bind() error" << endl;
      return false;
    }

    return true;

  }

  bool Send(){
  
  }

  bool Recv(){
  
  }
private:
  int _sock;
};

