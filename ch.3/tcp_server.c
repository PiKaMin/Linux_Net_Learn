#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
int main(int argc, char** argv){

  int serv_sock, clnt_sock;
  char message[BUF_SIZE];
  struct sockaddr_in serv_adr, clnt_adr;
  socklen_t clnt_adr_sz;

  if (argc != 2){
    printf("Usage: %s <port>", argv[0]);
    return 1;
  }

  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  if (serv_sock == -1){
    perror("socket() error");
    return 1;
  }

  memset(&serv_adr, 0, sizeof serv_adr);
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_port = htons(atoi(argv[1]));
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(serv_sock, (struct sockaddr* )&serv_adr
           , sizeof serv_adr) == -1){
    perror("bind() error");
    return 1;
  }

  if (listen(serv_sock, 5) == -1){
    perror("listen() error");
    return 1;
  }

  clnt_adr_sz = sizeof clnt_adr;

  clnt_sock = accept(serv_sock, (struct sockaddr* )&clnt_adr, &clnt_adr_sz);
  
  int str_len = 0;
  read(clnt_sock, &str_len, 4);

  printf("Read len : %d\n", str_len);
  int recv_len = 0;
  while(recv_len < str_len){
    int recv_sz = read(clnt_sock, &message, BUF_SIZE - 4);
    recv_len += recv_sz;
  }

  printf("Read Message: %s\n", message);

  return 0;
}

