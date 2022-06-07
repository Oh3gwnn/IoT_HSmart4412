
// Bomb Breakdown(��ź ��ü ����) Code 
// �⺻������ TeraTerm, Ÿ�� �ý��� �� �� ��� �� �� �ְ� �����Ͽ����ϴ�. 

// ����� ������� ���� 
#include<stdio.h> 			// ����� ���� 
#include<stdlib.h> 			// ���ڿ� ��ȯ, �޸� ���� 
#include<unistd.h> 			// POSIX �ü�� API�� ���� �׼��� ���� 
#include<fcntl.h> 			// Ÿ�ٽý��� ����� ��ġ ���� 
#include<sys/types.h> 		// �ý��ۿ��� ����ϴ� �ڷ��� ���� 
#include<sys/ioctl.h> 		// �ϵ������ ����� ���� ���� 
#include<sys/stat.h> 		// ������ ���¿� ���� ���� 
#include <string.h> 		// ���ڿ� ó�� 
#include <time.h> 			// �ð� ���� 

// Ÿ�� �ý��� ��ġ �ҷ����� 
#define fnd "/dev/fnd" 		// 7-Segment FND 
#define dot "/dev/dot" 		// Dot Matrix
#define tact "/dev/tactsw" 	// Tact Switch
#define led "/dev/led" 		// LED 
#define dip "/dev/dipsw"	// Dip Switch
#define clcd "/dev/clcd" 	// Character LCD

// �Լ� ��� 
int array_equal(int a[], int b[], int size); 	// �迭 ��ġ Ȯ�� �Լ� 
int FIRST_PRINT(); 								// ù ��° CLCD ��� �Լ�
int PRINT(char P[]); 							// CLCD ��� �Լ�
int Game_1(int tm, int ts, int endTime); 		// ���� ã�� ���� 
int Game_2(int tm, int ts, int endTime); 		// �̷� ã�� ���� 
int Game_3(int tm, int ts, int endTime); 		//  ��  ���� ���� 

// ���� ���� ���
// �ҷ��� Ÿ�� �ý��� ��ġ ���� ���� 
int dips;
int leds;
int dot_mtx;
int tactsw;
int clcds;
int fnds;

unsigned char t; 					// Tact Switch �� ���� 
char P[40];							// CLCD �� ���� 
unsigned char fnd_num[4] = {0,};	// 7-Segment �� ���� 

// 7-Segment�� 0~9�� ��� ��
// ����� ���� ������ �ؾ� ����� ��µ� 
unsigned char Time_Table[]={~0x3f,~0x06,~0x5b,~0x4f,~0x66,~0x6d,~0x7d,~0x07,~0x7f,~0x67,~0x00};

// Timer ���� ���� 
int tm = 0; 	// Timer Minute 
int ts = 0;		// Timer Second(10�� �ڸ� ��) 
int * ptr_m;	// tm Point 
int * ptr_s;	// ts Point

// time() �̿��� Timer ������ ���� ����
// endTime�� ����, startTime�� �귯���� �ϰ� �� ���� ���� �� ���� Ÿ�̸� ���� 
int endTime;	 
int startTime;

// ���� �Լ� 
int main(){
	FIRST_PRINT();
	PRINT("   First Game        Start!   ");
	endTime = (unsigned)time(NULL)+300; 	// endTime�� time ��(19070.01.01)�� 300�� �Ҵ� 
	ptr_s = &ts;							// ts ��ġ �Ҵ� 
	ptr_m = &tm;							// tm ��ġ �Ҵ� 
	
	// ù ��° ����
	// return true = ���, false = ���� 
	if(Game_1(tm, ts, endTime)){
		printf("Game Clear!\n");					// ������ ȯ�� ��� 
		PRINT("   First Game        Clear!   ");	// Ÿ�� �ý��� ��� 
		usleep(450000);
		PRINT("  Second Game        Start!   ");
	}else{
		printf("Game Over!\n");
		PRINT("                   GAME OVER!   ");
		exit(0);
	}
	
	// �� ��° ����
	if(Game_2(tm, ts, endTime)){
		printf("\nGame Clear!\n");
		PRINT("  Second Game        Clear!   ");
		usleep(450000);
		PRINT("   Final Game        Start!   ");
	}else{
		printf("\nGame Over!\n");
		PRINT("                   GAME OVER!   ");
		exit(0);
	}
	
	// �� ��° ����
	if(Game_3(tm, ts, endTime)){
		printf("Game Clear!\n");
		PRINT(" Bomb Breakdown   Game Clear!!  ");
	}else{
		printf("\nGame Over!\n");
		PRINT(" Bomb Breakdown    GAME OVER!   ");
		exit(0);
	}
	return 0;
}

