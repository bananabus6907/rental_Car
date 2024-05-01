#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h> // window api
#include <conio.h> // 콘솔 입출력 함수
#include <stdbool.h> // bool, true, false

//
typedef struct
{
    int id; // 고유 번호
    char* name; // 차량 종류
    char* gateTag; // 위치
    char* location; // 위치
    int PPH; // price per hour
    int PPK; // price per km
} CarData;

typedef struct
{
    char* name;
    char* license; // 운전면허 번호
    char* RRN; // resident registration number 주민등록번호
    char* studentId; // 학번
    char* id; // 아이디
    char* password; // 패스워드
} UserData;

typedef int CarDataTag; //int형 자료형 정의
enum CarDataTag_ //열거형 정의
{
    CarDataTag_none = 0,
    CarDataTag_gateTag = 1,
    CarDataTag_location = 2,
    CarDataTag_name = 3,
    CarDataTag_PPH = 4,
    CarDataTag_PPK = 5,

};

typedef int UserDataTag; //사용자 정보 열거형 정의
enum UserDataTag_
{
    UserDataTag_none = 0,
    UserDataTag_name = 1,
    UserDataTag_license = 2,
    UserDataTag_RRN = 3,
    UserDataTag_studentId = 4,
    UserDataTag_id = 5,
    UserDataTag_password = 6,

};

typedef int Key;  //키보드 방향키, 엔터 입력 정보 설정
enum Key_
{
    Key_none = 0,

    Key_up = 72,
    Key_down = 80,
    Key_left = 75,
    Key_right = 77,

    Key_enter = 100,
};

typedef int KeyFilter; //키보드 입력 조건 설정
enum KeyFilter_
{
    KeyFilter_none = 0,
    KeyFilter_no_korean = 1, //한글 입력제한
    KeyFilter_number = 2, //숫자만 
};

enum ColorType { //색깔 열거형으로 정의
    BLACK,     //0
    darkBLUE,   //1
    DarkGreen,   //2
    darkSkyBlue,//3
    DarkRed,     //4
    DarkPurple,   //5
    DarkYellow,   //6
    GRAY,      //7
    DarkGray,   //8
    BLUE,      //9
    GREEN,      //10
    SkyBlue,   //11
    RED,      //12
    PURPLE,      //13
    YELLOW,      //14
    WHITE      //15
} COLOR;

//
CarData* carDataList = NULL; //포인터 초기화
UserData* userDataList = NULL;

//
CarData* get_car_data_list(); //자동차 데이터 목록을 반환하는 함수
UserData* get_user_data_list(); //사용자 데이터 목록을 반환하는 함수

int userFound(UserData* data, char* id, char* password); //ID, 비밀번호를 사용하여 사용자를 찾는 함수
bool user_login(UserData* data); // 로그인  함수
bool user_join(UserData* data); //회원가입 함수
bool save_user_data(const UserData data, int size); //사용자 데이터를 저장 함수

static int cy = 0; // 현재 y(줄) 위치
static int csy = 0; // 현재 선택한 y축 위치 (실제로 출력되고 있는 y축)

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //커서숨기는 함수에 사용되는 열거형 
void setcursortype(CURSOR_TYPE c) { //커서숨기는 함수 
    CONSOLE_CURSOR_INFO CurInfo; //콘솔 커서 정보 저장할 구조체 ㅈ선언
    //경우에 맞는 콘솔 커서 설정
    switch (c) {
    case NOCURSOR: //커서 X
        CurInfo.dwSize = 1;
        CurInfo.bVisible = FALSE;
        break;
    case SOLIDCURSOR: //선형태 커서
        CurInfo.dwSize = 100;
        CurInfo.bVisible = TRUE;
        break;
    case NORMALCURSOR: //기본 커서
        CurInfo.dwSize = 20;
        CurInfo.bVisible = TRUE;
        break;
    }
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo); //커서 설정
}

bool gotoxy(int x, int y) { //커서 이동을 위한 값 계산 함수 
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // 표준 출력 핸들러

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi); //현재 콘솔 화면 정보 가져오기

    int page = csbi.dwSize.Y * (int)(y / csbi.dwSize.Y); // page위치

    if (csbi.dwSize.Y * (int)(csy / csbi.dwSize.Y) <= y && y < csbi.dwSize.Y * ((int)(csy / csbi.dwSize.Y) + 1)) { // 현재 y좌표가 유효한 범위 내에 있을때 
#ifdef _WIN32 
        COORD pos = { x, y - page }; //커서 설정
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos); //설정된 위치로 커서 이동
#else // window가 아닐 때 (Linux나 macOS)
        printf("\033[%d;%dH", y - yfilter, x);
#endif

        return true; // 제대로 실행 되면 true
    }

    return false; // 안되면 false
}

void Text_select(char const* const _Format, bool selected, int x, int y) //커서 위치에서 누르면 텍스트 색상 변경 설정
{
    if (gotoxy(x, y)) {

        if (selected) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WHITE << 4) + BLACK); // set

        printf(_Format);

        if (selected) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE); // reset
    }
}
void Text_Filter(char const* const _Format, bool selected, bool filtered, int x, int y) //커서 위치에서 누르고, 필터링 될때 색상 변경 설정
{
    if (gotoxy(x, y)) {
        if (selected) {
            if (filtered)        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (GRAY << 4) + BLACK); // selected & filtered
            else                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WHITE << 4) + BLACK); // selected
        }
        else if (filtered)     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (DarkGreen << 4) + BLACK); // filtered

        printf(_Format);

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE); // reset
    }
}
void Text_error(char const* const _Format, bool error, int x, int y) //오류 여부 확인 함수 
{
    if (gotoxy(x, y)) {
        if (error) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + RED); // set red
            printf(_Format);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE); // reset , white
        }
        else {
            for (int i = 0; i < strlen(_Format); i++) printf(" ");
        }
    }
}


