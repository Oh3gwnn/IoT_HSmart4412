#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include <string.h>
#include <time.h>

#define clcd "/dev/clcd"
#define tact "/dev/tactsw"
int clcds;
int tactsw;
unsigned char t;

//int FIRST_PRINT(){
int main(){
	clcds = open(clcd, O_RDWR);
	if(clcds < 0){printf("Can't open Character LCD.\n"); exit(0);}
	char pr_clcd[40] = "Wrong Direction!  - 10 Second!  ";
	write(clcds, pr_clcd, strlen(pr_clcd));
	close(clcds);
	
	while(1){
		tactsw = open(tact, O_RDWR);
		if (tact < 0) {printf("Can't open tact\n"); exit(0);}
		read(tactsw, &t, sizeof(t));
		close(tactsw);
		
	  	if (t != 0){
	  		return 0;
		}
	}
}
