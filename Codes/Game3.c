#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include <string.h>
#include <time.h>

#define led "/dev/led"
#define dip "/dev/dipsw"

int dips, leds;
int main()
{
	unsigned char c;
	unsigned char d[3] ={0xad, 0x5c, 0xc8};
	int data[1] = {0,};

	srand(time(NULL));
	int random = 0;
	random = rand()%3;
	
	data[0] |= d[random];
	
	leds = open(led, O_RDWR);
  	if (leds < 0) {printf("Can't open dot led.\n"); exit(0);}

	dips = open(dip, O_RDWR);
	if (dips < 0) {printf("Can't open tact\n"); exit(0);}
		
	while(1){
		
	  	read(dips, &c, sizeof(c));
  		
		if(data[0]==0xad){
			printf("%d 입력\n", c); 
			if (c == 64){
			close(dips);
			close(leds);
			return 1;
			}
			else if(c != 64 && c != 0){return 0;}
		}
		if(data[0]==0x5c){
	    	printf("%d 입력\n", c);
	    	if (c == 1){
			close(dips);
			close(leds);
			return 1;
			}
			else if(c != 1 && c != 0){return 0;}
		}
		if(data[0]==0xc8){
	    	printf("%d 입력\n", c);
			if (c == 2){
			close(dips);
			close(leds);
			return 1;
			}
			else if(c != 2 && c != 0){return 0;}
		}
  		
  		write(leds, &data, sizeof(data));
  		usleep(250000);
	}
}