Key input(char** str, KeyFilter filter) //입력 함수
{
    setcursortype(NORMALCURSOR);

    Key key = Key_none;
    char tmp[100]; // 충분한 공간 확보를 위한 임시 문자 배열 선언
    int i = 0;

    if ((*str) != NULL)
    {
        strcpy(tmp, (*str));
        for (; i < tmp[i] != '\0'; i++) putchar(tmp[i]); // 입력된 문자 출력

    }

    char ch;
    while (1) {
        ch = _getch(); // 키 입력 읽기

        if (ch == -32) // 방향키
        {
            ch = _getch(); //입력값 input

            key = ch;

            tmp[i] = '\0'; // 문자열 끝에 널 문자 추가
            break;
        }
        else {
            if (ch == '\r' || ch == '\n') { // Enter 키가 입력되면 입력 종료
                key = Key_enter;
                tmp[i] = '\0'; // 문자열 끝에 널 문자 추가
                break;
            }
            else if (ch == '\b') { // 백스페이스 키가 입력되면 이전 문자 삭제
                if (i > 0) {
                    printf("\b \b"); // 커서를 지우고 뒤로 이동
                    i--;
                }
            }
            else if (i < 99) { // 최대 길이까지 입력 받기
                if (filter == KeyFilter_number
                    && !(48 <= (int)ch && (int)ch <= 57)) //숫자만
                {
                    continue;
                }
                if (filter == KeyFilter_no_korean && IsDBCSLeadByte(ch)) continue; //한글 안됨

                putchar(ch); // 입력된 문자 출력
                tmp[i] = ch; // 입력된 문자 저장
                i++;
            }
        }
    }

    *str = (char*)malloc(strlen(tmp) + 1);
    strcpy(*str, tmp);

    setcursortype(NOCURSOR);

    return key;
}

Key input_stack(char** str, int* stack, int maxStack, KeyFilter filter) //방향키로 움직이는 커서를 스택으로 관리
{
    Key key = input(str, filter); // 입력받음

    if (stack != NULL) {
        if (key == Key_up)     --(*stack); // 위쪽 화살표 스택 감소
        else if (key == Key_down || key == Key_enter)   ++(*stack); // 아래쪽 스택 증가

        if (*stack < 0)         *stack = maxStack; //스택 음수값이면 최댓값
        else if (*stack > maxStack)  *stack = 0; // 최댓값 초과시 0
    }

    return key;
}

Key input_key(int* stack, int maxStack) //방향키 기본 설정
{
    Key key = Key_none;

    char ch = _getch(); // 키 입력 읽기

    if (ch == -32) // 방향키
    {
        ch = _getch(); //입력값 input

        key = ch;
    }
    else
    {
        if (ch == '\r' || ch == '\n') {
            key = Key_enter;
        }
    }

    if (stack != NULL) {
        if (key == Key_up)     --(*stack);
        else if (key == Key_down || key == Key_enter)    ++(*stack);

        if ((*stack) < 0)         (*stack) = maxStack;
        else if ((*stack) > maxStack)  (*stack) = 0;
    }

    return key;
}

