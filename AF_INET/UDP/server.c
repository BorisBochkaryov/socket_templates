#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(){
    int sock;
    struct sockaddr_in addr,cliaddr;
    char buf[1024];
    int bytes_read;

    if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
      perror("socket");
      return -1;
    }
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2015);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
      printf("Error bind!\n");
      return -1;
    }
    
    while(1){
      int clilen = sizeof(cliaddr);
      bytes_read = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&cliaddr, &clilen); // прием от клиентов
      printf("%s\n",buf);
      sendto(sock, buf, sizeof(buf), 0, (struct sockaddr*)&cliaddr, clilen); // отправка обратно этому же клиенту
    }
    
    close(sock);
    return 0;
}
