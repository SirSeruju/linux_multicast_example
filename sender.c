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


#define GROUP argv[1]
#define PORT atoi(argv[2])
#define msg argv[3]

int main(int argc, char *argv[]){
  struct sockaddr_in addr;
  int fd;

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
      perror("socket");
      exit(1);
  }

  // Default TTL
  setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &(unsigned char){1}, sizeof(unsigned char));

  struct ip_mreqn group;
  struct ifaddrs* addrs;
  getifaddrs(&addrs);
  struct ifaddrs* tmp = addrs;
  while(tmp){
    if(tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET){
      struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
      memset(&addr,0,sizeof(addr));
      addr.sin_family = AF_INET;
      addr.sin_addr.s_addr = inet_addr(inet_ntoa(pAddr->sin_addr));
      addr.sin_port=htons(PORT);
      if(sendto(fd, msg, strlen(msg), 0, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        perror("sendto");
        return 1;
      }
    }
    tmp = tmp->ifa_next;
  }
  freeifaddrs(addrs);
}
