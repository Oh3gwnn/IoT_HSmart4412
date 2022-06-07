
// Bomb Breakdown(폭탄 해체 게임) Code 
// 기본적으로 TeraTerm, 타겟 시스템 둘 다 출력 될 수 있게 제작하였습니다. 

// 사용한 헤더파일 종류 
#include<stdio.h> 			// 입출력 관련 
#include<stdlib.h> 			// 문자열 변환, 메모리 관련 
#include<unistd.h> 			// POSIX 운영체제 API에 대한 액세스 제공 
#include<fcntl.h> 			// 타겟시스템 입출력 장치 관련 
#include<sys/types.h> 			// 시스템에서 사용하는 자료형 정보 
#include<sys/ioctl.h> 			// 하드웨어의 제어와 상태 정보 
#include<sys/stat.h> 			// 파일의 상태에 대한 정보 
#include <string.h> 			// 문자열 처리 
#include <time.h> 			// 시간 관련 

// 타겟 시스템 장치 불러오기 
#define fnd "/dev/fnd" 		// 7-Segment FND 
#define dot "/dev/dot" 		// Dot Matrix
#define tact "/dev/tactsw" 	// Tact Switch
#define led "/dev/led" 		// LED 
#define dip "/dev/dipsw"	// Dip Switch
#define clcd "/dev/clcd" 	// Character LCD

// 함수 목록 
int array_equal(int a[], int b[], int size); 	// 배열 일치 확인 함수 
int FIRST_PRINT(); 								// 첫 번째 CLCD 출력 함수
int PRINT(char P[]); 								// CLCD 출력 함수
int Game_1(int tm, int ts, int endTime); 		// 문양 찾기 게임 
int Game_2(int tm, int ts, int endTime); 		// 미로 찾기 게임 
int Game_3(int tm, int ts, int endTime); 		//  선  끊기 게임 

// 전역 변수 목록
// 불러온 타겟 시스템 장치 변수 선언 
int dips;
int leds;
int dot_mtx;
int tactsw;
int clcds;
int fnds;

unsigned char t; 			// Tact Switch 값 변수 
char P[40];				// CLCD 값 변수 
unsigned char fnd_num[4] = {0,};	// 7-Segment 값 변수 

// 7-Segment의 0~9의 출력 값
// 참고로 음수 값으로 해야 제대로 출력됨 
unsigned char Time_Table[]={~0x3f,~0x06,~0x5b,~0x4f,~0x66,~0x6d,~0x7d,~0x07,~0x7f,~0x67,~0x00};

// Timer 관련 변수 
int tm = 0; 	// Timer Minute 
int ts = 0;	// Timer Second(10의 자리 수) 
int * ptr_m;	// tm Point 
int * ptr_s;	// ts Point

// time() 이용한 Timer 설정을 위해 선언
// endTime은 고정, startTime은 흘러가게 하고 두 값의 차로 초 단위 타이머 구현 
int endTime;	 
int startTime;

