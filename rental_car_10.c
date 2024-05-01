#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
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
    char* id; // 아이다
    char* password; // 패스워드
} UserData;

typedef int CarDataTag;
enum CarDataTag_
{
    CarDataTag_none = 0,
    CarDataTag_gateTag = 1,
    CarDataTag_location = 2,
    CarDataTag_name = 3,
    CarDataTag_PPH = 4,
    CarDataTag_PPK = 5,

};

typedef int UserDataTag;
enum UserDataTag_
{
    UserDataTag_none = 0,
    UserDataTag_name = 1,
    UserDataTag_license = 2,
    UserDataTag_RRN = 3,
    UserDataTag_studentId = 4,
    UserDataTag_id = 5,
    UserDataTag_passward = 6,

};

typedef int Key;
enum Key_
{
    Key_none = 0,

    Key_up = 72,
    Key_down = 80,
    Key_left = 75,
    Key_right = 77,

    Key_enter = 100,
};

typedef int KeyFilter;
enum KeyFilter_
{
    KeyFilter_none,
    KeyFilter_number,
};

enum ColorType {
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
CarData* carDataList = NULL;
UserData* userDataList = NULL;

//
CarData* get_car_data_list();
UserData* get_user_data_list();

int userFound(UserData* data, char* id, char* password);
bool user_login(UserData* data);
bool user_join(UserData* data);
bool save_user_data(const UserData data, int size);

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //커서숨기는 함수에 사용되는 열거형 
void setcursortype(CURSOR_TYPE c) { //커서숨기는 함수 
    CONSOLE_CURSOR_INFO CurInfo;

    switch (c) {
    case NOCURSOR:
        CurInfo.dwSize = 1;
        CurInfo.bVisible = FALSE;
        break;
    case SOLIDCURSOR:
        CurInfo.dwSize = 100;
        CurInfo.bVisible = TRUE;
        break;
    case NORMALCURSOR:
        CurInfo.dwSize = 20;
        CurInfo.bVisible = TRUE;
        break;
    }
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void gotoxy(int x, int y) {
    //x, y 좌표 설정
    COORD pos = { x,y };
    //커서 이동
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Text_select(char const* const _Format, bool selected, int x, int y)
{
    gotoxy(x, y);

    if (selected) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WHITE << 4) + BLACK); // set

    printf(_Format);

    if (selected) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE); // reset
}
void Text_Filter(char const* const _Format, bool selected, bool filtered, int x, int y)
{
    gotoxy(x, y);

    if (selected) {
        if (filtered)        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (GRAY << 4) + BLACK); // selected & filtered
        else                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WHITE << 4) + BLACK); // selected
    }
    else if (filtered)     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (DarkGreen << 4) + BLACK); // filtered

    printf(_Format);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE); // reset
}
void Text_error(char const* const _Format, bool error, int x, int y)
{
    gotoxy(x, y);

    if (error) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + RED); // set
        printf(_Format);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE); // reset
    }
    else {
        for (int i = 0; i < strlen(_Format); i++) printf(" ");
    }
}


Key input(char** str, KeyFilter filter)
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
                    && !(48 <= (int)ch && (int)ch <= 57))
                {
                    continue;
                }
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

Key input_stack(char** str, int* stack, int maxStack, KeyFilter filter)
{
    Key key = input(str, filter);

    if (stack != NULL) {
        if (key == Key_up)     --(*stack);
        else if (key == Key_down || key == Key_enter)   ++(*stack);

        if (*stack < 0)         *stack = maxStack;
        else if (*stack > maxStack)  *stack = 0;
    }

    return key;
}

Key input_key(int* stack, int maxStack)
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

