#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <sys/ioctl.h>


int main(int argc, char **argv)
{


   int ctl = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
   // Open HCI socket
   if (ctl < 0) {
     // Can't open HCI socket
   }

    struct hci_dev_req dr;
    dr.dev_id = 0;  // hci0
    dr.dev_opt = SCAN_DISABLED;
    dr.dev_opt = SCAN_PAGE | SCAN_INQUIRY;

    if (ioctl(ctl, HCISETSCAN, (unsigned long)&dr) < 0) {
      printf("Can't set scan mode");
    }
    else {
       printf("hci0 is now discoverable");
    }
    close(ctl);


    printf("test\n");
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    printf("Socket allocated\n");
    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    printf("listening\n");
    // put socket into listening mode
    listen(s, 1);

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));
    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
    }

    // close connection
    close(client);
    close(s);
    return 0;
}
