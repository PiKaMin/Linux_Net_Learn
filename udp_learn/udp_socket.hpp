// 实现以UdpSocket类封装udp常用操作
#include <iostream>
#include <sys/types.h>        
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

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
    memset(&addr, 0, sizeof addr);
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

  // str: 发送的字符串 | ip: 对端ip | port: 对端端口号
  bool Send(std::string& str, std::string& ip, uint16_t port){

    sockaddr_in dstAddr;
    memset(&dstAddr, 0, sizeof dstAddr);
    dstAddr.sin_family = AF_INET;
    dstAddr.sin_port = htons(port);
    dstAddr.sin_addr.s_addr = inet_addr(ip.c_str());

    int ret = sendto(_sock, str.c_str(), str.size(), 0, (sockaddr*)&dstAddr, sizeof(dstAddr));

    if (ret == -1){
      cout << "Send() error" << endl;
      return false;
    }
    return true;

  }

  // str: 接收到的字符串 | ip: 传出对端ip | port: 可传出对端端口
  bool Recv(std::string& str, std::string* ip = nullptr, uint16_t* port = nullptr){
    sockaddr_in dstAddr;
    socklen_t len = sizeof(dstAddr);
    char buf[1024] = {0};
    int recvSize = recvfrom(_sock, buf, sizeof buf - 1, 0, (sockaddr*)&dstAddr, &len);
    if (recvSize == -1) {
      cout << "recv() error" << endl;
      return false;
    }


    str.assign(buf, recvSize);
    if (ip != nullptr){
      *ip = inet_ntoa(dstAddr.sin_addr);
    }
    if (port != nullptr){
      *port = ntohs(dstAddr.sin_port);
    }
    return true;
  }

  bool Close(){
    if(close(_sock) == -1){
      return false;
    }
    _sock = -1;
    return true;
  }

private:
  int _sock;
};