Key input_key_grid(int* x, int* y) // 방향키 상하좌우 이동을 위한 기본 설정
{
    Key key = Key_none;

    char ch = _getch(); // 키 입력 읽기

    if (ch == -32) // 방향키
    {
        ch = _getch(); //입력값 input

        key = ch;
    }
    else {
        if (ch == '\r' || ch == '\n') {
            key = Key_enter;
        }
    }

    if (key == Key_up)          --(*y);
    else if (key == Key_down)   ++(*y);

    if (key == Key_left)        --(*x);
    else if (key == Key_right)  ++(*x);

    return key;
}

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    COORD last_dwSize = csbi.dwSize;
    //
    setcursortype(NOCURSOR); //커서 없앰 

    // ----------------------------------------------------------------- (초기 정보 불러오기) start
    // 자동차 정보
    carDataList = get_car_data_list();

    if (!carDataList)
    {
        printf("error get_car_data_list\n");
        return -1;
    }
    //사용자 정보
    userDataList = get_user_data_list();

    if (!userDataList)
    {
        printf("error get_user_data_list\n");
        return -2;
    }
    // ----------------------------------------------------------------- (초기 정보 불러오기) end


    // ----------------------------------------------------------------- (로그인 & 회원가입) start
    UserData user = { NULL, NULL, NULL, NULL,  NULL, NULL }; //초기화
    while (!user_login(&user)) //로그인 성공될 때까지 반복
    {
        // ----------------------------------------------------------------- (회원가입) start
        UserData newUser = { NULL, NULL, NULL, NULL,  NULL, NULL };
        if (user_join(&newUser))
        {
            int size = (_msize(userDataList) / sizeof(UserData)) + 1; //데이터 크기 계산
            if (!save_user_data(newUser, size))
            {
                printf("error save_user_data\n");
                return -3;
            }

            userDataList = (UserData*)realloc(userDataList, sizeof(UserData) * size); //데이터 리스트 크기 재할당
            userDataList[size - 1] = newUser;
        }
        // ----------------------------------------------------------------- (회원가입) end
    }
    // ----------------------------------------------------------------- (로그인 & 회원가입) end


    // ----------------------------------------------------------------- (자동차 중복 제거) start
    char** carDataStack = (char**)malloc(sizeof(char*)); //자동차 데이터 스택 메모리 할당

    carDataStack[0] = (char*)malloc(strlen(carDataList[0].name) * sizeof(char)); //첫번째 데이터 크기 할당
    carDataStack[0] = carDataList[0].name; // 첫번째 데이터 저장

    int carCount = 1, maxCarNameLegth = 0; // 초기화
    for (int index = 1; index < _msize(carDataList) / sizeof(CarData); index++) //리스트 탐색, 중복 데이터 제거
    {
        bool isNew = true; //새로운 데이터 추가 여부초기화
        for (int i = 0; i < carCount; i++)
        {
            if (strcmp(carDataList[index].name, carDataStack[i]) == 0) //중복인 경우 데이터 추가 x
            {
                isNew = false;
                break;
            }
        }
        if (isNew) // 데이터 추가, 중복된 이름이 아닐 경우 데이터 추가
        {
            char** temp = (char**)realloc(carDataStack, sizeof(char*) * (carCount + 1));
            if (temp != NULL) {
                carDataStack = temp;
                carDataStack[carCount] = (char*)malloc(strlen(carDataList[index].name) + 1);
                if (carDataStack[carCount] != NULL) {
                    strcpy(carDataStack[carCount], carDataList[index].name);
                    if (strlen(carDataStack[carCount]) > maxCarNameLegth) maxCarNameLegth = strlen(carDataStack[carCount]);
                    carCount++;
                }
            }
        }
    }
    // ----------------------------------------------------------------- (자동차 중복 제거) end

    // ----------------------------------------------------------------- (게이트 태그 & 위치 중복 제거) start
    char** gateTagStack = (char**)malloc(sizeof(char*));
    char** locationStack = (char**)malloc(sizeof(char*)); //동적할당

    gateTagStack[0] = (char*)malloc(strlen(carDataList[0].gateTag) * sizeof(char));
    gateTagStack[0] = carDataList[0].gateTag;

    locationStack[0] = (char*)malloc(strlen(carDataList[0].location) * sizeof(char));
    locationStack[0] = carDataList[0].location;

    int gtCount = 1, maxGtNameLegth = 0; //게이트 및 위치 데이터 초기화
    int locCount = 1, maxLocNameLegth = 0;
    for (int index = 1; index < _msize(carDataList) / sizeof(CarData); index++) //자동차 데이터 리스트 탐색
    {
        if (strcmp(carDataList[index].gateTag, gateTagStack[gtCount - 1]) != 0) //이전 게이트 태그와 다름
        {
            // 임시로 새로운 크기의 게이트 태그 스택을 할당하여 현재 게이트 태그를 추가하기 위한 공간을 확보
            char** temp = (char**)realloc(gateTagStack, sizeof(char*) * (gtCount + 1));
            if (temp != NULL) {
                gateTagStack = temp; // 새로운 게이트 태그 스택으로 업데이트
                gateTagStack[gtCount] = (char*)malloc(sizeof(char) * (strlen(carDataList[index].gateTag) + 1)); // 현재 게이트 태그를 저장할 공간 할당
                if (gateTagStack[gtCount] != NULL) {
                    strcpy(gateTagStack[gtCount], carDataList[index].gateTag); // 현재 게이트 태그를 새로운 공간에 복사
                    if (strlen(gateTagStack[gtCount]) > maxGtNameLegth) maxGtNameLegth = strlen(gateTagStack[gtCount]); // 가장 긴 게이트 태그의 길이 업데이트

                    gtCount++; // 게이트 태그 카운터 증가
                }
            }
        }
        if (strcmp(carDataList[index].location, locationStack[locCount - 1]) != 0)
        {
            char** temp = (char**)realloc(locationStack, sizeof(char*) * (locCount + 1));
            if (temp != NULL) {
                locationStack = temp;
                locationStack[locCount] = (char*)malloc(sizeof(char) * (strlen(carDataList[index].location) + 1));
                if (locationStack[locCount] != NULL) {
                    strcpy(locationStack[locCount], carDataList[index].location);
                    if (strlen(locationStack[locCount]) > maxLocNameLegth) maxLocNameLegth = strlen(locationStack[locCount]);

                    locCount++;
                }
            }
        }
    }
    // ----------------------------------------------------------------- (게이트 태그 & 위치 중복 제거) end

    // filter들 각 stack크기에 맞춰 메모리 할당 및 초기화
    bool* carDataFilter = (bool*)malloc(carCount * sizeof(bool));
    for (int i = 0; i < carCount; i++)   carDataFilter[i] = false;

    bool* gateTagFilter = (bool*)malloc(gtCount * sizeof(bool));
    for (int i = 0; i < gtCount; i++)  gateTagFilter[i] = false;

    bool* locationFilter = (bool*)malloc(locCount * sizeof(bool));
    for (int i = 0; i < locCount; i++)  locationFilter[i] = false;

    char* l_PPH = NULL;
    char* h_PPH = NULL;
    char* l_PPK = NULL;
    char* h_PPK = NULL;

    // 예약
    int selectedCarIndex = -1;
    int* reserveCarIndex = NULL;

    system("cls");

    int x = 0;
    while (true)
    {

        if (gotoxy(1, 0)) {
            printf("%s | ", user.name);

            if (cy == -1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WHITE << 4) + BLACK); // set
            printf("[사용자 정보 보기]");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE); // reset
        }

        if (gotoxy(0, 3))
            printf("================== [필터] ==================\n");

        int y1 = (gtCount / 4) + (int)(gtCount % 4 != 0); // 0 ~ gt stack
        int y2 = y1 + (locCount / 3) + (int)(locCount % 3 != 0); // ~ loc stack
        int y3 = y2 + (carCount / 3) + (int)(carCount % 3 != 0); // ~ car stack
        int sizeX = 0;

        if (gotoxy(0, 5))
            printf(" - 게이트\n");

        for (int index = 0; index < gtCount; index++)
        {
            Text_Filter(
                gateTagStack[index],
                x == index % 4 && cy == index / 4, gateTagFilter[index],
                sizeX, 6 + (index / 4)
            );
            sizeX += maxGtNameLegth + 2;
            if ((index + 1) % 4 == 0) sizeX = 0;
        }
        sizeX = 0;

        if (gotoxy(0, 7 + y1))
            printf(" - 위치\n");

        for (int index = 0; index < locCount; index++)
        {
            Text_Filter(
                locationStack[index],
                x == index % 3 && cy == y1 + index / 3, locationFilter[index],
                sizeX, 8 + y1 + index / 3
            );
            sizeX += maxLocNameLegth + 2;
            if ((index + 1) % 3 == 0) sizeX = 0;
        }
        sizeX = 0;

        if (gotoxy(0, 9 + y2))
            printf(" - 차종\n");
        for (int index = 0; index < carCount; index++)
        {
            Text_Filter(
                carDataStack[index],
                x == index % 3 && cy == y2 + index / 3, carDataFilter[index],
                sizeX, 10 + y2 + index / 3
            );
            sizeX += maxCarNameLegth + 2;
            if ((index + 1) % 3 == 0) sizeX = 0;
        }
        sizeX = 0;

        if (gotoxy(0, 11 + y3))
            printf(" - 가격\n");

        if (gotoxy(2, 12 + y3))
            printf("Price per hour     \n");
        Text_select("최소:", x == 0 && cy == y3,
            4, 13 + y3);
        if (l_PPH != NULL && !(x == 0 && cy == y3)) {
            if (gotoxy(10, 13 + y3)) printf("%s", l_PPH);
        }

        Text_select("최대:", x == 0 && cy == y3 + 1,
            4, 14 + y3);
        if (h_PPH != NULL && !(x == 0 && cy == y3 + 1)) {
            if (gotoxy(10, 14 + y3)) printf("%s", h_PPH);
        }

        if (gotoxy(2, 15 + y3))
            printf("Price per km     ");
        Text_select("최소:", x == 0 && cy == y3 + 2,
            4, 16 + y3);
        if (l_PPK != NULL && !(x == 0 && cy == y3 + 2)) {
            if (gotoxy(10, 16 + y3)) printf("%s", l_PPK);
        }

        Text_select("최대:", x == 0 && cy == y3 + 3,
            4, 17 + y3);
        if (h_PPK != NULL && !(x == 0 && cy == y3 + 3)) {
            if (gotoxy(10, 17 + y3)) printf("%s", h_PPK);
        }

        Text_select("[필터 초기화]", cy == y3 + 4, 1, 19 + y3);

        if (gotoxy(0, 21 + y3))
            printf("============== [차량 리스트] ==============");


        selectedCarIndex = -1;

        int index;
        int count = 0; // 차랑 필터링 후 남은 종류
        for (index = 0; index < _msize(carDataList) / sizeof(CarData); index++)
        {
            int filter = 0; // 필터 초기화

            // 게이트 태그 필터링
            for (int i = 0; i < _msize(gateTagFilter) / sizeof(bool); i++) {
                if (gateTagFilter[i]) {
                    filter |= 1 << 3; // 필터 비트 설정
                    if (strcmp(gateTagStack[i], carDataList[index].gateTag) == 0) {
                        filter |= 1 << 0; // 필터 비트 설정
                        break;
                    }
                }
            }

            // 위치 필터링
            for (int i = 0; i < _msize(locationFilter) / sizeof(bool); i++) {
                if (locationFilter[i]) {
                    filter |= 1 << 4; // 필터 비트 설정
                    if (strcmp(locationStack[i], carDataList[index].location) == 0) {
                        filter |= 1 << 1; // 필터 비트 설정
                        break;
                    }
                }
            }

            // 차량 데이터 필터링
            for (int i = 0; i < _msize(carDataFilter) / sizeof(bool); i++) {
                if (carDataFilter[i]) {
                    filter |= 1 << 5; // 필터 비트 설정
                    if (strcmp(carDataStack[i], carDataList[index].name) == 0) {
                        filter |= 1 << 2; // 필터 비트 설정
                        break;
                    }
                }
            }

            // PPH 범위 필터링
            bool inRange = false;
            int min = 0, max = -1; // 무한대
            if (l_PPH != NULL) {
                if (strcmp(l_PPH, "\0") != 0) {
                    min = atoi(l_PPH);
                }
            }
            if (h_PPH != NULL) {
                if (strcmp(h_PPH, "\0") != 0) {
                    max = atoi(h_PPH);
                }
            }

            if (min <= carDataList[index].PPH) {
                if (max >= 0) {
                    if (carDataList[index].PPH <= max) {
                        filter |= 1 << 6; // 필터 비트 설정
                    }
                }
                else filter |= 1 << 6; // 필터 비트 설정
            }

            // PPK 범위 필터링
            min = 0; max = -1;
            if (l_PPK != NULL) {
                if (strcmp(l_PPK, "\0") != 0) {
                    min = atoi(l_PPK);
                }
            }
            if (h_PPK != NULL) {
                if (strcmp(h_PPK, "\0") != 0) {
                    max = atoi(h_PPK);
                }
            }

            if (min <= carDataList[index].PPK) {
                if (max >= 0) {
                    if (carDataList[index].PPK <= max) {
                        filter |= 1 << 7; // 필터 비트 설정
                    }
                }
                else filter |= 1 << 7; // 필터 비트 설정
            }

            // 필터링된 값에서 필터링 비트를 추출하여 임시 필터 변수에 저장
            int tempFilter = filter & 0b00111111;

            // 필터링된 값에서 다시 필터링 비트를 제거하고 반전하여 필터링되지 않은 비트를 추출한 후, 필터 변수에 할당
            filter = ~((tempFilter ^ (tempFilter >> 3)) | ~(filter | 0b00111111));

            // 필터된 값이 특정 상태인지 확인하여 처리
            if ((filter & 0b11000111) == 0b11000111) {
                // 특정 출력 위치에 따라 텍스트 색상 변경
                if (cy == y3 + 5 + count) {
                    selectedCarIndex = index;
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WHITE << 4) + BLACK); // 텍스트 색상 설정   
                }

                // 예약된 차량이 특정 출력 위치에 따라 텍스트 색상 변경
                if (reserveCarIndex != NULL) {
                    for (int i = 0; i < _msize(reserveCarIndex) / sizeof(int); i++) {
                        if (reserveCarIndex[i] == carDataList[index].id) {
                            if (cy == y3 + 5 + count) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (GRAY << 4) + BLACK); // 텍스트 색상 설정   
                            else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (darkBLUE << 4) + WHITE); // 텍스트 색상 설정   
                            break;
                        }
                    }
                }

                // 출력 카운트 증가
                ++count;

                // 출력
                if (gotoxy(0, 23 + y3 + (count - 1) * 3))
                    printf("%d. %s | %s"
                        , count
                        , carDataList[index].gateTag
                        , carDataList[index].location);

                // 출력
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE); // 텍스트 색상 리셋

                // 출력
                if (gotoxy(0, 23 + y3 + (count - 1) * 3 + 1))
                    printf("    %s | (%d원/h / %d원/km)"
                        , carDataList[index].name
                        , carDataList[index].PPH
                        , carDataList[index].PPK);
            }
        }

        if (gotoxy(0, 23 + y3 + count * 3))
            printf("==========================================");

        if (y3 <= cy && cy <= y3 + 1)  gotoxy(10, 13 + y3 + (cy - y3));
        else if (cy <= y3 + 3)              gotoxy(10, 14 + y3 + (cy - y3));


        Key key = Key_none;

        if (cy < y3) {
            key = input_key_grid(&x, &cy);
        }
        else if (cy < y3 + 4) {
            if (cy == y3)     key = input_stack(&l_PPH, NULL, 0, KeyFilter_number);
            else if (cy == y3 + 1) key = input_stack(&h_PPH, NULL, 0, KeyFilter_number);

            if (cy == y3 + 2) key = input_stack(&l_PPK, NULL, 0, KeyFilter_number);
            else if (cy == y3 + 3) key = input_stack(&h_PPK, NULL, 0, KeyFilter_number);

            if (key == Key_up)                         --cy;
            else if (key == Key_down || key == Key_enter) {
                key = Key_down;
                ++cy;
            }
        }
        else {
            key = input_key(NULL, 0);

            if (key == Key_up)          --cy;
            else if (key == Key_down)   ++cy;
        }

        if (key == Key_enter)
        {
            if (cy == -1) {
                system("cls");

                printf("[사용자 정보] - 엔터를 눌러서 종료\n\n");

                printf("          이름 | %s\n\n", user.name);

                printf(" 운전면허 번호 | %s\n", user.RRN);
                printf("  주민등록번호 | %s\n\n", user.studentId);

                printf("        아이디 | %s\n", user.id);
                printf("      패스워드 | %s\n\n", user.password);

                printf(" ======== 예약목록 ========\n\n");

                if (reserveCarIndex != NULL) {
                    int size = _msize(reserveCarIndex) / sizeof(int);
                    if (size == 0) {

                    }
                    else {
                        for (int count = 0; count < size; count++)
                        {
                            int index = reserveCarIndex[count];

                            printf("%d. %s | %s\n"
                                , count + 1
                                , carDataList[index].gateTag
                                , carDataList[index].location);

                            printf("    %s | (%d원/h / %d원/km)\n\n"
                                , carDataList[index].name
                                , carDataList[index].PPH
                                , carDataList[index].PPK);
                        }
                    }
                }
                else {
                    printf("       [예약목록없음]       \n\n");
                }

                printf(" ==========================");

                Key key = Key_none;
                while (key != Key_enter) // 엔터 입력 기다리기
                    key = input_key(NULL, 0);

                system("cls");
            }

            else if (cy <= y1 - 1)
                gateTagFilter[cy * 4 + x] = !gateTagFilter[cy * 4 + x];

            else if (cy <= y2 - 1)
                locationFilter[(cy - y1) * 3 + x] = !locationFilter[(cy - y1) * 3 + x];

            else if (cy <= y3 - 1)
                carDataFilter[(cy - y2) * 3 + x] = !carDataFilter[(cy - y2) * 3 + x];

            else if (cy == y3 + 4) { // 필터 초기화
                for (int i = 0; i < gtCount; i++)   gateTagFilter[i] = false;
                for (int i = 0; i < locCount; i++)  locationFilter[i] = false;
                for (int i = 0; i < carCount; i++)  carDataFilter[i] = false;
                l_PPH = h_PPH = NULL;
                l_PPK = h_PPK = NULL;
                system("cls");
            }

            else if (cy > y3 + 4) { // 차 선택
                if (selectedCarIndex != -1) { // 현재 선택중인 차량이 있는지 확인

                    bool isNew = true; // 새로운 예약 여부 초기화
                    if (reserveCarIndex != NULL) { // 예약된 차량이 있는지 확인

                        int size = _msize(reserveCarIndex) / sizeof(int); // 예약된 차량 배열의 크기 확인

                        // 예약된 차량 배열을 반복하면서 선택된 차량이 이미 예약되어 있는지 확인
                        for (int i = 0; i < size; i++) {
                            if (reserveCarIndex[i] == carDataList[selectedCarIndex].id) { // 선택된 차량이 이미 예약된 경우
                                isNew = false; // 새로운 예약이 아님

                                // 예약된 차량 배열에서 선택된 차량을 제거하고 배열을 재조정
                                int size = _msize(reserveCarIndex) / sizeof(int);

                                if (size == 1) reserveCarIndex = NULL; // 배열에 남은 요소가 없는 경우 NULL로 설정
                                else {
                                    int* temp = (int*)malloc(sizeof(int) * (size - 1)); // 새로운 배열 할당

                                    // 예약된 차량 배열을 순회하면서 선택된 차량을 제외하고 새로운 배열에 복사
                                    for (int i = 0, count = 0; i < size; i++) {
                                        if (reserveCarIndex[i] != carDataList[selectedCarIndex].id) {
                                            temp[count++] = reserveCarIndex[i];
                                            if (count >= size - 1) break;
                                        }
                                    }

                                    if (temp != NULL) {
                                        reserveCarIndex = temp; // 새로운 배열을 예약된 차량 배열에 할당
                                        reserveCarIndex[size - 1] = carDataList[selectedCarIndex].id; // 선택된 차량을 배열에 추가
                                    }
                                }
                                break;
                            }
                        }

                        // 새로운 예약인 경우
                        if (isNew) {
                            // 예약된 차량 배열의 크기를 증가시키고 선택된 차량을 추가
                            int* temp = (int*)realloc(reserveCarIndex, sizeof(int) * (size + 1));

                            if (temp != NULL) {
                                reserveCarIndex = temp; // 새로운 배열을 예약된 차량 배열에 할당
                                reserveCarIndex[size] = carDataList[selectedCarIndex].id; // 선택된 차량을 배열에 추가
                            }
                        }
                    }
                    else { // 예약된 차량이 없는 경우
                        // 예약된 차량 배열을 생성하고 선택된 차량을 추가
                        reserveCarIndex = (int*)malloc(sizeof(int));
                        reserveCarIndex[0] = carDataList[selectedCarIndex].id;
                    }

                    // 콘솔 화면을 지우고 선택된 차량 정보 출력
                    system("cls");

                    printf("%s | %s\n"
                        , carDataList[selectedCarIndex].gateTag
                        , carDataList[selectedCarIndex].location);

                    printf("    %s | (%d원/h / %d원/km)\n\n"
                        , carDataList[selectedCarIndex].name
                        , carDataList[selectedCarIndex].PPH
                        , carDataList[selectedCarIndex].PPK);

                    // 예약 상태에 따라 메시지 출력
                    if (isNew) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLUE << 4) + WHITE); // 색상 설정
                        printf("예약이 완료 되었습니다.");
                    }
                    else {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (RED << 4) + WHITE); // 색상 설정
                        printf("예약이 취소 되었습니다.");
                    }

                    // 출력 형식 초기화
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE); // 색상 리셋
                    printf("     "); // 공백 출력

                    // 확인 메시지 출력
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WHITE << 4) + BLACK); // 색상 설정
                    printf("확인"); // 확인 메시지 출력

                    // 출력 형식 초기화
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE); // 색상 리셋

                    // 사용자 입력 대기
                    Key key = Key_none;
                    while (key != Key_enter)
                        key = input_key(NULL, 0);

                    // 콘솔 화면을 지움
                    system("cls");
                }
            }
        }
        else
        {
            int lastPage = csbi.dwSize.Y * (int)(csy / csbi.dwSize.Y);

            if (cy < -1) cy = -1;

            else if (cy == -1) {
                x = 0;
                csy = 0;
            }

            else if (cy > y3 + 4 + count) {
                cy = y3 + 4 + count;
            }

            else if (cy <= y1 - 1) {
                csy = 6 + cy;
                if (cy == y1 - 1 && gtCount % 4 != 0) {
                    int endIndex = gtCount % 4 - 1;
                    if (x < 0)         x = endIndex;
                    else if (x > endIndex)  x = 0;
                }
                else {
                    if (x < 0) x = 3;
                    else if (x > 3) x = 0;
                }
            }

            else if (cy <= y2 - 1) {
                csy = 7 + y1 + cy;
                if (cy == y2 - 1 && locCount % 3 != 0) {
                    int endIndex = locCount % 3 - 1;
                    if (x < 0)              x = endIndex;
                    else if (x > endIndex)  x = 0;
                }
                else {
                    if (x < 0) x = 2;
                    else if (x > 2) x = 0;
                }
            }

            else if (cy <= y3 - 1) {
                csy = 6 + y2 + cy;
                if (cy == y3 - 1 && carCount % 3 != 0) {
                    int endIndex = carCount % 3 - 1;
                    if (x < 0)              x = endIndex;
                    else if (x > endIndex)  x = 0;
                }
                else {
                    if (x < 0) x = 2;
                    else if (x > 2) x = 0;
                }
            }

            else {
                x = 0;
                if (cy <= y3 + 1)
                    csy = 2 + y3 + cy;
                else if (cy <= y3 + 3)
                    csy = 3 + y3 + cy;
                else if (cy == y3 + 4)
                    csy = 4 + y3 + cy;
                else
                    csy = 12 + y3 * 2 + ((cy - (y3 + 5)) * 3);
            }

            // 이전에 계산된 페이지가 이동한 페이지와 다르면
            if (lastPage != csbi.dwSize.Y * (int)(csy / csbi.dwSize.Y))
                system("cls"); // 화면 지우기
        }

        GetConsoleScreenBufferInfo(hConsole, &csbi); // 콘솔 화면 버퍼 정보 가져오기

        // 이전 크기와 현재 크기가 다른 경우
        if (last_dwSize.X != csbi.dwSize.X || last_dwSize.Y != csbi.dwSize.Y) {
            system("cls"); // 화면 지우기
            last_dwSize = csbi.dwSize; // 이전 크기 업데이트
        }
    }

    int size = _msize(carDataList) / sizeof(CarData);
    for (int i = 0; i < size; i++) {
        free(carDataList[i].gateTag);
        free(carDataList[i].location);
        free(carDataList[i].name);
    }
    free(carDataList);

    // carDataStack 메모리 해제
    size = _msize(carDataStack) / sizeof(char*);
    for (int i = 0; i < size; i++) {
        free(carDataStack[i]);
    }
    free(carDataStack);

    // gateTagStack 메모리 해제
    size = _msize(gateTagStack) / sizeof(char*);
    for (int i = 0; i < size; i++) {
        free(gateTagStack[i]);
    }
    free(gateTagStack);

    // locationStack 메모리 해제
    size = _msize(locationStack) / sizeof(char*);
    for (int i = 0; i < size; i++) {
        free(locationStack[i]);
    }
    free(locationStack);

    free(gateTagFilter);
    free(locationFilter);
    free(carDataFilter);

    free(l_PPH);
    free(h_PPH);
    free(l_PPK);
    free(h_PPK);

    // reserveCarIndex 해제
    free(reserveCarIndex);
    return 0;
}


