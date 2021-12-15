#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <ifaddrs.h>

#include <unistd.h>

#define ADDR argv[1]
#define PORT argv[2]
#define GROUP argv[3]
#define MSG argv[4]
#define BUFF_SIZE 2000

int main(int argc, char *argv[]){
  struct sockaddr_in addr;
  int fd, read_size;
  char *buff[BUFF_SIZE];

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
      perror("socket");
      exit(1);
  }

  // Default TTL
  setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &(unsigned char){1}, sizeof(unsigned char));
  // Define network interface for send datagrams
  setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &(in_addr_t){inet_addr(ADDR)}, sizeof(in_addr_t));

  memset(&addr,0,sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(GROUP);
  addr.sin_port=htons(atoi(PORT));
  if(sendto(fd, MSG, strlen(MSG), 0, (struct sockaddr*)&addr, sizeof(addr)) < 0){
    perror("sendto");
    return 1;
  }

  int addr_len = sizeof(addr);
  read_size = recvfrom(fd, buff, BUFF_SIZE - 1, MSG_WAITALL, (struct sockaddr*)&addr, &addr_len);
  buff[read_size] = "\0";
  printf("SERVER MESSAGE: %s\n", buff);
}
