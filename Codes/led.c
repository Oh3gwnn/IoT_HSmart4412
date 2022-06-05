#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<sys/stat.h>

#define led "/dev/led" // led 장치 불러오기 

int main(){
	int dev, i;
	unsigned char data;
	dev = open(led, O_RDWR); // dev에 led 장치 불러오기 
	if(dev<0){
		printf("Can't open LED'.\n");
		exit(0);			// 장치를 못 불러올 경우 예외 처리 후 종료 
	}
	
	for(i=0;i<8;++i){		// 1,3,5,7 과 2,4,6,8 led 점멸 
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
