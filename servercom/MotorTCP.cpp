
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

int sleep(int wait){
  int goal;
  goal = wait + clock();

  while(goal > clock()){;}

  return 0;
}

class TCP_Server{
public:
  FILE* fp;
  struct sockaddr_in server_addr, client_addr;
  int option, ServerSocket, ClientSocket, len;
  char temp[20];
  
  TCP_Server(int argc, char **argv);
  ~TCP_Server();
};

TCP_Server::TCP_Server(int argc, char **argv){
  /*if(argc!=2){
    std::cout<<"./filename Port"<<std::endl;
    exit(0);
  }*/
  
  if((ServerSocket = socket(PF_INET, SOCK_STREAM, 0))==-1){
    std::cout<<"Server cannot open socket!"<<std::endl;
    exit(0);
  }
  option=1;
  setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
  
  memset(&server_addr, 0x00, sizeof(server_addr));
  
  server_addr.sin_family=AF_INET;
  server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  server_addr.sin_port=htons(10663);
  
  if(bind(ServerSocket, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
    std::cout<<"Server cannot bind local address"<<std::endl;
    exit(0);

  
  if(listen(ServerSocket, 10)<0){
    std::cout<<"Server cannot listen connect"<<std::endl;
    exit(0);
  }
  
  len=sizeof(client_addr);
  std::cout<<"===== [PORT] : "<<10663<<" ====="<<std::endl;
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
    fp=fopen("/home/ubuntu/AtlasCon/mouse", "r");
    float motormodex, motormodey;
    float open;
    int isClientSocketOpen;
    fscanf(fp, "%f %f %f", &motormodex, &motormodey, &open);
    std::cout<<motormodex<<" "<<motormodey<<" "<<open<<std::endl;
    int send_ch1, send_ch2, send_ch3, recv_ch;
    send_ch1=send(ClientSocket, &motormodex, 0x4, MSG_NOSIGNAL);
    send_ch2=send(ClientSocket, &motormodey, 0x4, MSG_NOSIGNAL);
    send_ch3=send(ClientSocket, &open, 0x4, MSG_NOSIGNAL);
    recv_ch=recv(ClientSocket, &isClientSocketOpen, 0x4, 0);
    std::cout<<send_ch1<<" "<<send_ch2<<" "<<send_ch3<<" "<<recv_ch<<std::endl;
    if(send_ch1==-1) return;
    if(send_ch2==-1) return;
    if(send_ch3==-1) return;
    if(recv_ch==0) return;
    sleep(50000);

    fclose(fp);
  }
}

TCP_Server::~TCP_Server(){
  close(ClientSocket);
  close(ServerSocket);
  fclose(fp);
}

int main(int argc, char** argv){
 
  while(1){
    TCP_Server tcp_server(argc, argv);
  }
  
  
  
  
  return 0;
}