CarData* get_car_data_list()
{
    // 파일 포인터 선언
    FILE* file;

    // 파일 열기
    fopen_s(&file, "lental car data.txt", "r");

    // 파일 열기에 실패한 경우
    if (file == NULL) {
        fprintf(stderr, "파일 '%s'를 열 수 없습니다.\n", "lental car data.txt"); // 에러 출력
        return NULL;
    }

    char line[100]; // 라인 버퍼를 저장할 변수

    fgets(line, sizeof(line), file);
    int size = atoi(line); // 자동차 개수
    CarData* data = (CarData*)malloc(sizeof(CarData) * size);

    // 파일 끝까지 한 줄씩 읽어들임
    CarDataTag tag = CarDataTag_none;
    char gatetag[100]; // 게이트 위치를 저장할 버퍼
    char location[100]; // 위치 저장할 버퍼
    int count = 0; // 불러온 자동차 카운트
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strcmp(line, "tag\n") == 0) // "tag\n" 문자열이 포함된 라인인지 확인
        {
            tag = CarDataTag_gateTag; // tag를 게이트 태그로 설정 ("tag\n" 다음줄)
        }
        else if (tag == CarDataTag_gateTag) // 현재 tag가 게이트 태그인 경우
        {
            line[strcspn(line, "\n")] = '\0'; // 개행 문자를 널 문자로 변경하여 라인의 끝을 표시
            strcpy(gatetag, line); // 게이트 태그 값을 복사
            tag = CarDataTag_none; // tag를 기본값으로 재설정
        }

        if (strcmp(line, "loc\n") == 0) // "loc\n" 문자열이 포함된 라인인지 확인
        {
            tag = CarDataTag_location; // tag를 위치로 설정
        }
        else if (tag == CarDataTag_location) // 현재 tag가 위치인 경우  ("loc\n" 다음줄)
        {
            line[strcspn(line, "\n")] = '\0'; // 개행 문자를 널 문자로 변경하여 라인의 끝을 표시
            strcpy(location, line); // 위치 값을 복사
            tag = CarDataTag_none; // tag를 기본값으로 재설정
        }

        if (strcmp(line, "car\n") == 0) // "car\n" 문자열이 포함된 라인인지 확인
        {
            // 게이트 태그와 위치 값을 할당하여 데이터에 저장
            data[count].gateTag = (char*)malloc(sizeof(char) * (strlen(gatetag) + 1));
            strcpy(data[count].gateTag, gatetag);

            data[count].location = (char*)malloc(sizeof(char) * (strlen(location) + 1));
            strcpy(data[count].location, location);

            tag = CarDataTag_name; // tag를 차 이름으로 설정
        }
        else if (tag == CarDataTag_name) // 현재 tag가 차 이름인 경우
        {
            line[strcspn(line, "\n")] = '\0'; // 개행 문자를 널 문자로 변경하여 라인의 끝을 표시
            data[count].name = (char*)malloc(strlen(line) + 1); // 메모리 할당하여 차 이름 저장
            strcpy(data[count].name, line); // 차 이름 값을 복사
            tag++; // 다음 tag로 이동
        }
        else if (tag == CarDataTag_PPH) // 현재 tag가 시간당 가격인 경우
        {
            data[count].PPH = atoi(line); // 문자열을 정수로 변환하여 시간당 가격 저장
            tag++; // 다음 tag로 이동
        }
        else if (tag == CarDataTag_PPK) // 현재 tag가 킬로미터당 가격인 경우
        {
            data[count].PPK = atoi(line); // 문자열을 정수로 변환하여 킬로미터당 가격 저장
            data[count].id = count; // 데이터 ID 설정
            tag = CarDataTag_none; // tag를 기본값으로 재설정
            count++; // 다음 데이터로 이동

            if (size <= count) break; // 배열 크기를 초과하는 경우 반복문 종료
        }

    }

    // 파일 닫기
    fclose(file);

    return data;
}

