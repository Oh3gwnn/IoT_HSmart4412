#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include <string.h>
#include <time.h>


#define dot "/dev/dot"
#define tact "/dev/tactsw"
int reset(int a[], int b[], unsigned char c[], int i, int j);
int array_equal(int a[], int b[], int size);

int dot_mtx;
int tactsw;
unsigned char t;
int res[5][4] = {
	{3,2,4,1},
	{3,1,4,2},
	{4,3,1,2},
	{1,2,4,3},
	{2,4,1,3}
};
int ans[4] = {0,};
int num[4] = {0,};
int k=0;
int i=0;

unsigned char d[5][8] = {
    {0xCC, 0x71, 0x2B, 0xCA, 0xCC, 0xCB, 0xD3, 0x95},
    {0x94, 0x67, 0x59, 0x11, 0x46, 0x7F, 0x95, 0x15},
    {0x2C, 0xFC, 0xFD, 0x89, 0x38, 0x8F, 0xC4, 0x23},
    {0xC0, 0x1B, 0xBD, 0xAE, 0x23, 0xF8, 0xFC, 0x82},
    {0x66, 0xFE, 0x5D, 0x52, 0xB9, 0x26, 0xE5, 0x81}
  	};
unsigned char t;

int main()
{
	srand(time(NULL));
	int random = 0;
	random = rand()%4;
	
	for (k=0; k< 4; k++){
		ans[k] = res[random][k];
	}
	
	while(1){
		dot_mtx = open(dot, O_RDWR);
  		if (dot_mtx < 0) {printf("Can't open dot matrix.\n"); return 0;}
		switch (random){
			case 0: write(dot_mtx, &d[0], sizeof(d[0])); usleep(50000); break;
	    	case 1: write(dot_mtx, &d[1], sizeof(d[1])); usleep(50000); break;
	    	case 2: write(dot_mtx, &d[2], sizeof(d[2])); usleep(50000); break;
	    	case 3: write(dot_mtx, &d[3], sizeof(d[3])); usleep(50000); break;
	    	case 4: write(dot_mtx, &d[4], sizeof(d[4])); usleep(50000); break;
  		}
  		close(dot_mtx);
  		
	  	tactsw = open(tact, O_RDWR);
		if (tact < 0) {printf("Can't open tact\n"); return 0;}
		
	  	read(tactsw, &t, sizeof(t));
	  	close(tactsw);
	  	
		switch (t){
			case 1: printf("%d입력\n", t); num[i]=1; usleep(250000);
						i++;
						for (k=0; k< 4; k++){
							printf("%d ",num[k]);
						}
						break;
	    	case 2: printf("%d입력\n", t); num[i]=2; usleep(250000);
						i++;
						for (k=0; k< 4; k++){
							printf("%d ",num[k]);
						}
						break;
	    	case 4: printf("%d입력\n", t); num[i]=3; usleep(250000);
						i++;
						for (k=0; k< 4; k++){
							printf("%d ",num[k]);
						}
						break;
	    	case 5: printf("%d입력\n", t); num[i]=4; usleep(250000); 
					 	i++; 
						for (k=0; k< 4; k++){
							printf("%d ",num[k]);
						}
						break;
  		}
  		
  		if(array_equal(ans, num, 4)){
  			return 0;	
		}
	}
}
			
int array_equal(int a[], int b[], int size){
	int i;
	for (i=0; i< 4; i++){
		if (a[i]!=b[i]){
			return 0;
			break;
		}
		else{
			continue;
		}
		return 1;
	}
}
