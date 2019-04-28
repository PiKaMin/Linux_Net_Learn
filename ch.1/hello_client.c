#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char **argv){
  
  int sock;
  struct sockaddr_in serv_addr;
  char message[30];
  int str_len;

  if (argc != 3){
    printf("Usage : %s <IP> <Port>", argv[0]);
    return 1;
  }

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1){
    perror("socket() error");
    return 1;
  }

  memset(&serv_addr, 0, sizeof serv_addr);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (struct sockaddr* )&serv_addr, sizeof serv_addr) == -1){
    perror("connect() error");
    return 1;
  }

  str_len = read(sock, message, sizeof(message) - 1);
  if (str_len == -1){
    perror("read() error");
    return 1;
  }
  printf("Message from server: %s\n", message);
  printf("Receive size : %d\n", str_len);
  close(sock);

  return 0;
}