UserData* get_user_data_list()
{
    // 파일 포인터 선언
    FILE* file;

    // 파일 열기
    fopen_s(&file, "userdata.txt", "r");

    // 파일 열기에 실패한 경우
    if (file == NULL) {
        fprintf(stderr, "파일 '%s'를 열 수 없습니다.\n", "userdata.txt"); // 에러 출력
        return NULL;
    }

    char line[100]; // 라인 버퍼를 저장할 변수

    fgets(line, sizeof(line), file);
    int size = atoi(line); // 자동차 개수
    UserData* data = (UserData*)malloc(sizeof(UserData) * size);

    // 파일 끝까지 한 줄씩 읽어들임
    UserDataTag tag = UserDataTag_none;
    int count = 0; // 불러온 자동차 카운트
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strcmp(line, "\n") == 0) // 빈 줄인지 확인
        {
            tag = UserDataTag_name; // 태그를 이름으로 설정
        }
        else // 빈 줄이 아닌 경우
        {
            line[strcspn(line, "\n")] = '\0'; // 개행 문자를 널 문자로 변경하여 라인의 끝을 표시
            if (tag == UserDataTag_name) // 현재 태그가 이름인 경우
            {
                // 이름을 할당하여 데이터에 저장
                data[count].name = (char*)malloc(strlen(line) + 1);
                strcpy(data[count].name, line);
                tag++; // 다음 태그로 이동
            }
            else if (tag == UserDataTag_license) // 현재 태그가 면허증 번호인 경우
            {
                // 면허증 번호를 할당하여 데이터에 저장
                data[count].license = (char*)malloc(strlen(line) + 1);
                strcpy(data[count].license, line);
                tag++; // 다음 태그로 이동
            }
            else if (tag == UserDataTag_RRN) // 현재 태그가 주민등록번호인 경우
            {
                // 주민등록번호를 할당하여 데이터에 저장
                data[count].RRN = (char*)malloc(strlen(line) + 1);
                strcpy(data[count].RRN, line);
                tag++; // 다음 태그로 이동
            }
            else if (tag == UserDataTag_studentId) // 현재 태그가 학번인 경우
            {
                // 학번을 할당하여 데이터에 저장
                data[count].studentId = (char*)malloc(strlen(line) + 1);
                strcpy(data[count].studentId, line);
                tag++; // 다음 태그로 이동
            }
            else if (tag == UserDataTag_id) // 현재 태그가 ID인 경우
            {
                // ID를 할당하여 데이터에 저장
                data[count].id = (char*)malloc(strlen(line) + 1);
                strcpy(data[count].id, line);
                tag++; // 다음 태그로 이동
            }
            else if (tag == UserDataTag_password) // 현재 태그가 비밀번호인 경우
            {
                // 비밀번호를 할당하여 데이터에 저장
                data[count].password = (char*)malloc(strlen(line) + 1);
                strcpy(data[count].password, line);
                tag = UserDataTag_none; // 태그를 기본값으로 재설정
                count++; // 다음 데이터로 이동
            }
        }

        if (size <= count) break; // 배열 크기를 초과하는 경우 반복문 종료

    }

    // 파일 닫기
    fclose(file);

    return data;
}


