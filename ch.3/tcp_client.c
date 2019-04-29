#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define LEN_SZ 4
#define BUF_SIZE 1024
int main(int argc, char** argv){
  int sock;
  struct sockaddr_in serv_adr;
  char message[BUF_SIZE] = {0};
  char str[BUF_SIZE] = {0};
  if (argc != 3){
    printf("Usage : %s <ip> <port>", argv[0]);
    return 1;
  }

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1){
    perror("socket() error");
    return 1;
  }

  memset(&serv_adr, 0 , sizeof serv_adr);
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_port = htons(atoi(argv[2]));
  serv_adr.sin_addr.s_addr =inet_addr(argv[1]);

  if (connect(sock, (struct sockaddr* )&serv_adr, sizeof serv_adr) == -1){
    perror("connect() error");
    return 1;
  }


  fputs("Input Message: ", stdout);
  fgets(str, sizeof message, stdin);
  int len = strlen(str);
  str[len - 1] = '\0';

  *((int *)&message[0]) = len - 1;
  
  for (int i = 0, j = LEN_SZ; i < len; i++, j++){
    message[j] = str[i];
  }
  
  write(sock, message, LEN_SZ + len);
  

  close(sock);
  return 0;
}
