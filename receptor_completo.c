#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

int main(int argc, char **argv)
{
	int s, i; 
	int nbytes;
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;
	int data1;
	unsigned int data1_msb, data1_lsb;

	printf("CAN Sockets Receive Demo\r\n");

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Socket");
		return 1;
	}

	strcpy(ifr.ifr_name, "can1" );
	ioctl(s, SIOCGIFINDEX, &ifr);

	while(1){	
	
	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind");
		return 1;
	}

	nbytes = read(s, &frame, sizeof(struct can_frame));

 	if (nbytes < 0) {
		perror("Read");
		return 1;
	}

	printf("0x%03X [%d] ",frame.can_id, frame.can_dlc);

	for (i = 0; i < frame.can_dlc; i++)        //printa os 8 bytes da mensagem no barramento
		printf("%02X ",frame.data[i]);
		
		
	data1 = ((frame.data[0] << 8) & 0xFF00) | (frame.data[1] & 0xFF); //concatenando 2 bytes. Data1 é um inteiro
	printf("Data1: %0x\n",data1);
	printf("Data1: %d\n",data1);
		
	printf("\r\n");
	}
		
	
	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

	return 0;
}
