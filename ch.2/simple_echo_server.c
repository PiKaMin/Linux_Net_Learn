#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
int main(int argc, char** argv){
  int serv_sock, clnt_sock;
  char message[BUF_SIZE];
  
  struct sockaddr_in serv_adr, clnt_adr;
  socklen_t clnt_adr_sz;

  if (argc != 2){
    printf("Usage : %s <port>", argv[0]);
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

  if (bind(serv_sock, (struct sockaddr* )&serv_adr, sizeof serv_adr) == -1){
    perror("bind() error");
    return 1;
  }

  if (listen(serv_sock, 5) == -1){
    perror("listen() error");
    return 1;
  }

  clnt_adr_sz = sizeof clnt_adr;

  int str_len;
  for (int i = 0; i < 5; i++){
    clnt_sock = accept(serv_sock, (struct sockaddr* )&clnt_adr, &clnt_adr_sz);
    if (clnt_sock == -1){
      perror("accept() error");
      return 1;
    }else{
      printf("connected client: %d\n", i + 1);
    }

    while( (str_len = read(clnt_sock, message, BUF_SIZE)) != 0){
      printf("Receive size: %d\n", str_len);
      write(clnt_sock, message, str_len);
    }

    close(clnt_sock);

  }
  close(serv_sock);
  return 0;
}