Key input_key_grid(int* x, int* y)
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

    userDataList = get_user_data_list();

    if (!userDataList)
    {
        printf("error get_user_data_list\n");
        return -2;
    }
    // ----------------------------------------------------------------- (초기 정보 불러오기) end


    // ----------------------------------------------------------------- (로그인 & 회원가입) start
    UserData user = { NULL, NULL, NULL, NULL,  NULL, NULL };
    while (!user_login(&user))
    {
        // ----------------------------------------------------------------- (회원가입) start
        UserData newUser = { NULL, NULL, NULL, NULL,  NULL, NULL };
        if (user_join(&newUser))
        {
            int size = (_msize(userDataList) / sizeof(UserData)) + 1;
            if (!save_user_data(newUser, size))
            {
                printf("error save_user_data\n");
                return -3;
            }

            userDataList = (UserData*)realloc(userDataList, sizeof(UserData) * size);
            userDataList[size - 1] = newUser;
        }
        // ----------------------------------------------------------------- (회원가입) end
    }
    // ----------------------------------------------------------------- (로그인 & 회원가입) end


    // ----------------------------------------------------------------- (자동차 중복 제거) start
    char** carDataStack = (char**)malloc(sizeof(char*));

    carDataStack[0] = (char*)malloc(strlen(carDataList[0].name) * sizeof(char));
    carDataStack[0] = carDataList[0].name;

    int carCount = 1, maxCarNameLegth = 0;
    for (int index = 1; index < _msize(carDataList) / sizeof(CarData); index++)
    {
        bool isNew = true;
        for (int i = 0; i < carCount; i++)
        {
            if (strcmp(carDataList[index].name, carDataStack[i]) == 0)
            {
                isNew = false;
                break;
            }
        }
        if (isNew)
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
    char** locationStack = (char**)malloc(sizeof(char*));

    gateTagStack[0] = (char*)malloc(strlen(carDataList[0].gateTag) * sizeof(char));
    gateTagStack[0] = carDataList[0].gateTag;

    locationStack[0] = (char*)malloc(strlen(carDataList[0].location) * sizeof(char));
    locationStack[0] = carDataList[0].location;

    int gtCount = 1, maxGtNameLegth = 0;
    int locCount = 1, maxLocNameLegth = 0;
    for (int index = 1; index < _msize(carDataList) / sizeof(CarData); index++)
    {
        if (strcmp(carDataList[index].gateTag, gateTagStack[gtCount - 1]) != 0)
        {
            char** temp = (char**)realloc(gateTagStack, sizeof(char*) * (gtCount + 1));
            if (temp != NULL) {
                gateTagStack = temp;
                gateTagStack[gtCount] = (char*)malloc(strlen(carDataList[index].gateTag) + 1);
                if (gateTagStack[gtCount] != NULL) {
                    strcpy(gateTagStack[gtCount], carDataList[index].gateTag);
                    if (strlen(gateTagStack[gtCount]) > maxGtNameLegth) maxGtNameLegth = strlen(gateTagStack[gtCount]);
                    if (strlen(gateTagStack[gtCount]) > maxGtNameLegth) maxGtNameLegth = strlen(gateTagStack[gtCount]);
                    gtCount++;
                }
            }
        }
        if (strcmp(carDataList[index].location, locationStack[locCount - 1]) != 0)
        {
            char** temp = (char**)realloc(locationStack, sizeof(char*) * (locCount + 1));
            if (temp != NULL) {
                locationStack = temp;
                locationStack[locCount] = (char*)malloc(strlen(carDataList[index].location) + 1);
                if (locationStack[locCount] != NULL) {
                    strcpy(locationStack[locCount], carDataList[index].location);
                    if (strlen(locationStack[locCount]) > maxLocNameLegth) maxLocNameLegth = strlen(locationStack[locCount]);
                    if (strlen(locationStack[locCount]) > maxLocNameLegth) maxLocNameLegth = strlen(locationStack[locCount]);
                    locCount++;
                }
            }
        }
    }
    // ----------------------------------------------------------------- (게이트 태그 & 위치 중복 제거) end

    // filter들
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

    int x = 0, y = 0;
    while (true)
    {
        gotoxy(1, 0);
        printf("%s | ", user.name);

        if (y == -1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WHITE << 4) + BLACK); // set
        printf("[사용자 정보 보기]");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE); // reset

        gotoxy(0, 3);
        printf("================== [필터] ==================");

        int y1 = (gtCount / 4) + (int)(gtCount % 4 != 0);
        int y2 = y1 + (locCount / 3) + (int)(locCount % 3 != 0);
        int y3 = y2 + (carCount / 3) + (int)(carCount % 3 != 0);
        int sizeX = 0;

        gotoxy(0, 5);
        printf(" - 게이트");
        for (int index = 0; index < gtCount; index++)
        {
            Text_Filter(
                gateTagStack[index],
                x == index % 4 && y == index / 4, gateTagFilter[index],
                sizeX, 6 + (index / 4)
            );
            sizeX += maxGtNameLegth + 2;
            if ((index + 1) % 4 == 0) sizeX = 0;
        }
        sizeX = 0;

        gotoxy(0, 7 + y1);
        printf(" - 위치");
        for (int index = 0; index < locCount; index++)
        {
            Text_Filter(
                locationStack[index],
                x == index % 3 && y == y1 + index / 3, locationFilter[index],
                sizeX, 8 + y1 + index / 3
            );
            sizeX += maxLocNameLegth + 2;
            if ((index + 1) % 3 == 0) sizeX = 0;
        }
        sizeX = 0;

        gotoxy(0, 9 + y2);
        printf(" - 차종");
        for (int index = 0; index < carCount; index++)
        {
            Text_Filter(
                carDataStack[index],
                x == index % 3 && y == y2 + index / 3, carDataFilter[index],
                sizeX, 10 + y2 + index / 3
            );
            sizeX += maxCarNameLegth + 2;
            if ((index + 1) % 3 == 0) sizeX = 0;
        }
        sizeX = 0;

        gotoxy(0, 11 + y3);
        printf(" - 가격");

        gotoxy(2, 12 + y3);
        printf("Price per hour     ");
        Text_select("최소:", x == 0 && y == y3,
            4, 13 + y3);
        Text_select("최대:", x == 0 && y == y3 + 1,
            4, 14 + y3);

        gotoxy(2, 15 + y3);
        printf("Price per hour     ");
        Text_select("최소:", x == 0 && y == y3 + 2,
            4, 16 + y3);
        Text_select("최대:", x == 0 && y == y3 + 3,
            4, 17 + y3);

        Text_select("[필터 초기화]", y == y3 + 4, 1, 19 + y3);

        gotoxy(0, 21 + y3);
        printf("============== [차량 리스트] ==============");


        selectedCarIndex = -1;

        for (int i = 0; i < _msize(carDataList) / sizeof(CarData) + 1; i++)
        {
            gotoxy(0, 23 + y3 + i * 3);
            printf("                                                        ");
            gotoxy(0, 23 + y3 + i * 3 + 1);
            printf("                                                        ");
        }

        int index;
        int count = 0; // 차랑 필터링 후 남은 종류
        for (index = 0; index < _msize(carDataList) / sizeof(CarData); index++)
        {
            int filter = 0;
            // gateTag
            for (int i = 0; i < _msize(gateTagFilter) / sizeof(bool); i++) {
                if (gateTagFilter[i]) {
                    filter |= 1 << 3;
                    if (strcmp(gateTagStack[i], carDataList[index].gateTag) == 0) {
                        filter |= 1 << 0;
                        break;
                    }
                }
            }
            // location
            for (int i = 0; i < _msize(locationFilter) / sizeof(bool); i++) {
                if (locationFilter[i]) {
                    filter |= 1 << 4;
                    if (strcmp(locationStack[i], carDataList[index].location) == 0) {
                        filter |= 1 << 1;
                        break;
                    }
                }
            }
            // carData
            for (int i = 0; i < _msize(carDataFilter) / sizeof(bool); i++) {
                if (carDataFilter[i]) {
                    filter |= 1 << 5;
                    if (strcmp(carDataStack[i], carDataList[index].name) == 0) {
                        filter |= 1 << 2;
                        break;
                    }
                }
            }
            // PPH
            bool inRange = false;
            int min = 0, max = -1; // infiniti
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
                        filter |= 1 << 6;
                    }
                }
                else filter |= 1 << 6;
            }

            // PPK
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
                        filter |= 1 << 7;
                    }
                }
                else filter |= 1 << 7;
            }


            int tempFilter = filter & 0b00111111;
            filter = ~((tempFilter ^ (tempFilter >> 3)) | ~(filter | 0b00111111));
            if ((filter & 0b11000111) == 0b11000111) {
                if (y == y3 + 5 + count)
                {
                    selectedCarIndex = index;
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WHITE << 4) + BLACK); // set   
                }

                if (reserveCarIndex != NULL) {
                    for (int i = 0; i < _msize(reserveCarIndex) / sizeof(int); i++) {
                        if (reserveCarIndex[i] == carDataList[index].id) {
                            if (y == y3 + 5 + count) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (GRAY << 4) + BLACK); // set   
                            else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (darkBLUE << 4) + WHITE); // set   
                            break;
                        }
                    }
                }

                ++count;

                gotoxy(0, 23 + y3 + (count - 1) * 3);
                printf("%d. %s | %s"
                    , count
                    , carDataList[index].gateTag
                    , carDataList[index].location);

                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE); // reset

                gotoxy(0, 23 + y3 + (count - 1) * 3 + 1);
                printf("    %s | (%d원/h / %d원/km)"
                    , carDataList[index].name
                    , carDataList[index].PPH
                    , carDataList[index].PPK);
            }
        }

        gotoxy(0, 23 + y3 + count * 3);
        printf("==========================================");

        if (y < y3)             gotoxy(0, 0);
        else if (y <= y3 + 1)   gotoxy(10, 13 + y3 + (y - y3));
        else if (y <= y3 + 3)   gotoxy(10, 14 + y3 + (y - y3));
        else                    gotoxy(0, (y3 + 4) + (y - (y3 + 4)) * 3);

        Key key = Key_none;
        if (y < y3) {
            key = input_key_grid(&x, &y);
        }
        else if (y < y3 + 4) {
            if (y == y3)     key = input_stack(&l_PPH, NULL, 0, KeyFilter_number);
            else if (y == y3 + 1) key = input_stack(&h_PPH, NULL, 0, KeyFilter_number);

            if (y == y3 + 2) key = input_stack(&l_PPK, NULL, 0, KeyFilter_number);
            else if (y == y3 + 3) key = input_stack(&h_PPK, NULL, 0, KeyFilter_number);

            if (key == Key_up)                         --y;
            else if (key == Key_down || key == Key_enter)   ++y;
        }
        else {
            key = input_key(NULL, 0);

            if (key == Key_up)          --y;
            else if (key == Key_down)   ++y;
        }

        if (key == Key_enter)
        {
            if (y == -1) {
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

                            printf("    %s | (%d원/h / %d원/km)\n"
                                , carDataList[index].name
                                , carDataList[index].PPH
                                , carDataList[index].PPK);
                        }
                    }
                }
                else {
                    printf("        [예약목록없음]       ");
                }

                printf("\n\n ==========================");

                Key key = Key_none;
                while (key != Key_enter)
                    key = input_key(NULL, 0);

                system("cls");
            }

            else if (y <= y1 - 1)
                gateTagFilter[y * 4 + x] = !gateTagFilter[y * 4 + x];

            else if (y <= y2 - 1)
                locationFilter[(y - y1) * 3 + x] = !locationFilter[(y - y1) * 3 + x];

            else if (y <= y3 - 1)
                carDataFilter[(y - y2) * 3 + x] = !carDataFilter[(y - y2) * 3 + x];

            else if (y == y3 + 4) { // 필터 초기화
                for (int i = 0; i < gtCount; i++)   gateTagFilter[i] = false;
                for (int i = 0; i < locCount; i++)  locationFilter[i] = false;
                for (int i = 0; i < carCount; i++)  carDataFilter[i] = false;
                l_PPH = h_PPH = NULL;
                l_PPK = h_PPK = NULL;
                system("cls");
            }

            else if (y > y3 + 4) { // 차 선택
                if (selectedCarIndex != -1) {

                    bool isNew = true;
                    if (reserveCarIndex != NULL) {
                        int size = _msize(reserveCarIndex) / sizeof(int);

                        for (int i = 0; i < size; i++) {
                            if (reserveCarIndex[i] == carDataList[selectedCarIndex].id) {
                                isNew = false;
                                int size = _msize(reserveCarIndex) / sizeof(int);

                                if (size == 1) reserveCarIndex = NULL;
                                else {
                                    int* temp = (int*)malloc(sizeof(int) * (size - 1));

                                    for (int i = 0, count = 0; i < size; i++) {
                                        if (reserveCarIndex[i] != carDataList[selectedCarIndex].id) {
                                            temp[count++] = reserveCarIndex[i];
                                            if (count >= size - 1) break;
                                        }
                                    }

                                    if (temp != NULL) {
                                        reserveCarIndex = temp;
                                        reserveCarIndex[size - 1] = carDataList[selectedCarIndex].id;
                                    }
                                }
                                break;
                            }
                        }

                        if (isNew) {
                            int* temp = (int*)realloc(reserveCarIndex, sizeof(int) * (size + 1));

                            if (temp != NULL) {
                                reserveCarIndex = temp;
                                reserveCarIndex[size] = carDataList[selectedCarIndex].id;
                            }
                        }
                    }
                    else {
                        reserveCarIndex = (int*)malloc(sizeof(int));
                        reserveCarIndex[0] = carDataList[selectedCarIndex].id;
                    }

                    system("cls");

                    printf("%s | %s\n"
                        , carDataList[selectedCarIndex].gateTag
                        , carDataList[selectedCarIndex].location);

                    printf("    %s | (%d원/h / %d원/km)\n\n"
                        , carDataList[selectedCarIndex].name
                        , carDataList[selectedCarIndex].PPH
                        , carDataList[selectedCarIndex].PPK);

                    if (isNew || reserveCarIndex == NULL) {
                        printf("예약이 완료 되었습니다. \n");
                    }
                    else {
                        printf("예약이 취소 되었습니다. \n");
                    }

                    Text_select("확인", true,
                        30, 4);

                    Key key = Key_none;
                    while (key != Key_enter)
                        key = input_key(NULL, 0);

                    system("cls");
                }
            }

        }
        else
        {
            if (y < -1) y = -1;

            if (y == -1) x = 0;

            if (y > y3 + 4 + count) y = y3 + 4 + count;

            if (y <= y1 - 1) {
                if (y == y1 - 1 && gtCount % 4 != 0) {
                    int endIndex = gtCount % 4 - 1;
                    if (x < 0)         x = endIndex;
                    else if (x > endIndex)  x = 0;
                }
                else {
                    if (x < 0) x = 3;
                    else if (x > 3) x = 0;
                }
            }

            else if (y <= y2 - 1) {
                if (y == y2 - 1 && locCount % 3 != 0) {
                    int endIndex = locCount % 3 - 1;
                    if (x < 0)              x = endIndex;
                    else if (x > endIndex)  x = 0;
                }
                else {
                    if (x < 0) x = 2;
                    else if (x > 2) x = 0;
                }
            }

            else if (y <= y3 - 1) {
                if (y == y3 - 1 && carCount % 3 != 0) {
                    int endIndex = carCount % 3 - 1;
                    if (x < 0)              x = endIndex;
                    else if (x > endIndex)  x = 0;
                }
                else {
                    if (x < 0) x = 2;
                    else if (x > 2) x = 0;
                }
            }

            else x = 0;
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
        if (strcmp(line, "tag\n") == 0)
        {
            tag = CarDataTag_gateTag;
        }
        else if (tag == CarDataTag_gateTag)
        {
            line[strcspn(line, "\n")] = '\0';
            strcpy(gatetag, line);
            tag = CarDataTag_none;
        }

        if (strcmp(line, "loc\n") == 0)
        {
            tag = CarDataTag_location;
        }
        else if (tag == CarDataTag_location)
        {
            line[strcspn(line, "\n")] = '\0';
            strcpy(location, line);
            tag = CarDataTag_none;
        }

        if (strcmp(line, "car\n") == 0)
        {
            data[count].gateTag = (char*)malloc(sizeof(char) * (strlen(gatetag) + 1));
            strcpy(data[count].gateTag, gatetag);

            data[count].location = (char*)malloc(sizeof(char) * (strlen(location) + 1));
            strcpy(data[count].location, location);

            tag = CarDataTag_name;
        }
        else if (tag == CarDataTag_name)
        {
            line[strcspn(line, "\n")] = '\0';
            data[count].name = (char*)malloc(strlen(line) + 1);
            strcpy(data[count].name, line);
            tag++;
        }
        else if (tag == CarDataTag_PPH)
        {
            data[count].PPH = atoi(line);
            tag++;
        }
        else if (tag == CarDataTag_PPK)
        {
            data[count].PPK = atoi(line);
            data[count].id = count;
            tag = CarDataTag_none;
            count++;
        }

        if (size <= count) break;
    }

    // 파일 닫기
    fclose(file);

    //free(gatetag);
    //free(location);

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
        if (strcmp(line, "\n") == 0)
        {
            tag = UserDataTag_name;
        }
        else
        {
            line[strcspn(line, "\n")] = '\0';
            if (tag == UserDataTag_name)
            {
                data[count].name = (char*)malloc(strlen(line) + 1);
                strcpy(data[count].name, line);
                tag++;
            }
            else if (tag == UserDataTag_license)
            {
                data[count].license = (char*)malloc(strlen(line) + 1);
                strcpy(data[count].license, line);
                tag++;
            }
            else if (tag == UserDataTag_RRN)
            {
                data[count].RRN = (char*)malloc(strlen(line) + 1);
                strcpy(data[count].RRN, line);
                tag++;
            }
            else if (tag == UserDataTag_studentId)
            {
                data[count].studentId = (char*)malloc(strlen(line) + 1);
                strcpy(data[count].studentId, line);
                tag++;
            }
            else if (tag == UserDataTag_id)
            {
                data[count].id = (char*)malloc(strlen(line) + 1);
                strcpy(data[count].id, line);
                tag++;
            }
            else if (tag == UserDataTag_passward)
            {
                data[count].password = (char*)malloc(strlen(line) + 1);
                strcpy(data[count].password, line);
                tag = UserDataTag_none;
                count++;
            }
        }

        if (size <= count) break;
    }

    // 파일 닫기
    fclose(file);

    return data;
}


