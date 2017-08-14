#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CompressedImage.h>
#include <std_msgs/String.h>
#include <std_msgs/Int16.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/fill_image.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>

#define PORT 10654
#define SERVERADD "52.78.148.176"

class uvc_cam{
public:
  ros::NodeHandle nh;
  //ros::Rate r(10);

  sensor_msgs::Image img_;

  int framerate_;
  int image_height_;
  int image_width_;

  static int ClientSocket, ServerSocket;
  static int i;
  //FILE* in=fopen("/home/gunjun/imagetest.jpg", "rb");
  static unsigned char temp, check;

  uvc_cam();
  ~uvc_cam();
  void spinOnce();
  static void imageCallback(const sensor_msgs::CompressedImagePtr& msg);
  static void sendImgtoServer(const sensor_msgs::CompressedImagePtr& msg);
};

int uvc_cam::ClientSocket, uvc_cam::ServerSocket;
int uvc_cam::i;
unsigned char uvc_cam::temp, uvc_cam::check;

uvc_cam::uvc_cam(){
  image_transport::ImageTransport it(nh);


  ClientSocket=socket(PF_INET, SOCK_STREAM, 0);

  sockaddr_in client_addr;

  client_addr.sin_addr.s_addr=inet_addr(SERVERADD);
  client_addr.sin_family=AF_INET;
  client_addr.sin_port=htons(PORT);
  std::cout<<"connecting..."<<std::endl;
  if(connect(ClientSocket, (struct sockaddr*)&client_addr, sizeof(client_addr))==-1){
    std::cout<<"Cannot connect"<<std::endl;
    close(ClientSocket);
    return ;
  }
  std::cout<<"connecting complete!"<<std::endl;
  ros::Subscriber sub = nh.subscribe("/usb_cam/image_raw/compressed", 1, imageCallback);
  ros::spin();
  //r.sleep();
}

uvc_cam::~uvc_cam(){
  std::cout<<"close"<<std::endl;
  close(ClientSocket);
}

void uvc_cam::imageCallback(const sensor_msgs::CompressedImagePtr& msg){
  /*FILE *out=fopen("/home/gunjun/imagetest.jpg", "wb");
  fputc(msg->data[0], out);
  fputc(msg->data[1], out);
  unsigned char temp=0;
  int i=2;
  while(1){
    if(temp==0xff && msg->data[i]==0xd9){
      fputc(0xd9, out);
      break;
    }else{
      temp=(unsigned char)msg->data[i];
      fputc(msg->data[i++], out);
    }
  }
  fclose(out);*/

  sendImgtoServer(msg);

}

void uvc_cam::sendImgtoServer(const sensor_msgs::CompressedImagePtr& msg){


  /*send(ClientSocket, "Hi, Server!", 0xff, 0);
  recv(ClientSocket, recv_data, sizeof(recv_data), 0);

  std::cout<<"recv data : "<<recv_data<<std::endl;*/


  std::cout<<"start image transfer"<<std::endl;

  //std::cout<<"recv data : "<<motorx.data<<" "<<motory.data<<std::endl;
  //motorx_pub.publish(motorx);
  //motory_pub.publish(motory);
  i=0;
  while(1){


    temp=msg->data[i++];

    //  std::cout<<(int)temp<<std::endl;
    if(check==0xff&&temp==0xd9){
      std::cout<<"meet EOF"<<std::endl;
      send(ClientSocket, &temp, 0x1, 0);
      break;
    }
    //printf("i : %d\ttemp : %x\n", i, temp);
    check=temp;
    send(ClientSocket, &temp, 0x1, 0);
  }
  std::cout<<"end image transfer"<<std::endl;

  //fclose(in);

  return ;
}

int main(int argc, char **argv){
  ros::init(argc, argv, "uvc_cam_msg_subscriber");

  uvc_cam a;

  return 0;
}
