#include "tcp_client.hpp"
#include "Selector.hpp"
#include <vector>

using std::vector;

int main(){
  string ip = "192.168.79.140";
  TcpClient client(ip, 2233);
  Selector selector;
  client.Connect();

  selector.Add(client.GetTcpSocket());
  vector<int> readyClient;
  while(true){
    int ret = selector.WaitUnBlocking(readyClient, 2, 0);
    cout << "Do other things" << endl;
    if (ret == false){
      break;
    }
    if (!readyClient.empty()){
      
    }
  }
  return 0;
}
