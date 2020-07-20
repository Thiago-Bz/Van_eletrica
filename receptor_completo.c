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

	for (i = 0; i < frame.can_dlc; i++)
		printf("%02X ",frame.data[i]);
		
	/*unsigned base = 10;
    	while(frame.data[1] >= base)
        base *= 10;
    	data1_msb = frame.data[0] * base + frame.data[1];
		
	while(frame.data[3] >= base)
        base *= 10;
    	data1_lsb = frame.data[2] * base + frame.data[3];
	*/	
	//data1_msb = (frame.data[0]<<24) | (frame.data[1]<<16)| (frame.data[2]<<8) | frame.data[3];	
		
	//data1_msb = frame.data[0] + frame.data[1];
	//data1_lsb = frame.data[2] + frame.data[3];
	
	//printf("int %d\n",data1_msb);
	//printf("char %c\n",data1_msb);
	//printf("int %d\n",data1_lsb);
		
	//int data1 = (int)(((unsigned)data1_msb << 8) | data1_lsb); //concatena os valores das posições dos 2 primeiros bytes do can frame
	printf("Data Frame0: %d\n",frame.data[0]);
	printf("Data Frame0: %0x\n",frame.data[0]);
		
	printf("Data Frame1: %d\n",frame.data[1]);
	printf("Data Frame1: %0x\n",frame.data[1]);
		
	printf("Data Frame2: %d\n",frame.data[2]);
	printf("Data Frame2: %0x\n",frame.data[2]);
		
	printf("Data Frame3: %d\n",frame.data[3]);
	printf("Data Frame3: %0x\n",frame.data[3]);
		
	printf("\r\n");
	}
	
	
	
	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

	return 0;
}