// 메인 함수 
int main(){
	FIRST_PRINT();
	PRINT("   First Game        Start!   ");
	endTime = (unsigned)time(NULL)+300; 	// endTime에 time 값(19070.01.01)과 300초 할당 
	ptr_s = &ts;							// ts 위치 할당 
	ptr_m = &tm;							// tm 위치 할당 
	
	// 첫 번째 게임
	// return true = 통과, false = 실패 
	if(Game_1(tm, ts, endTime)){
		printf("Game Clear!\n");			// 윈도우 환경 출력 
		PRINT("   First Game        Clear!   ");	// 타겟 시스템 출력 
		usleep(450000);
		PRINT("  Second Game        Start!   ");
	}else{
		printf("Game Over!\n");
		PRINT("                   GAME OVER!   ");
		exit(0);
	}
	
	// 두 번째 게임
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
	
	// 세 번째 게임
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
	// 정답 배열 모음 
	int res[5][4] = {
	{3,2,4,1},
	{3,1,4,2},
	{4,3,1,2},
	{1,2,4,3},
	{2,4,1,3}
	};
	
	// 정답 배열에 따라 Dot Matrix 출력 
	unsigned char d1[5][8] = {
    {0xCC, 0x71, 0x2B, 0xCA, 0xCC, 0xCB, 0xD3, 0x95},
    {0x94, 0x67, 0x59, 0x11, 0x46, 0x7F, 0x95, 0x15},
    {0x2C, 0xFC, 0xFD, 0x89, 0x38, 0x8F, 0xC4, 0x23},
    {0xC0, 0x1B, 0xBD, 0xAE, 0x23, 0xF8, 0xFC, 0x82},
    {0x66, 0xFE, 0x5D, 0x52, 0xB9, 0x26, 0xE5, 0x81}
  	};
  	
	int ans1[4] = {0,};	// 입력 값과 비교할 정답 배열 
	int num1[4] = {0,};	// 입력 값 
	int k=0; // 반복문 변수 
	int i=0; // 반복문 변수 

	srand(time(NULL)); 	// 시간 고정 
	int random = 0; 	// 랜덤 값 변수 
	random = rand()%4; 	// 0~4 값 random 함수에 넣기 
	
	// 비교할 정답 배열 랜덤으로 넣기 
	for (k=0; k< 4; k++){
		ans1[k] = res[random][k];
	}
	
	while(1){
		int startTime = (unsigned)time(NULL)+1;				// time 값과 +1 반복해서 선언 
		
		if(ts<6){
			// 7-Segment 장치 불러오기와 타이머 출력 부분 
			fnds = open(fnd, O_RDWR);
			if(fnds < 0){printf("Can't open FND.\n"); exit(0);}
			fnd_num[0] = Time_Table[0];
			fnd_num[1] = Time_Table[2-tm];
			fnd_num[2] = Time_Table[5-ts];
			fnd_num[3] = Time_Table[((endTime-startTime)%10)];	// 초 단위 타이머 
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
		
		// Dot Matrix 출력 부분 
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
  		
  		// Tact Switch 입력 부분 
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
  		
  		// 정답 비교 
  		if(array_equal(ans1, num1, 4)){
  			*ptr_s = ts;
  			*ptr_m = tm;
  			return 1;
		}
		
		// CLCD 출력 부분 
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
			ts += 1;						// 틀릴 경우 10초 감소 
			printf("-10 sec!\n");
		};
	}
}

int Game_2(int tm, int ts, int endTime)
{
	// 정답 부분 ans와 입력 값 num, 그 외 반복분에서 사용할 변수 
	int ans[16] = {5,9,9,9,5,5,7,7,7,5,9,9,5,9,9,5};
	int num[16] = {0,};
	int k=0;
	int i=0;
	int j=7;
	unsigned char c[8] = {0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};	// ㄱㄴ모양 도트 배열 
	unsigned char d[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20};	// 시작 점 도트 배열 
	
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
		
		// Dot Matirx 출력 부분 
		dot_mtx = open(dot, O_RDWR);
  		if (dot_mtx < 0) {printf("Can't open dot matrix.\n"); exit(0);}
		
		// 시작 점 배열 c에 d의 ㄱㄴ 모양만 넣기 
		d[0] = c[0];
		d[1] = c[1];
		
		write(dot_mtx, &d, sizeof(d)); usleep(450000);
  		close(dot_mtx);
  		
  		// Tact Switch 입력 부분 
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
						d[j-1] = d[j];	// 반복문으로 d의 8개의 행 순서대로 출력 
						d[j] = 0x00;
						j-=1;
						for (k=0; k< 16; k++){
							printf("%d",num[k]);
						}
						break;
					}	//위 
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
					}					//왼쪽 
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
					}					//오른쪽 
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
					 }	//아래 
  		}
  		
  		// 비교 함수 
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
	int data[1] = {0,}; 	// Dip Switch 입력 값 

	srand(time(NULL));
	int random = 0;
	random = rand()%3;
	
	data[0] |= d[random];	// Zero matrix와 or 연산 
	
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
		
		// Dip Switch 입력 부분 
	  	read(dips, &c, sizeof(c));
  		
  		// 입력 값에 따라 출력 값이 다름
		// 참고로 각 스위치는 0~128 사이의 2의 제곱수인데 입력 값은 합 연산 
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

// 배열 일치 함수: size를 받아와서 한 칸씩 비교 
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

// 게임 시작 시 CLCD에 출력하는 함수
// 그냥 PRINT랑 다른 점은 Tact Switch 입력 시 return 
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

// CLCD 출력 함수 
int PRINT(char P[]){
	clcds = open(clcd, O_RDWR);
	if(clcds < 0){printf("Can't open Character LCD.\n"); exit(0);}
	write(clcds, P, strlen(P));
	close(clcds);
}