int Game_1(int tm, int ts, int endTime)
{
	// ���� �迭 ���� 
	int res[5][4] = {
	{3,2,4,1},
	{3,1,4,2},
	{4,3,1,2},
	{1,2,4,3},
	{2,4,1,3}
	};
	
	// ���� �迭�� ���� Dot Matrix ��� 
	unsigned char d1[5][8] = {
    {0xCC, 0x71, 0x2B, 0xCA, 0xCC, 0xCB, 0xD3, 0x95},
    {0x94, 0x67, 0x59, 0x11, 0x46, 0x7F, 0x95, 0x15},
    {0x2C, 0xFC, 0xFD, 0x89, 0x38, 0x8F, 0xC4, 0x23},
    {0xC0, 0x1B, 0xBD, 0xAE, 0x23, 0xF8, 0xFC, 0x82},
    {0x66, 0xFE, 0x5D, 0x52, 0xB9, 0x26, 0xE5, 0x81}
  	};
  	
	int ans1[4] = {0,};	// �Է� ���� ���� ���� �迭 
	int num1[4] = {0,};	// �Է� �� 
	int k=0; // �ݺ��� ���� 
	int i=0; // �ݺ��� ���� 

	srand(time(NULL)); 	// �ð� ���� 
	int random = 0; 	// ���� �� ���� 
	random = rand()%4; 	// 0~4 �� random �Լ��� �ֱ� 
	
	// ���� ���� �迭 �������� �ֱ� 
	for (k=0; k< 4; k++){
		ans1[k] = res[random][k];
	}
	
	while(1){
		int startTime = (unsigned)time(NULL)+1;					// time ���� +1 �ݺ��ؼ� ���� 
		
		if(ts<6){
			// 7-Segment ��ġ �ҷ������ Ÿ�̸� ��� �κ� 
			fnds = open(fnd, O_RDWR);
			if(fnds < 0){printf("Can't open FND.\n"); exit(0);}
			fnd_num[0] = Time_Table[0];
			fnd_num[1] = Time_Table[2-tm];
			fnd_num[2] = Time_Table[5-ts];
			fnd_num[3] = Time_Table[((endTime-startTime)%10)];	// �� ���� Ÿ�̸� 
			usleep(200000);
			write(fnds, &fnd_num, sizeof(fnd_num));
			close(fnds);
			if((endTime-startTime)%10==0){
				ts+=1;
				usleep(750000);
			}
		}
		else{
			tm++;
			ts = 0;
		}
		
		if (tm == 3){
			return 0;
		}
		
		// Dot Matrix ��� �κ� 
		dot_mtx = open(dot, O_RDWR);
  		if (dot_mtx < 0) {printf("Can't open dot matrix.\n"); exit(0);}
		switch (random){
			case 0: write(dot_mtx, &d1[0], sizeof(d1[0])); usleep(450000); break;
	    	case 1: write(dot_mtx, &d1[1], sizeof(d1[1])); usleep(450000); break;
	    	case 2: write(dot_mtx, &d1[2], sizeof(d1[2])); usleep(450000); break;
	    	case 3: write(dot_mtx, &d1[3], sizeof(d1[3])); usleep(450000); break;
	    	case 4: write(dot_mtx, &d1[4], sizeof(d1[4])); usleep(450000); break;
  		}
  		close(dot_mtx);
  		
  		// Tact Switch �Է� �κ� 
	  	tactsw = open(tact, O_RDWR);
		if (tact < 0) {printf("Can't open tact\n"); exit(0);}
	  	read(tactsw, &t, sizeof(t));
	  	close(tactsw);
	  	
		switch (t){
			case 1: for (k=0; k< 4; k++){
							printf("%d ",num1[k]);
					}
					printf("Input %d\n", t); num1[i]=1; i++; usleep(150000); break;
	    	case 2: for (k=0; k< 4; k++){
							printf("%d ",num1[k]);
					}
					printf("Input %d\n", t); num1[i]=2; i++; usleep(150000); break;
	    	case 4: for (k=0; k< 4; k++){
							printf("%d ",num1[k]);
					}
					printf("Input %d\n", t-1); num1[i]=3; i++; usleep(150000); break;
	    	case 5: for (k=0; k< 4; k++){
							printf("%d ",num1[k]);
					}
					printf("Input %d\n", t-1); num1[i]=4; i++; usleep(150000); break;
  		}
  		
  		// ���� �� 
  		if(array_equal(ans1, num1, 4)){
  			*ptr_s = ts;
  			*ptr_m = tm;
  			return 1;
		}
		
		// CLCD ��� �κ� 
		if(i == 0){PRINT("   Enter the      First number! ");}
		else if(i == 1){{PRINT("   Enter the     Second number! ");}}
		else if(i == 2){{PRINT("   Enter the      Third number! ");}}
		else if(i == 3){{PRINT("   Enter the     Fourth number! ");}}
		else if(i==4){
			PRINT("  Wrong number    - 10 Second!  ");
			i=0;
			for (k=0; k< 4; k++){
				num1[k] = 0;
			}
			ts += 1;				// Ʋ�� ��� 10�� ���� 
			printf("-10 sec!\n");
		};
	}
}