// 1: id 불일치, 2: password만 불일치, 3: 모두 일치
int userFound(UserData* data, char* id, char* password)
{
    int foundMask = 0;

    for (int index = 0; index < _msize(userDataList) / sizeof(UserData); index++) // 사용자 데이터 목록 반복
    {
        // 아이디 확인
        if (id != NULL) { // 입력된 아이디가 있는 경우
            if (strcmp(userDataList[index].id, id) == 0) { // 사용자 데이터의 아이디와 입력된 아이디가 일치하는지 확인
                // 비밀번호 확인
                if (password != NULL) { // 입력된 비밀번호가 있는 경우
                    if (strcmp(userDataList[index].password, password) == 0) { // 사용자 데이터의 비밀번호와 입력된 비밀번호가 일치하는지 확인
                        *data = userDataList[index]; // 일치하는 사용자 데이터를 data에 할당
                        foundMask = 3; // 아이디와 비밀번호 모두 일치
                        break; // 검색 종료
                    }
                    else {
                        foundMask = 2; // 아이디는 일치하지만 비밀번호가 일치하지 않음
                        break; // 검색 종료
                    }
                }
                else {
                    foundMask = 2; // 아이디는 일치하지만 비밀번호가 입력되지 않음
                    break; // 검색 종료
                }
            }
            else foundMask = 1; // 입력된 아이디와 일치하는 사용자 데이터를 찾지 못함
        }
        else foundMask; // 입력된 아이디가 없는 경우
    }

    return foundMask;
}

