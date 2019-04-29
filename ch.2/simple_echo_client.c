#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
int main(int argc, char** argv){
  int sock;
  char message[BUF_SIZE];
  struct sockaddr_in serv_adr;

  if (argc != 3){
    printf("Usage : %s <ip> <port>", argv[0]);
    return 1;
  }

  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1){
    perror("socket() error");
    return 1;
  }

  memset(&serv_adr, 0, sizeof serv_adr);
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_port = htons(atoi(argv[2]));
  serv_adr.sin_addr.s_addr = inet_addr(argv[1]);

  if (connect(sock, (struct sockaddr* )&serv_adr, sizeof serv_adr) == -1){
    perror("connect() error");
    return 1;
  }else {
    puts("conneted........");
  }

  while(1){
    fputs("Input message(Q to quit):", stdout);
    fgets(message, BUF_SIZE, stdin);

    if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")){
      break;
    }
    
    int str_len = strlen(message);
    message[str_len - 1] = '\0';
    printf("Send size: %d\n", str_len);
    write(sock, message, str_len);
    /* str_len = read(sock, message, BUF_SIZE); */
    int recv_len = 0;
    while (recv_len < str_len){
      int recv_sz = read(sock, &message[recv_len], BUF_SIZE);
      if (recv_sz == -1){
        perror("read() error");
        return 1;
      }
      
      recv_len += recv_sz;

    }
    printf("Message: %s\n", message);
    printf("Receive size : %d\n", recv_len);
  }

  close(sock);
  return 0;
}