int Game_2(int tm, int ts, int endTime)
{
	// ���� �κ� ans�� �Է� �� num, �� �� �ݺ��п��� ����� ���� 
	int ans[16] = {5,9,9,9,5,5,7,7,7,5,9,9,5,9,9,5};
	int num[16] = {0,};
	int k=0;
	int i=0;
	int j=7;
	unsigned char c[8] = {0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};	// ������� ��Ʈ �迭 
	unsigned char d[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20};	// ���� �� ��Ʈ �迭 
	
	while(1){
		int startTime = (unsigned)time(NULL)+1;
		
		if(ts<6){
			fnds = open(fnd, O_RDWR);
			if(fnds < 0){printf("Can't open FND.\n"); exit(0);}
			fnd_num[0] = Time_Table[0];
			fnd_num[1] = Time_Table[2-tm];
			fnd_num[2] = Time_Table[5-ts];
			fnd_num[3] = Time_Table[((endTime-startTime)%10)];
			usleep(200000);
			write(fnds, &fnd_num, sizeof(fnd_num));
			close(fnds);
			if((endTime-startTime)%10==0){
				ts+=1;
				usleep(750000);
			}
		}
		else{
			tm++;
			ts = 0;
		}
		
		if (tm == 3){
			return 0;
		}
		
		// Dot Matirx ��� �κ� 
		dot_mtx = open(dot, O_RDWR);
  		if (dot_mtx < 0) {printf("Can't open dot matrix.\n"); exit(0);}
		
		// ���� �� �迭 c�� d�� ���� ��縸 �ֱ� 
		d[0] = c[0];
		d[1] = c[1];
		
		write(dot_mtx, &d, sizeof(d)); usleep(450000);
  		close(dot_mtx);
  		
  		// Tact Switch �Է� �κ� 
	  	tactsw = open(tact, O_RDWR);
		if (tact < 0) {printf("Can't open tact\n"); exit(0);}
		
	  	read(tactsw, &t, sizeof(t));
	  	close(tactsw);
	  	
		switch (t){
			case 5: printf(" Input %d\n", t); num[i]=5; usleep(150000);
					if(ans[i]!=num[i]){
						ts += 1;
						printf("-10 sec!\n");
						PRINT("Wrong Direction!  - 10 Second!  ");
						usleep(350000);
						PRINT("                                ");
						break;
					}
					else{
						i++;
						d[j-1] = d[j];	// �ݺ������� d�� 8���� �� ������� ��� 
						d[j] = 0x00;
						j-=1;
						for (k=0; k< 16; k++){
							printf("%d",num[k]);
						}
						break;
					}	//�� 
	    	case 7: printf(" Input %d\n", t); num[i]=7; usleep(150000);
	    			if(ans[i]!=num[i]){
	    				ts += 1;
						printf("-10 sec!\n");
						PRINT("Wrong Direction!  - 10 Second!  ");
						usleep(350000);
						PRINT("                                ");
						break;
					}
					else{
						i++; 
						d[j] = d[j]<<1; 
						for (k=0; k< 16; k++){
							printf("%d",num[k]);
						}
						break;
					}					//���� 
	    	case 9: printf(" Input %d\n", t); num[i]=9; usleep(150000);
	    			if(ans[i]!=num[i]){
	    				ts += 1;
						printf("-10 sec!\n");
						PRINT("Wrong Direction!  - 10 Second!  ");
						usleep(350000);
						PRINT("                                ");
						break;
					}
					else{
						i++; d[j] = d[j]>>1; 
						for (k=0; k< 16; k++){
							printf("%d",num[k]);
						}
						break;
					}					//������ 
	    	case 11: printf(" Input %d\n", t); num[i]=11; usleep(150000); 
	    			 if(ans[i]!=num[i]){
	    			 	ts += 1;
						printf("-10 sec!\n");
						PRINT("Wrong Direction!  - 10 Second!  ");
						usleep(350000);
						PRINT("                                ");
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
					 }	//�Ʒ� 
  		}
  		
  		// �� �Լ� 
  		if(array_equal(ans, num, 16)){
  			*ptr_s = ts;
  			*ptr_m = tm;
  			return 1;	
		}
	}
}