// 1: id 불일치, 2: password만 불일치, 3: 모두 일치
int userFound(UserData* data, char* id, char* password)
{
    int foundMask = 0;
    for (int index = 0; index < _msize(userDataList) / sizeof(UserData); index++)
    {
        // 아이디 확인
        if (id != NULL) {
            if (strcmp(userDataList[index].id, id) == 0) {
                // 비밀번호 확인
                if (password != NULL) {
                    if (strcmp(userDataList[index].password, password) == 0) {
                        *data = userDataList[index];
                        foundMask = 3;
                        break;
                    }
                    else {
                        foundMask = 2;
                        break;
                    }
                }
                else {
                    foundMask = 2;
                    break;
                }

            }
            else foundMask = 1;
        }
        else foundMask;
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
        if (stack == 0) key = input_stack(&id, &stack, 3, KeyFilter_none);
        else if (stack == 1) key = input_stack(&password, &stack, 3, KeyFilter_none);
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
        else if (stack == 1) key = input_stack(&data->name, &stack, 7, KeyFilter_none);
        else if (stack == 2) key = input_stack(&data->id, &stack, 7, KeyFilter_none);
        else if (stack == 3) key = input_stack(&data->password, &stack, 7, KeyFilter_none);
        else if (stack == 4) key = input_stack(&data->license, &stack, 7, KeyFilter_number);
        else if (stack == 5) key = input_stack(&data->RRN, &stack, 7, KeyFilter_number);
        else if (stack == 6) key = input_stack(&data->studentId, &stack, 7, KeyFilter_number);
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
            else if (stack - 1 == -1 || stack - 1 == 2) {
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
            else if (stack - 1 == -1 || stack - 1 == 3) {
                if (data->password != NULL) {
                    if (strlen(data->password) == 0) errCode = stack = 3;
                    else errCode = 0;
                }
                else errCode = stack = 3;
            }

            // ----------------------------------------------------------------- license 확인
            else if ((stack - 1 == -1 || stack - 1 == 4)) {
                if (data->license != NULL)
                {
                    if (strlen(data->license) != 10) errCode = stack = 4;
                    else errCode = 0;
                }
                else errCode = stack = 4;
            }

            // ----------------------------------------------------------------- RRN 확인
            else if ((stack - 1 == -1 || stack - 1 == 5)) {
                if (data->RRN != NULL)
                {
                    if (strlen(data->RRN) != 7) errCode = stack = 5;
                    else errCode = 0;
                }
                else errCode = stack = 5;
            }

            // ----------------------------------------------------------------- studentId 확인
            else if ((stack - 1 == -1 || stack - 1 == 6)) {
                if (data->studentId != NULL)
                {
                    if (strlen(data->studentId) != 10) errCode = stack = 6;
                    else errCode = 0;
                }
                else errCode = stack = 6;
            }

            // ----------------------------------------------------------------- 회원가입 확인
            if (stack - 1 == -1) return true;
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