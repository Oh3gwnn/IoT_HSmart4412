
# **폭탄 해체 게임(BOMB BREAKDOWN GAME)**


## **1. 프로젝트 계획**

H-Smart4412 타겟 시스템의 다양한 장치를 이용하고 싶어서 계획한 폭탄 해체 게임.  
게임을 해결해나가는 아이디어 부분은 `Keep Talking and Nobody Explodes` 게임을 참고하였습니다.  

['**Keep Talking and Nobody Explodes**'](http://www.keeptalkinggame.com/) 사이트 링크 참고

---

## **2. 프로젝트 사전조사**

 - 이전에 누군가 진행하였던 타겟시스템 실습 내용들을 확인  
    프로젝트들의 전체적인 구조와 IEB Board 사용 방법을 참고


### **게임 특징**

 - **미니 게임들 (Mini Games)**  
    전체적인 폭탄 해체 게임에 여러 미니 게임 모듈이 랜덤으로 진행  
    이 점을 참고하여 타겟시스템의 장치에 맞춰 아이디어를 내서 미니 게임을 제작  

 - **게임 매뉴얼 (Game Manual)**  
    단순히 게임을 잘한다고 해서 게임을 클리어할 수 없다.  
    게임을 직접 플레이하는 사람과 게임 클리어 매뉴얼이 존재하여 클리어 방법을 설명  
    제작할 게임들을 바탕으로 새로운 매뉴얼 제작 예정


   ['**Keep Talking and Nobody Explodes 폭탄 해체 매뉴얼**'](https://www.bombmanual.com/ko/) 참고

 - **폭탄해체 게임 알고리즘**

    ![img_01](/Images/Bomb_flowchart.jpg)

---

## **3. 게임 소개**
### **게임 제작**

    문양 해석, 미로 찾기, 선 끊기 3개의 게임으로 구성
    독자적인 게임을 제작하였고, 게임마다 랜덤 시드 값으로 무작위 정답으로 구성

자세한 설명은  
    ['Bomb Breakdown_02_20220524.pdf'](https://github.com/hyoungteak/IoT_HSmart4412/blob/main/Docs/Bomb%20Breakdown_02_20220524.pdf), ['Bomb Breakdown_03_20220531.pdf'](https://github.com/hyoungteak/IoT_HSmart4412/blob/main/Docs/Bomb%20Breakdown_03_20220531.pdf) 문서  
    Google Docs 문서 ['1차 중간 발표'](https://docs.google.com/document/d/18wyuJe8805JIE2ftS6ue8w0owCxuHiCCFlvMyepP3zM/edit?usp=sharing), ['2차 중간 발표'](https://docs.google.com/document/d/1gHcK0-FiGUvA0DYAOu1RZ-o5m-i1AtpV3A5RLSvth9A/edit?usp=sharing) 참고  **(추천)**

 - ### **첫번째 게임 - 문양 해석**

    ![gif_01](/Images/Pattern_interpretation.gif)  

 - ### **두번째 게임 - 미로 찾기**
    
    ![gif_02](/Images/Maze.gif)
    
 - ### **세번째 게임 - 선 끊기**

    ![gif_03](/Images/Cut_line.gif)
 - ### **그 외 기능**

    - **Character LCD 기능**  
    게임을 진행하는 사람에게도 CLCD 문자 출력으로 편의성 추가
    ![img_02](/Images/Clcd.jpg)

    - **타이머 기능(`<time.h>` 모듈 활용)**  
    실시간으로 확인할 수 있는 타이머를 7-segment를 이용하여 출력
    ![gif_04](/Images/Timer.gif)

 - ### **게임 매뉴얼**
   기존 게임의 매뉴얼을 참고하여 폭탄 해체 게임의 매뉴얼도 제작하였습니다.  
   ![img_02](/Images/BB_Manual.png)  
   [**Bomb Breakdown Manual**](https://github.com/hyoungteak/IoT_HSmart4412/blob/main/Docs/Bomb_Breakdown_Manual.pdf) 참고


---

## **4. 게임 코드**

 - **`'Maingame.c'` - 메인코드 작성**  
    앞의 모든 코드를 종합하여 메인 코드 제작 - 
    ['Maingame.c'](https://github.com/hyoungteak/IoT_HSmart4412/blob/main/Codes/Maingame.c) 참고

---

## **5. 게임 실행 영상**
- **폭탄 해체 게임 실행 Youtube Link** : [H-Smart4412 활용 폭탄 해체 게임(Bomb Breakdown Game)](https://youtu.be/Z68i4ZN5vgY)

---

## **출처 및 참고**
 - [**Keep Talking and Nobody Explodes**](http://www.keeptalkinggame.com/) - 게임 참고
 - [**Keep Talking and Nobody Explodes 폭탄 해체 매뉴얼**](https://www.bombmanual.com/ko/) - 게임 매뉴얼 참고
 - [**SnakeGame**](https://github.com/jinwoo1225/SnakeGameWithSmart4412) - H-Smart4412 타겟 시스템의 장치 활용 참고  

---

## [**제작 일지**](https://github.com/hyoungteak/IoT_HSmart4412/blob/main/Docs/Bomb_breakdown_production_log.md)

