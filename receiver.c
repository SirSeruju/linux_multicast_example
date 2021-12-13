#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>


#define PORT 12345
#define GROUP "224.1.1.5"
#define BUFF_SIZE 2000

int main(int argc, char *argv[]){
  struct sockaddr_in addr, caddr;
  int sd, read_size;
  char buff[BUFF_SIZE];

  if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
      perror("socket");
      exit(1);
  }

  /* set up destination address */
  memset(&addr,0,sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port=htons(PORT);

  struct ip_mreq group = {};
  group.imr_multiaddr.s_addr = inet_addr(GROUP);
  group.imr_interface.s_addr = INADDR_ANY;
  setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&group, sizeof(group));


  if(bind(sd, (struct sockaddr*)&addr, sizeof(addr)) < 0){
		perror("bind");
		return 1;
	}
  
  int c = sizeof(caddr);
  while(1){
    read_size = recvfrom(sd, buff, BUFF_SIZE - 1, MSG_WAITALL, (struct sockaddr*) &caddr, &c);
    buff[read_size] = '\0';
    printf("%s\n", inet_ntoa(caddr.sin_addr));
    printf("MESG: %s\n", buff);
  }
}
