#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>

int main(){
  struct sockaddr_in server, client;
  int sock;
  // создание сокета
  if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
    printf("Error create!\n");
    return -1;
  }
  // структура для сервера
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY; // локальный адрес
  server.sin_port = htons((u_short)2015); // порт сервера
  // связка
  if(bind(sock, (struct sockaddr*)&server, sizeof(server)) == -1){
    printf("Error bind!\n");
    return -1;
  }
  printf("Адрес сервера: %s\n",inet_ntoa(server.sin_addr));
  // объявляем готовность к соединению
  if(listen(sock,5)==-1){
    printf("Error listen!\n");
    return -1;
  }
  while(1){
    int newsock, clnlen;
    if((newsock = accept(sock, (struct sockaddr*)&client, &clnlen)) == -1){
      printf("Error accept!\n");
      return -1;
    }
    printf("Новый клиент: %s\n",inet_ntoa(client.sin_addr));
    int pid = fork();
    if(pid == 0){
      close(sock);
      int size;
      char buf[255];
      while((size = recv(newsock, buf, sizeof(buf), 0)) != 0){ // пока получаем от клиента
        send(newsock, buf, sizeof(buf), 0); // отправляем эхо
      }
      close(newsock);
      return 0;
    }
    close(newsock);
  }
  return 0;
}
