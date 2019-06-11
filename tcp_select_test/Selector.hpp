#ifndef SELECTOR_HPP
#define SELECTOR_HPP

#include "tcp_socket.hpp"
#include <sys/select.h>
#include <sys/time.h>
#include <unordered_map>
#include <cassert>
#include <vector>

class Selector{
public:
  Selector(){
    _maxFd = 0;
    FD_ZERO(&_fdRead);
  }
  bool Add(const TcpSocket& sock){
    int fd = sock.GetFd();
    if (_fdMap.find(fd) != _fdMap.end()){
      cout << "Add Failed! sock has in Selector" << endl;
      return false;
    }
    _fdMap[fd] = sock;
    FD_SET(fd, &_fdRead);
    if (fd > _maxFd){
      _maxFd = fd;
    }
    return true;
  }

  bool Del(const TcpSocket& sock){
    int fd = sock.GetFd();
    if (_fdMap.find(fd) == _fdMap.end()){
      cout << "Del Failed! No sock in Selector" << endl;
      return false;
    }
    _fdMap.erase(fd);
    FD_CLR(fd, &_fdRead);
    return true;
  }
  bool WaitBlocking(std::vector<int>& readyFds){
    return Wait(readyFds);
  }
  bool WaitUnBlocking(std::vector<int>& readyFds, int sec, int usec){
    timeval timeout;
    timeout.tv_sec = sec;
    timeout.tv_usec = usec;
    return Wait(readyFds, &timeout);
  }
private:
  bool Wait(std::vector<int>& readyFds, timeval* timeout = nullptr){
    readyFds.clear();
    fd_set tmpFd = _fdRead;
    int ret = select(_maxFd + 1, &tmpFd, nullptr, nullptr, timeout);
    if (ret < 0){
      cout << "Wait (select) error" << endl;
      return false;
    }

    for (int i = 0; i < _maxFd + 1; i++){
      if (!FD_ISSET(i, &tmpFd)){
        continue;
      }
      readyFds.push_back(i);
    }

    return true;
  }
private:
  int _maxFd;
  fd_set _fdRead;
  std::unordered_map<int, TcpSocket> _fdMap;
};

#endif