int Game_3(int tm, int ts, int endTime)
{
	unsigned char c;
	unsigned char d[3] ={0xad, 0x5c, 0xc8};
	int data[1] = {0,}; 	// Dip Switch �Է� �� 

	srand(time(NULL));
	int random = 0;
	random = rand()%3;
	
	data[0] |= d[random];	// Zero matrix�� or ���� 
	
	leds = open(led, O_RDWR);
  	if (leds < 0) {printf("Can't open dot led.\n"); exit(0);}

	dips = open(dip, O_RDWR);
	if (dips < 0) {printf("Can't open tact\n"); exit(0);}
		
	while(1){
		int startTime = (unsigned)time(NULL)+1;
		
		if(ts<6){
			fnds = open(fnd, O_RDWR);
			if(fnds < 0){printf("Can't open FND.\n"); exit(0);}
			fnd_num[0] = Time_Table[0];
			fnd_num[1] = Time_Table[2-tm];
			fnd_num[2] = Time_Table[5-ts];
			fnd_num[3] = Time_Table[((endTime-startTime)%10)];
			usleep(200000);
			write(fnds, &fnd_num, sizeof(fnd_num));
			close(fnds);
			if((endTime-startTime)%10==0){
				ts+=1;
				usleep(750000);
			}
		}
		else{
			tm++;
			ts = 0;
		}
		
		if (tm == 3){
			return 0;
		}
		
		// Dip Switch �Է� �κ� 
	  	read(dips, &c, sizeof(c));
  		
  		// �Է� ���� ���� ��� ���� �ٸ�
		// ����� �� ����ġ�� 0~128 ������ 2�� �������ε� �Է� ���� �� ���� 
		if(data[0]==0xad){
			if (c == 64){
			close(dips);
			close(leds);
			return 1;
			}
			else if(c != 64 && c != 0){return 0;}
		}
		if(data[0]==0x5c){
	    	if (c == 1){
			close(dips);
			close(leds);
			return 1;
			}
			else if(c != 1 && c != 0){return 0;}
		}
		if(data[0]==0xc8){
			if (c == 2){
			close(dips);
			close(leds);
			return 1;
			}
			else if(c != 2 && c != 0){return 0;}
		}
  		
  		write(leds, &data, sizeof(data));
  		usleep(450000);
	}
}

// �迭 ��ġ �Լ�: size�� �޾ƿͼ� �� ĭ�� �� 
int array_equal(int a[], int b[], int size){
	int i;
	for (i=0; i< size; i++){
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

// ���� ���� �� CLCD�� ����ϴ� �Լ�
// �׳� PRINT�� �ٸ� ���� Tact Switch �Է� �� return 
int FIRST_PRINT(){
	clcds = open(clcd, O_RDWR);
	if(clcds < 0){printf("Can't open Character LCD.\n"); exit(0);}
	char pr_clcd[40] = " Bomb Breakdown  PRESS ANY KEY! ";
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

// CLCD ��� �Լ� 
int PRINT(char P[]){
	clcds = open(clcd, O_RDWR);
	if(clcds < 0){printf("Can't open Character LCD.\n"); exit(0);}
	write(clcds, P, strlen(P));
	close(clcds);
}
