#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<sys/stat.h>

#define led "/dev/led" // led ��ġ �ҷ����� 

int main(){
	int dev, i;
	unsigned char data;
	dev = open(led, O_RDWR); // dev�� led ��ġ �ҷ����� 
	if(dev<0){
		printf("Can't open LED'.\n");
		exit(0);			// ��ġ�� �� �ҷ��� ��� ���� ó�� �� ���� 
	}
	
	for(i=0;i<8;++i){		// 1,3,5,7 �� 2,4,6,8 led ���� 
		if(i%2==1){
			data = 0x55;
		}
		else{
			data = 0xAA;
		}
		write(dev, &data, sizeof(unsigned char));
		usleep(300000);
	}
	close(dev);
	
	return 0;
}