bool user_login(UserData* data)
{
    char* id = NULL;
    char* password = NULL;

    int foundMask = 0;

    system("cls");

    int stack = 0;
    while (true)
    {

        Text_select("아이디:", stack == 0, 40, 0);
        Text_error("아이디가 일치하지 않습니다. >> ", foundMask == 1, 8, 0);

        Text_select("비밀번호:", stack == 1, 38, 1);
        Text_error("비밀번호가 일치하지 않습니다. >> ", foundMask == 2, 4, 1);

        Text_select("로그인", stack == 2, 44, 3);
        Text_select("회원가입", stack == 3, 43, 4);

        gotoxy(48, stack);

        Key key;
        if (stack == 0) key = input_stack(&id, &stack, 3, KeyFilter_no_korean);
        else if (stack == 1) key = input_stack(&password, &stack, 3, KeyFilter_no_korean);
        else {
            key = input_key(&stack, 3);
        }

        if (key == Key_enter)
        {
            if (stack == 0) return false; // 회원가입

            foundMask = userFound(data, id, password);
            if (foundMask == 3) {
                return true;
            }
            else stack = foundMask - 1;
        }
    }
}

bool user_join(UserData* data)
{
    int errCode = 0;

    system("cls");

    int stack = 1;
    while (true)
    {

        Text_select("< 뒤로", stack == 0, 0, 0);

        Text_select("이름:", stack == 1, 52, 1);
        Text_error("이름을 입력하세요. >>", errCode == 1, 28, 1);

        Text_select("아이디:", stack == 2, 50, 2);
        Text_error("사용중인 아이디가 존재합니다. >>", errCode == 2, 16, 2);
        if (errCode == 12) Text_error("아이디를 입력하세요. >>", errCode == 12, 24, 2);

        Text_select("비밀번호:", stack == 3, 48, 3);
        Text_error("비밀번호를 입력하세요. >>", errCode == 3, 20, 3);

        Text_select("운전면허 번호:", stack == 4, 43, 4);
        Text_error("올바르게 입력하세요(10자리). >>", errCode == 4, 8, 4);

        Text_select("주민등록번호(뒤 1자리까지):", stack == 5, 30, 5);
        Text_error("올바르게 입력하세요. >>", errCode == 5, 4, 5);

        Text_select("학번:", stack == 6, 52, 6);
        Text_error("올바르게 입력하세요(10자리). >>", errCode == 6, 18, 6);

        Text_select("회원가입", stack == 7, 53, 8);


        gotoxy(58, stack);

        Key key;
        if (stack == 0) key = input_key(&stack, 7);
        else if (stack == 1) {
            printf("                   ");
            gotoxy(58, stack);
            key = input_stack(&(data->name), &stack, 7, KeyFilter_none);
        }
        else if (stack == 2) key = input_stack(&(data->id), &stack, 7, KeyFilter_no_korean);
        else if (stack == 3) key = input_stack(&(data->password), &stack, 7, KeyFilter_no_korean);
        else if (stack == 4) key = input_stack(&(data->license), &stack, 7, KeyFilter_number);
        else if (stack == 5) key = input_stack(&(data->RRN), &stack, 7, KeyFilter_number);
        else if (stack == 6) key = input_stack(&(data->studentId), &stack, 7, KeyFilter_number);
        else if (stack == 7) key = input_key(&stack, 7);

        if (key == Key_enter)
        {
            // ----------------------------------------------------------------- 뒤로
            if (stack - 1 == 0) return false;

            // ----------------------------------------------------------------- name 확인
            if (stack - 1 == -1 || stack - 1 == 1) {
                if (data->name != NULL) {
                    if (strlen(data->name) == 0) errCode = stack = 1;
                    else errCode = 0;
                }
                else errCode = stack = 1;
            }

            // ----------------------------------------------------------------- id 확인
            if (stack - 1 == -1 || stack - 1 == 2) {
                if (data->id != NULL)
                {
                    if (strlen(data->id) == 0) {
                        stack = 2;
                        errCode = 12;
                    }
                    else if (userFound(data, data->id, NULL) == 2) { // 아이디 존재
                        errCode = stack = 2;
                    }
                    else errCode = 0;
                }
                else errCode = stack = 2;
            }

            // ----------------------------------------------------------------- password 확인
            if (stack - 1 == -1 || stack - 1 == 3) {
                if (data->password != NULL) {
                    if (strlen(data->password) == 0) errCode = stack = 3;
                    else errCode = 0;
                }
                else errCode = stack = 3;
            }

            // ----------------------------------------------------------------- license 확인
            if (stack - 1 == -1 || stack - 1 == 4) {
                if (data->license != NULL)
                {
                    if (strlen(data->license) != 10) errCode = stack = 4;
                    else errCode = 0;
                }
                else errCode = stack = 4;
            }

            // ----------------------------------------------------------------- RRN 확인
            if (stack - 1 == -1 || stack - 1 == 5) {
                if (data->RRN != NULL)
                {
                    if (strlen(data->RRN) != 7) errCode = stack = 5;
                    else errCode = 0;
                }
                else errCode = stack = 5;
            }

            // ----------------------------------------------------------------- studentId 확인
            if (stack - 1 == -1 || stack - 1 == 6) {
                if (data->studentId != NULL)
                {
                    if (strlen(data->studentId) != 10) errCode = stack = 6;
                    else errCode = 0;
                }
                else errCode = stack = 6;
            }

            // ----------------------------------------------------------------- 회원가입 확인
            if (stack - 1 == -1 && errCode == 0) return true;
        }
    }


    return false;
}

bool save_user_data(const UserData data, int size)
{
    // 파일 포인터 선언
    FILE* file;

    // 파일 열기 (기존 데이터를 보호한 채 첫줄에 적기 위해 r+를 사용한다)
    fopen_s(&file, "userdata.txt", "r+");

    // 파일 열기에 실패한 경우
    if (file == NULL) {
        fprintf(stderr, "파일 '%s'를 열 수 없습니다.\n", "userdata.txt"); // 에러 출력
        return false;
    }
    fprintf(file, "%d\n", size);

    // 파일 닫기
    fclose(file);

    // 파일 열기
    fopen_s(&file, "userdata.txt", "a");

    // 데이터 파일에 쓰기
    fprintf(file, "\n%s\n", data.name);
    fprintf(file, "%s\n", data.license);
    fprintf(file, "%s\n", data.RRN);
    fprintf(file, "%s\n", data.studentId);
    fprintf(file, "%s\n", data.id);
    fprintf(file, "%s\n", data.password);

    // 파일 닫기
    fclose(file);

    return true;
}