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
int ans[16] = {5,9,9,9,5,5,7,7,7,5,9,9,5,9,9,5};
int num[16] = {0,};
int k=0;
int i=0;
int j=7;
unsigned char c[8] = {0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char d[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20};
unsigned char t;

int main()
{
	while(1){
		dot_mtx = open(dot, O_RDWR);
  		if (dot_mtx < 0) {printf("Can't open dot matrix.\n"); return 0;} //예외
		
		d[0] = c[0];
		d[1] = c[1];
		
		write(dot_mtx, &d, sizeof(d)); usleep(50000);
  		close(dot_mtx);
  		
	  	tactsw = open(tact, O_RDWR);
		if (tact < 0) {printf("Can't open tact\n"); return 0;}
		
	  	read(tactsw, &t, sizeof(t));
	  	close(tactsw);
	  	
		switch (t){
			case 5: printf("%d입력\n", t); num[i]=5; usleep(250000);
					if(ans[i]!=num[i]){
						break;
					}
					else{
						i++;
						d[j-1] = d[j];
						d[j] = 0x00;
						j-=1;
						for (k=0; k< 16; k++){
							printf("%d",num[k]);
						}
						break;
					}	//위 
	    	case 7: printf("%d입력\n", t); num[i]=7; usleep(250000);
	    			if(ans[i]!=num[i]){
						break;
					}
					else{
						i++; 
						d[j] = d[j]<<1; 
						for (k=0; k< 16; k++){
							printf("%d",num[k]);
						}
						break;
					}					//왼쪽 
	    	case 9: printf("%d입력\n", t); num[i]=9; usleep(250000);
	    			if(ans[i]!=num[i]){
						break;
					}
					else{
						i++; d[j] = d[j]>>1; 
						for (k=0; k< 16; k++){
							printf("%d",num[k]);
						}
						break;
					}					//오른쪽 
	    	case 11: printf("%d입력\n", t); num[i]=11; usleep(250000); 
	    			 if(ans[i]!=num[i]){
						break;
						}
					 else{
					 	i++; 
						d[j+1] = d[j]; d[j] = 0x00; 
						j++; 
						for (k=0; k< 16; k++){
							printf("%d",num[k]);
						}
						break;
						}	//아래 
  		}
  		
  		if(array_equal(ans, num, 16)){
  			return 0;	
		}
	}
}
			
int array_equal(int a[], int b[], int size){
	int i;
	for (i=0; i< 16; i++){
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
