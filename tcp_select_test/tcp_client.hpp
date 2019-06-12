#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include "Selector.hpp"
#include "tcp_socket.hpp"
#include <iostream>


class TcpClient{
public:
  TcpClient(const string& ip, const uint16_t port) : _ip(ip), _port(port){
    _sock.Socket();
  }

  ~TcpClient(){
    _sock.Close();
  }
public:
  bool Connect(){
    return _sock.Connect(_ip, _port);
  }

  bool Recv(string& buf){
    return _sock.Recv(buf);
  }
  bool Send(string& msg){
    return _sock.Send(msg);
  }
  TcpSocket GetTcpSocket(){
    return _sock;
  }
    

private:
  TcpSocket _sock;
  string _ip;
  uint16_t _port;
};


#endif
