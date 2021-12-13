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

int main(int argc, char *argv[]){
  struct sockaddr_in addr;
  int fd;
  char *msg = "Hello, I'm sender!";

  if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
      perror("socket");
      exit(1);
  }

  /* set up destination address */
  memset(&addr,0,sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(GROUP);
  addr.sin_port=htons(PORT);

  while (1) {
    if(sendto(fd, msg, strlen(msg), 0, (struct sockaddr*)&addr, sizeof(addr)) < 0){
        perror("sendto");
        return 1;
    }
    sleep(3);
  }
}
