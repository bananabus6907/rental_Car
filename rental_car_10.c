#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h> // bool, true, false

//
typedef struct
{
    int id; // ���� ��ȣ
    char* name; // ���� ����
    char* gateTag; // ��ġ
    char* location; // ��ġ
    int PPH; // price per hour
    int PPK; // price per km
} CarData;

typedef struct
{
    char* name;
    char* license; // �������� ��ȣ
    char* RRN; // resident registration number �ֹε�Ϲ�ȣ
    char* studentId; // �й�
    char* id; // ���̴�
    char* password; // �н�����
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

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //Ŀ������� �Լ��� ���Ǵ� ������ 
void setcursortype(CURSOR_TYPE c) { //Ŀ������� �Լ� 
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
    //x, y ��ǥ ����
    COORD pos = { x,y };
    //Ŀ�� �̵�
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
    char tmp[100]; // ����� ���� Ȯ���� ���� �ӽ� ���� �迭 ����
    int i = 0;

    if ((*str) != NULL)
    {
        strcpy(tmp, (*str));
        for (; i < tmp[i] != '\0'; i++) putchar(tmp[i]); // �Էµ� ���� ���
    }

    char ch;
    while (1) {
        ch = _getch(); // Ű �Է� �б�

        if (ch == -32) // ����Ű
        {
            ch = _getch(); //�Է°� input

            key = ch;

            tmp[i] = '\0'; // ���ڿ� ���� �� ���� �߰�
            break;
        }
        else {
            if (ch == '\r' || ch == '\n') { // Enter Ű�� �ԷµǸ� �Է� ����
                key = Key_enter;
                tmp[i] = '\0'; // ���ڿ� ���� �� ���� �߰�
                break;
            }
            else if (ch == '\b') { // �齺���̽� Ű�� �ԷµǸ� ���� ���� ����
                if (i > 0) {
                    printf("\b \b"); // Ŀ���� ����� �ڷ� �̵�
                    i--;
                }
            }
            else if (i < 99) { // �ִ� ���̱��� �Է� �ޱ�
                if (filter == KeyFilter_number
                    && !(48 <= (int)ch && (int)ch <= 57))
                {
                    continue;
                }
                putchar(ch); // �Էµ� ���� ���
                tmp[i] = ch; // �Էµ� ���� ����
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

    char ch = _getch(); // Ű �Է� �б�

    if (ch == -32) // ����Ű
    {
        ch = _getch(); //�Է°� input

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

    char ch = _getch(); // Ű �Է� �б�

    if (ch == -32) // ����Ű
    {
        ch = _getch(); //�Է°� input

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
    setcursortype(NOCURSOR); //Ŀ�� ���� 

    // ----------------------------------------------------------------- (�ʱ� ���� �ҷ�����) start
    // �ڵ��� ����
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
    // ----------------------------------------------------------------- (�ʱ� ���� �ҷ�����) end


    // ----------------------------------------------------------------- (�α��� & ȸ������) start
    UserData user = { NULL, NULL, NULL, NULL,  NULL, NULL };
    while (!user_login(&user))
    {
        // ----------------------------------------------------------------- (ȸ������) start
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
        // ----------------------------------------------------------------- (ȸ������) end
    }
    // ----------------------------------------------------------------- (�α��� & ȸ������) end


    // ----------------------------------------------------------------- (�ڵ��� �ߺ� ����) start
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
    // ----------------------------------------------------------------- (�ڵ��� �ߺ� ����) end

    // ----------------------------------------------------------------- (����Ʈ �±� & ��ġ �ߺ� ����) start
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
    // ----------------------------------------------------------------- (����Ʈ �±� & ��ġ �ߺ� ����) end

    // filter��
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

    // ����
    int selectedCarIndex = -1;
    int* reserveCarIndex = NULL;

    system("cls");

    int x = 0, y = 0;
    while (true)
    {
        gotoxy(1, 0);
        printf("%s | ", user.name);

        if (y == -1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WHITE << 4) + BLACK); // set
        printf("[����� ���� ����]");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BLACK << 4) + WHITE); // reset

        gotoxy(0, 3);
        printf("================== [����] ==================");

        int y1 = (gtCount / 4) + (int)(gtCount % 4 != 0);
        int y2 = y1 + (locCount / 3) + (int)(locCount % 3 != 0);
        int y3 = y2 + (carCount / 3) + (int)(carCount % 3 != 0);
        int sizeX = 0;

        gotoxy(0, 5);
        printf(" - ����Ʈ");
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
        printf(" - ��ġ");
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
        printf(" - ����");
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
        printf(" - ����");

        gotoxy(2, 12 + y3);
        printf("Price per hour     ");
        Text_select("�ּ�:", x == 0 && y == y3,
            4, 13 + y3);
        Text_select("�ִ�:", x == 0 && y == y3 + 1,
            4, 14 + y3);

        gotoxy(2, 15 + y3);
        printf("Price per hour     ");
        Text_select("�ּ�:", x == 0 && y == y3 + 2,
            4, 16 + y3);
        Text_select("�ִ�:", x == 0 && y == y3 + 3,
            4, 17 + y3);

        Text_select("[���� �ʱ�ȭ]", y == y3 + 4, 1, 19 + y3);

        gotoxy(0, 21 + y3);
        printf("============== [���� ����Ʈ] ==============");


        selectedCarIndex = -1;

        for (int i = 0; i < _msize(carDataList) / sizeof(CarData) + 1; i++)
        {
            gotoxy(0, 23 + y3 + i * 3);
            printf("                                                        ");
            gotoxy(0, 23 + y3 + i * 3 + 1);
            printf("                                                        ");
        }

        int index;
        int count = 0; // ���� ���͸� �� ���� ����
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
                printf("    %s | (%d��/h / %d��/km)"
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

                printf("[����� ����] - ���͸� ������ ����\n\n");

                printf("          �̸� | %s\n\n", user.name);

                printf(" �������� ��ȣ | %s\n", user.RRN);
                printf("  �ֹε�Ϲ�ȣ | %s\n\n", user.studentId);

                printf("        ���̵� | %s\n", user.id);
                printf("      �н����� | %s\n\n", user.password);

                printf(" ======== ������ ========\n\n");

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

                            printf("    %s | (%d��/h / %d��/km)\n"
                                , carDataList[index].name
                                , carDataList[index].PPH
                                , carDataList[index].PPK);
                        }
                    }
                }
                else {
                    printf("        [�����Ͼ���]       ");
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

            else if (y == y3 + 4) { // ���� �ʱ�ȭ
                for (int i = 0; i < gtCount; i++)   gateTagFilter[i] = false;
                for (int i = 0; i < locCount; i++)  locationFilter[i] = false;
                for (int i = 0; i < carCount; i++)  carDataFilter[i] = false;
                l_PPH = h_PPH = NULL;
                l_PPK = h_PPK = NULL;
                system("cls");
            }

            else if (y > y3 + 4) { // �� ����
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

                    printf("    %s | (%d��/h / %d��/km)\n\n"
                        , carDataList[selectedCarIndex].name
                        , carDataList[selectedCarIndex].PPH
                        , carDataList[selectedCarIndex].PPK);

                    if (isNew || reserveCarIndex == NULL) {
                        printf("������ �Ϸ� �Ǿ����ϴ�. \n");
                    }
                    else {
                        printf("������ ��� �Ǿ����ϴ�. \n");
                    }

                    Text_select("Ȯ��", true,
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

    // carDataStack �޸� ����
    size = _msize(carDataStack) / sizeof(char*);
    for (int i = 0; i < size; i++) {
        free(carDataStack[i]);
    }
    free(carDataStack);

    // gateTagStack �޸� ����
    size = _msize(gateTagStack) / sizeof(char*);
    for (int i = 0; i < size; i++) {
        free(gateTagStack[i]);
    }
    free(gateTagStack);

    // locationStack �޸� ����
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

    // reserveCarIndex ����
    free(reserveCarIndex);
    return 0;
}


CarData* get_car_data_list()
{
    // ���� ������ ����
    FILE* file;

    // ���� ����
    fopen_s(&file, "lental car data.txt", "r");

    // ���� ���⿡ ������ ���
    if (file == NULL) {
        fprintf(stderr, "���� '%s'�� �� �� �����ϴ�.\n", "lental car data.txt"); // ���� ���
        return NULL;
    }

    char line[100]; // ���� ���۸� ������ ����

    fgets(line, sizeof(line), file);
    int size = atoi(line); // �ڵ��� ����
    CarData* data = (CarData*)malloc(sizeof(CarData) * size);

    // ���� ������ �� �پ� �о����
    CarDataTag tag = CarDataTag_none;
    char gatetag[100]; // ����Ʈ ��ġ�� ������ ����
    char location[100]; // ��ġ ������ ����
    int count = 0; // �ҷ��� �ڵ��� ī��Ʈ
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

    // ���� �ݱ�
    fclose(file);

    //free(gatetag);
    //free(location);

    return data;
}

UserData* get_user_data_list()
{
    // ���� ������ ����
    FILE* file;

    // ���� ����
    fopen_s(&file, "userdata.txt", "r");

    // ���� ���⿡ ������ ���
    if (file == NULL) {
        fprintf(stderr, "���� '%s'�� �� �� �����ϴ�.\n", "userdata.txt"); // ���� ���
        return NULL;
    }

    char line[100]; // ���� ���۸� ������ ����

    fgets(line, sizeof(line), file);
    int size = atoi(line); // �ڵ��� ����
    UserData* data = (UserData*)malloc(sizeof(UserData) * size);

    // ���� ������ �� �پ� �о����
    UserDataTag tag = UserDataTag_none;
    int count = 0; // �ҷ��� �ڵ��� ī��Ʈ
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

    // ���� �ݱ�
    fclose(file);

    return data;
}


// 1: id ����ġ, 2: password�� ����ġ, 3: ��� ��ġ
int userFound(UserData* data, char* id, char* password)
{
    int foundMask = 0;
    for (int index = 0; index < _msize(userDataList) / sizeof(UserData); index++)
    {
        // ���̵� Ȯ��
        if (id != NULL) {
            if (strcmp(userDataList[index].id, id) == 0) {
                // ��й�ȣ Ȯ��
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
        Text_select("���̵�:", stack == 0, 40, 0);
        Text_error("���̵� ��ġ���� �ʽ��ϴ�. >> ", foundMask == 1, 8, 0);

        Text_select("��й�ȣ:", stack == 1, 38, 1);
        Text_error("��й�ȣ�� ��ġ���� �ʽ��ϴ�. >> ", foundMask == 2, 4, 1);

        Text_select("�α���", stack == 2, 44, 3);
        Text_select("ȸ������", stack == 3, 43, 4);

        gotoxy(48, stack);

        Key key;
        if (stack == 0) key = input_stack(&id, &stack, 3, KeyFilter_none);
        else if (stack == 1) key = input_stack(&password, &stack, 3, KeyFilter_none);
        else {
            key = input_key(&stack, 3);
        }

        if (key == Key_enter)
        {
            if (stack == 0) return false; // ȸ������

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
        Text_select("< �ڷ�", stack == 0, 0, 0);

        Text_select("�̸�:", stack == 1, 52, 1);
        Text_error("�̸��� �Է��ϼ���. >>", errCode == 1, 28, 1);

        Text_select("���̵�:", stack == 2, 50, 2);
        Text_error("������� ���̵� �����մϴ�. >>", errCode == 2, 16, 2);
        if (errCode == 12) Text_error("���̵� �Է��ϼ���. >>", errCode == 12, 24, 2);

        Text_select("��й�ȣ:", stack == 3, 48, 3);
        Text_error("��й�ȣ�� �Է��ϼ���. >>", errCode == 3, 20, 3);

        Text_select("�������� ��ȣ:", stack == 4, 43, 4);
        Text_error("�ùٸ��� �Է��ϼ���(10�ڸ�). >>", errCode == 4, 8, 4);

        Text_select("�ֹε�Ϲ�ȣ(�� 1�ڸ�����):", stack == 5, 30, 5);
        Text_error("�ùٸ��� �Է��ϼ���. >>", errCode == 5, 4, 5);

        Text_select("�й�:", stack == 6, 52, 6);
        Text_error("�ùٸ��� �Է��ϼ���(10�ڸ�). >>", errCode == 6, 18, 6);

        Text_select("ȸ������", stack == 7, 53, 8);


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
            // ----------------------------------------------------------------- �ڷ�
            if (stack - 1 == 0) return false;

            // ----------------------------------------------------------------- name Ȯ��
            if (stack - 1 == -1 || stack - 1 == 1) {
                if (data->name != NULL) {
                    if (strlen(data->name) == 0) errCode = stack = 1;
                    else errCode = 0;
                }
                else errCode = stack = 1;
            }

            // ----------------------------------------------------------------- id Ȯ��
            else if (stack - 1 == -1 || stack - 1 == 2) {
                if (data->id != NULL)
                {
                    if (strlen(data->id) == 0) {
                        stack = 2;
                        errCode = 12;
                    }
                    else if (userFound(data, data->id, NULL) == 2) { // ���̵� ����
                        errCode = stack = 2;
                    }
                    else errCode = 0;
                }
                else errCode = stack = 2;
            }

            // ----------------------------------------------------------------- password Ȯ��
            else if (stack - 1 == -1 || stack - 1 == 3) {
                if (data->password != NULL) {
                    if (strlen(data->password) == 0) errCode = stack = 3;
                    else errCode = 0;
                }
                else errCode = stack = 3;
            }

            // ----------------------------------------------------------------- license Ȯ��
            else if ((stack - 1 == -1 || stack - 1 == 4)) {
                if (data->license != NULL)
                {
                    if (strlen(data->license) != 10) errCode = stack = 4;
                    else errCode = 0;
                }
                else errCode = stack = 4;
            }

            // ----------------------------------------------------------------- RRN Ȯ��
            else if ((stack - 1 == -1 || stack - 1 == 5)) {
                if (data->RRN != NULL)
                {
                    if (strlen(data->RRN) != 7) errCode = stack = 5;
                    else errCode = 0;
                }
                else errCode = stack = 5;
            }

            // ----------------------------------------------------------------- studentId Ȯ��
            else if ((stack - 1 == -1 || stack - 1 == 6)) {
                if (data->studentId != NULL)
                {
                    if (strlen(data->studentId) != 10) errCode = stack = 6;
                    else errCode = 0;
                }
                else errCode = stack = 6;
            }

            // ----------------------------------------------------------------- ȸ������ Ȯ��
            if (stack - 1 == -1) return true;
        }
    }


    return false;
}

bool save_user_data(const UserData data, int size)
{
    // ���� ������ ����
    FILE* file;

    // ���� ���� (���� �����͸� ��ȣ�� ä ù�ٿ� ���� ���� r+�� ����Ѵ�)
    fopen_s(&file, "userdata.txt", "r+");

    // ���� ���⿡ ������ ���
    if (file == NULL) {
        fprintf(stderr, "���� '%s'�� �� �� �����ϴ�.\n", "userdata.txt"); // ���� ���
        return false;
    }
    fprintf(file, "%d\n", size);

    // ���� �ݱ�
    fclose(file);

    // ���� ����
    fopen_s(&file, "userdata.txt", "a");

    // ������ ���Ͽ� ����
    fprintf(file, "\n%s\n", data.name);
    fprintf(file, "%s\n", data.license);
    fprintf(file, "%s\n", data.RRN);
    fprintf(file, "%s\n", data.studentId);
    fprintf(file, "%s\n", data.id);
    fprintf(file, "%s\n", data.password);

    // ���� �ݱ�
    fclose(file);

    return true;
}