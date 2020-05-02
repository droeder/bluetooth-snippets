/*
  BlueZ example code to build an rfcomm client.
  This code just creates a socket and connects
  to a remote bluetooth device and sends a string.

  Programmed by Bastian Ballmann
  http://www.geektown.de

  Compile with gcc -lbluetooth <executable> <source>
*/

#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int main(int argc, char *argv[])
{
  int sock, d;
  struct sockaddr_rc laddr, raddr;
  struct hci_dev_info di;



  // if(argc < 4)
  //   {
  //     printf("%s <btaddr> <channel> <cmd>\n", argv[0]);
  //     exit(0);
  //   }
  
  
  //strcpy(str, "B8:27:EB:BE:F8:78");

  //char addr[] = "B8:27:EB:BE:F8:78";  //argv[1]

  char addr[] = "1A:31:72:70:4D:1C";  //argv[1]  
  int channel = 4; //atoi(argv[2])
  char cmd[] = "testCommand"; //argv[3]



  if(hci_devinfo(0, &di) < 0) 
    {
      perror("HCI device info failed");
      exit(1);
    }

  printf("Local device %s\n", batostr(&di.bdaddr));

  laddr.rc_family = AF_BLUETOOTH;
  laddr.rc_bdaddr = di.bdaddr;
  laddr.rc_channel = 0;

  raddr.rc_family = AF_BLUETOOTH;
  str2ba(addr,&raddr.rc_bdaddr);
  raddr.rc_channel = channel;  

  if( (sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) < 0)
    {
      perror("socket");
    }


  if( bind(sock, (struct sockaddr *)&laddr, sizeof(laddr)) < 0)
    {
      perror("bind");
      exit(1);
    }

  
  printf("Remote device %s\n", addr);
  
  if(connect(sock, (struct sockaddr *)&raddr, sizeof(raddr)) < 0)
    {
      perror("connect");
      exit(1);
    }
  
  printf("Connected.\nSending data %s\n",cmd);
  send(sock,argv[3],strlen(cmd),0);
  printf("Disconnect.\n");
  close(sock);
  return 0;
}

