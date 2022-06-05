#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include <string.h>
#include <time.h>

#define fnd "/dev/fnd"

int looptime(int m, int s, int end);

unsigned char fnd_num[4] = {0,};
int fnds;
unsigned char Time_Table[]={~0x3f,~0x06,~0x5b,~0x4f,~0x66,~0x6d,~0x7d,~0x07,~0x7f,~0x67,~0x00};

int main(){
	fnds = open(fnd, O_RDWR);
	if(fnds < 0){printf("Can't open FND.\n"); exit(0);}
	
	int end = (unsigned)time(NULL);
	end += 9;
	int m = 0;
	int s = 0;
	
	while(1){
		if(s<5){
			looptime(m, s, end);
		}
		else{
			s=0;
			m++; 
			if(m==2){
			 	return 0;
			}
		}
	}
}

int looptime(int m, int s, int end){
	int start = (unsigned)time(NULL);
		
	fnd_num[0] = Time_Table[0];
	fnd_num[1] = Time_Table[2-m];
	fnd_num[2] = Time_Table[5-s];
	fnd_num[3] = Time_Table[end-start];
	
	write(fnds, &fnd_num, sizeof(fnd_num));
	sleep(1);
		
	if(end - start == 0){
		int end = (unsigned)time(NULL);
		end += 9;
		s++;
		return s, end;
	}
}
