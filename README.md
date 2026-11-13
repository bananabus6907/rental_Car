# 🚗 Rental Car Reservation System

Windows 콘솔 기반 렌터카 예약 시스템입니다. C 언어와 Windows API를 활용하여 사용자 친화적인 TUI(Text User Interface)를 구현했습니다.

## 프로젝트 개요

이 프로젝트는 렌터카 예약 서비스의 핵심 기능을 콘솔 환경에서 구현한 프로그램입니다. 사용자 인증, 차량 검색, 예약 관리 등의 기능을 제공합니다.

### 주요 기능

- **사용자 관리**
  - 회원가입 (이름, 면허번호, 주민등록번호, 학번, ID/PW)
  - 로그인/로그아웃
  - 사용자 정보 파일 저장/로드

- **차량 관리**
  - 차량 목록 조회
  - 위치별 차량 검색
  - 차량 정보 (종류, 위치, 시간당/km당 요금)

- **예약 시스템**
  - 차량 예약
  - 예약 내역 조회
  - 요금 계산

- **TUI (Text User Interface)**
  - 방향키 기반 메뉴 네비게이션
  - 콘솔 색상 제어
  - 커서 위치 제어
  - 실시간 입력 처리

## 기술 스택

### 언어 및 API
- **C (C11 표준)**
- **Windows API**
  - Console API (커서, 색상 제어)
  - conio.h (키보드 입력)

### 주요 데이터 구조

```c
// 차량 데이터 구조체
typedef struct {
    int id;           // 차량 고유 번호
    char* name;       // 차량 종류
    char* location;   // 위치
    int PPH;          // 시간당 요금
    int PPK;          // km당 요금
} CarData;

// 사용자 데이터 구조체
typedef struct {
    char* name;       // 사용자 이름
    char* license;    // 운전면허 번호
    char* id;         // 아이디
    char* password;   // 비밀번호
} UserData;
```

## 파일 구조

```
rental_Car/
├── rental_car_11.c        # Windows 11 버전 (1,458줄)
├── rental_car_10.c        # Windows 10 버전
├── lental car data.txt    # 차량 데이터
├── userdata.txt           # 사용자 데이터
└── README.md
```

## 빌드 및 실행

### Visual Studio

```cmd
1. Visual Studio 실행
2. 새 프로젝트 → 빈 프로젝트
3. rental_car_11.c 추가
4. 빌드 및 실행 (Ctrl + F5)
```

### 명령줄

```cmd
# MSVC
cl rental_car_11.c

# 실행
rental_car.exe
```

## 주요 기능 구현

### 1. Windows Console API

```c
// 커서 위치 제어
gotoxy(int x, int y)

// 텍스트 색상 변경
SetConsoleTextAttribute()

// 키보드 입력 (방향키, Enter)
_getch()
```

### 2. 파일 I/O

- 차량 데이터 로드/저장
- 사용자 정보 관리
- CSV 형식 파싱

### 3. 동적 메모리 관리

- malloc/free 활용
- 구조체 배열 관리
- 메모리 누수 방지

## 학습 내용

1. **시스템 프로그래밍**
   - Windows API
   - 콘솔 I/O
   - 파일 시스템

2. **데이터 구조**
   - 구조체 설계
   - 동적 메모리
   - 포인터 활용

3. **UI/UX**
   - TUI 설계
   - 키보드 이벤트
   - 사용자 피드백

## 라이선스

Educational Project
