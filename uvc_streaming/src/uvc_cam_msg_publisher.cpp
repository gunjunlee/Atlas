#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>

#define PORT 10663
#define SERVERADD "52.78.148.176"

int sleep(int wait){
  int goal;
  goal = wait + clock();
  while(goal > clock()){;}

  return 0;
}

class TCP_Client{
public:
  int ClientSocket, isClientSocketOpen;

  TCP_Client(int argc, char** argv);
  ~TCP_Client();
};

TCP_Client::TCP_Client(int argc, char** argv){

  ros::init(argc, argv, "uvc_cam_msg_publisher");

  ros::NodeHandle nh;

  std_msgs::Float32 motor_x;
  std_msgs::Float32 motor_y;
  std_msgs::Float32 open;

  ros::Publisher motorx_pub=nh.advertise<std_msgs::Float32>("motor_x", 10);
  ros::Publisher motory_pub=nh.advertise<std_msgs::Float32>("motor_y", 10);
  ros::Publisher open_pub=nh.advertise<std_msgs::Float32>("open", 10);


  ClientSocket=socket(PF_INET, SOCK_STREAM, 0);

  sockaddr_in client_addr;
  isClientSocketOpen=1;

  client_addr.sin_addr.s_addr=inet_addr(SERVERADD);
  client_addr.sin_family=AF_INET;
  client_addr.sin_port=htons(PORT);
  std::cout<<"connecting..."<<std::endl;
  if(connect(ClientSocket, (struct sockaddr*)&client_addr, sizeof(client_addr))==-1){
    std::cout<<"Cannot connect"<<std::endl;
    close(ClientSocket);
    return;
  }
  std::cout<<"connecting complete!"<<std::endl;


  while(1){

    std::cout<<"recv1: "<<recv(ClientSocket, &motor_x.data, 0x4, 0);
    std::cout<<"\trecv2: "<<recv(ClientSocket, &motor_y.data, 0x4, 0);
    std::cout<<"\trecv3: "<<recv(ClientSocket, &open.data, 0x4, 0);
    std::cout<<"\tsend: "<<send(ClientSocket, &isClientSocketOpen, 0x4, 0)<<std::endl;
    std::cout<<motor_x.data<<" "<<motor_y.data<<" "<<open.data<<std::endl;
    motorx_pub.publish(motor_x);
    motory_pub.publish(motor_y);
    open_pub.publish(open);
  }

}

TCP_Client::~TCP_Client(){

  close(ClientSocket);

}

int main(int argc, char** argv){

  TCP_Client tcp_client(argc, argv);

  return 0;
}
