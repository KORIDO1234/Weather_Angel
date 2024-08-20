# 날씨 요정 개요

사람이 감지되면 비 여부를 알려주고, 우산통을 열어주는 시스템

# H/W 리소스
PIR 센서 , RaspberryPi, ARDUINO, Esp8266, 블루투스 모듈, LCD 디스플레이, 서보모터

# 시스템 구성

## 서버 - 클라이언트 구성
![image](https://github.com/user-attachments/assets/519e1e3a-67c3-438e-a223-277448daf275)

## 알고리즘 흐름도
![image](https://github.com/user-attachments/assets/ca5351bc-b5c2-42fe-835d-0a561cc17784)

# H/W 제작 

![image](https://github.com/user-attachments/assets/51643dbf-ffba-4ebf-a21d-1d88b36496f8)

# S/W 개발

## 라즈베리파이 클라이언트
![image](https://github.com/user-attachments/assets/636d0867-35c0-4100-af99-ca1b5dd273d9)
1. MOVE@ON이 감지되면, API(Python) 실행 및 날씨 체크 진행
2. 날씨 정보를 읽어, 서버에 RAIN@ON / RAIN@OFF 전송

## STM32 클라이언트

![image](https://github.com/user-attachments/assets/c13102ed-9bf4-43ee-b7e8-9a3beb6906d3)
1. PIR 센서에 움직임이 감지되면, 서버에 MOVE@ON 전송
2. RAIN@ON / RAIN@OFF를 감지하면, LCD 출력

## 아두이노 클라이언트
![image](https://github.com/user-attachments/assets/2c99ef34-1b43-4bf9-8cf5-3dc4bbb033db)
1. RAIN@ON이 감지되면, 우산통과 연결된 서보 모터 동작

   
# 시연 영상

https://github.com/user-attachments/assets/e313799c-96aa-43af-bb05-bdd5ac993165

