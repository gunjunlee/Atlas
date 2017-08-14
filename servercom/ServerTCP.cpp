
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <cstdlib>

class Temp{
public:
  int Serversock;
  Temp(int sock){
    Serversock=sock;
  }
  ~Temp(){
    close(Serversock);
  }
};


int main(int argc, char** argv){
  //system("pwd");
  unsigned char buffer, check;
  struct sockaddr_in server_addr, client_addr;
  char temp[20];
  int ServerSocket, ClientSocket;
  int len, msg_size;
  char test[20];
  int option;
  FILE* out;
  ex1:
  //if(argc!=2){
  //  std::cout<<"./filename Port"<<std::endl;
  //  exit(0);
  //}
  
  if((ServerSocket = socket(AF_INET, SOCK_STREAM, 0))==-1){
    std::cout<<"Server cannot open socket!"<<std::endl;
    exit(0);
  }
  
  option=1;
  setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  memset(&server_addr, 0x00, sizeof(server_addr));
  
  server_addr.sin_family=AF_INET;
  server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  server_addr.sin_port=htons(10654);
  
  
  if(bind(ServerSocket, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
    std::cout<<"Server cannot bind local address"<<std::endl;
    exit(0);
  }

  Temp temptemp(ServerSocket);

  if(listen(ServerSocket, 10)<0){
    std::cout<<"Server cannot listen connect"<<std::endl;
    exit(0);
  }
  
  //memset(buffer, 0x00, sizeof(buffer));
  len=sizeof(client_addr);
  std::cout<<"===== [PORT] : "<<10654<<" ====="<<std::endl;
  std::cout<<"Server waits connection request"<<std::endl;
  int count=0;
  ClientSocket=accept(ServerSocket, (struct sockaddr*)&client_addr, (socklen_t *)&len);
    
    if(ClientSocket<0){
      std::cout<<"Server cannot accept failed"<<std::endl;
      exit(0);
    }
    
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp, sizeof(temp));
    std::cout<<"Server:"<<temp<<"client connet"<<std::endl;
    
  while(1){
    out=fopen("/var/www/html/temp", "wb");
    std::cout<<"Here!"<<std::endl;
    
    std::cout<<"Image receive start"<<std::endl;
    while(1){
          //msg_size=recv(ClientSocket, &buffer, 1, 0);
          if(recv(ClientSocket, &buffer, 0x1, 0)==0){
            fclose(out);
            close(ClientSocket);
            close(ServerSocket);
            goto ex1;
            std::cout<<"Client die"<<std::endl;
          }
          //printf("buffer: %x\n", buffer);
          fputc(buffer, out);
          //break;
          if(check==0xff&&buffer==0xd9) break;

          check=buffer;

          //send(ClientSocket, buffer, msg_size, 0);
          //close(ClientSocket);
          //std::cout<<"Server: "<<temp<<" client closed"<<std::endl;
          //std::cout<<"Buffer: "<<buffer<<std::endl;
    }
    std::cout<<"Image receive end"<<std::endl;
    fclose(out);
    system("mv -f /var/www/html/temp /var/www/html/TCPimage.jpg");

  
    close(ClientSocket);
  close(ServerSocket);
  
  return 0;
}









