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
	int s; 
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;
  	char data[8] = {'2', '0', '1', '0', '0', '0', '0', 'F'}; //Para o endereço das luzes(0x470), o primeiro byte sao as setas. 1 - esquerda, 2- direita e 3 os dois; o ultimo byte e lanternas

	printf("Iniciando comunicacao CAN\r\n");

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Socket");
		return 1;
	}

	strcpy(ifr.ifr_name, "can1" );
	ioctl(s, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind");
		return 1;
	}

	frame.can_id = 0x470;  //endereço para RPM 0x280 //0x470 e luzes (setas) aviso de baterias 2, 0 , 1, 0 , 0 , 0 , 0 , 0 
	frame.can_dlc = 8; //tamanho do dado. Tem que verificar se e extendido
  	//data = ['2', '0', '1', '0', '0', '0', '0', '0'];
	//data = [1, 0, 0, 62];
  
	sprintf(frame.data, data); // valor referente a 4000 RPM

	if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
		perror("Write");
		return 1;
	}

	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

	return 0;
}
