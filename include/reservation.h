#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "dateTime.h"
#include <ctype.h>
#include <stdlib.h>

typedef struct couponInfo
{
    char cpnNum[20];
    char userPhoneNum[20];
    char issueDate[20];
    char expireDate[20];
    char kindOfCpn[20];
    int valid; // 0 사용 가능, 1 사용 불가 

} coupon;

// X1-1234-5678 010-1234-5678 20020501 20030429 W2000 1
coupon cpn;
coupon cpnlist[100]; // 사용 쿠폰 담아놓는 리스트


struct CurrentTime {
    int year;
    int month;
    int day;
    int hour;
    int minute;
};


void CurrentTime(struct CurrentTime* ct, struct DateTime* dt)
{
    ct->year = dt->year;
    ct->month = dt->month;
    ct->day = dt->day;
    ct->hour = dt->hour;
    ct->minute = dt->minute;
}


//상영관 정보 파일에서 레코드 요소를 저장하는 구조체
struct TheaterRecord {
    char movietitle[30];
    int moviedate[8];
    int movietime[4];
    int runningtime;
    int seatcheck[10];
};

//예매 정보를 저장하는 구조체
typedef struct Reservation {
    //file info
    char* filename;

    //movie info
    char movietitle[30];
    int moviedate;
    char movietime[5]; // 엔터까지 생각해서 크기를 하나 크게 설정해야함...
    int agelimit;

    int runningtime;
    char seatcheck[11]; //좌석여부
    char totalSeat[11][5]; //예매 파일에 업데이트 할 좌석 정보
    int numSeat; //선택 좌석 개수

    int numAdult;
    int numTeenager;
    int numChild;
    int numKids;
    int numTotal;

    //charge info
    double baseCharge; //결제금액
    double resultCharge;

    double basePoint; //멤버 파일에서 읽어온 포인트 정보 저장
    double usedPoint; //사용한 포인트
    double remainPoint; //남은 포인트
    double accPoint; //적립 포인트

};

//파일 레코드를 읽어 저장하는 변수
char records[4][100];
char rdate[4][9];
char rtime[4][5];
char rrunningtime[4][4];
char rseat[4][11];


//예매 정보 저장 
struct Reservation reserve;

//q 또는 Q 입력 처리
static int check_q(char* str)
{
    //printf("%s\n", str);
    int q_flag = 1;
    for (int i = 0; i < strlen(str); i++) {
        char ch = str[i];
        if (ch != ' ' && ch != 'q' && ch != 'Q') {
            q_flag = 0;
            break;
        }
    }
    //printf("%d\n", q_flag);
    if (q_flag)
    {
        return 0;
    }
}

// Theater_.csv 파일 열기, return count
static int TheaterFile(struct CurrentTime ct, struct DateTime* dt, char* mtitle)
{
    FILE* theaterFile = 0;

    printf("mtitle: %s\n", mtitle);

    if (!strcmp(mtitle, "dune"))
    {
        reserve.filename = "TheaterA.csv";
        isValidDataFile(reserve.filename);
        theaterFile = fopen(reserve.filename, "r");
        reserve.agelimit = 12;
    }
    else if (!strcmp(mtitle, "commentunit"))
    {
        reserve.filename = "TheaterB.csv";
        isValidDataFile(reserve.filename);
        theaterFile = fopen(reserve.filename, "r");
        reserve.agelimit = 15;
    }
    else if (!strcmp(mtitle, "thebeekeeper"))
    {
        reserve.filename = "TheaterC.csv";
        isValidDataFile(reserve.filename);
        theaterFile = fopen(reserve.filename, "r");
        reserve.agelimit = 19;
    }

    //read file record

    for (int i = 0; i < 4; i++)
    {
        fgets(records[i], 100, theaterFile);
        records[i][strcspn(records[i], "\n")] = 0;
        char* token = strtok(records[i], ",");
        strcpy(mtitle, token);
        token = strtok(NULL, ",");
        strcpy(rdate[i], token);
        token = strtok(NULL, ",");
        strcpy(rtime[i], token);
        token = strtok(NULL, ",");
        strcpy(rrunningtime[i], token);
        token = strtok(NULL, ",");
        strcpy(rseat[i], token);
    }

    int count = 0; //check after time
    // dt의 시간이 records의 시간보다 이후인 경우의 수
    for (int i = 0; i < 4; i++) {
        int hour = (rtime[i][0] - '0') * 10 + (rtime[i][1] - '0');
        int minute = (rtime[i][3] - '0') * 10 + (rtime[i][4] - '0');
        if (ct.hour > hour || (ct.hour == hour && ct.minute > minute)) count++;
    }

    return count;
}

static int selectMovie(struct CurrentTime ct, int age)
{

    while (1) {
        printf("\n------------------------------------------------\n");
        printf("[Reservation]\n");
        printf("Current DateTime: %d-%d-%d %02d:%02d\n", ct.year, ct.month, ct.day, ct.hour, ct.minute);
        printf("    Title(Movie Rating)          Running Time\n");
        printf("  1.    Dune(12)                    60\n");
        printf("  2.    Comment Unit(15)            60\n");
        printf("  3.    The Beekeeper(19)           60\n");
        printf("If you enter q, you will return to the main menu.\n");
        printf("\nEnter: ");

        char title[31];
        fgets(title, sizeof(title), stdin);

        // only enter
        if (strlen(title) == 1 && title[0] == '\n')
        {
            continue;
        }

        //개행문자 제거
        if (strchr(title, '\n') == NULL)
            while (getchar() != '\n');
        if (title[strlen(title) - 1] == '\n')
            title[strlen(title) - 1] = '\0';
        printf("%s", title);

        //check input q or Q
        if (check_q(title) == 0) return 0;

        //문자만 저장
        int j = 0;
        for (int i = 0; i < strlen(title); i++)
        {
            if (!isspace(title[i]) && title[i] != '\0' && !ispunct(title[i]) && !isdigit(title[i]))
            {
                reserve.movietitle[j] = tolower(title[i]);
                j++;
            }
        }
        reserve.movietitle[j] = '\0';

        //문법 및 의미 규칙 확인, 오류 출력
        if (strchr(title, ',') != NULL)
        {
            printf("Forbidden character, Please check it again\n");
            continue;
        }
        else if (!strcmp(reserve.movietitle, "dune") && age < 12)
        {
            printf("*You are at an age when you can't watch this movie. Please choose a different movie*\n");
            continue;
        }
        else if (!strcmp(reserve.movietitle, "commentunit") && age < 15)
        {
            printf("*You are at an age when you can't watch this movie. Please choose a different movie*\n");
            continue;
        }
        else if (!strcmp(reserve.movietitle, "thebeekeeper") && age < 19)
        {
            printf("*You are at an age when you can't watch this movie. Please choose a different movie*\n");
            continue;
        }
        else if ((strcmp(reserve.movietitle, "dune") && strcmp(reserve.movietitle, "commentunit") && strcmp(reserve.movietitle, "thebeekeeper")))
        {
            printf("*This is a non-existent movie name.*\n");
            continue;
        }
        else
        {
            break;
        }
    }
    return 1;
}

static int selectTime(struct CurrentTime ct, int count)
{

    while (1)
    {
        printf("[Select Time]");
        printf("Current date and time: %d.%d.%d %02d:%02d\n", ct.year, ct.month, ct.day, ct.hour, ct.minute);
        printf("Selected Movie: %s\n", reserve.movietitle);
        printf("Available Time      Seats\n\n");

        for (int i = count; i < 4; i++)
        {
            int sAvailable = 0;
            for (int j = 0; j < 10; j++)
            {
                if (rseat[i][j] == '0') sAvailable++;
            }
            printf("     %s           %d/10\n", rtime[i], sAvailable);

        }

        printf("(If you enter q, you will return to the main menu.)\n");
        printf("Enter: ");

        char selectedTime[11];
        fgets(selectedTime, sizeof(selectedTime), stdin);

        // only enter
        if (strlen(selectedTime) == 1 && selectedTime[0] == '\n')
        {
            continue;
        }

        //개행 문자 제거
        if (strchr(selectedTime, '\n') == NULL)
            while (getchar() != '\n');
        if (selectedTime[strlen(selectedTime) - 1] == '\n') {
            selectedTime[strlen(selectedTime) - 1] = '\0';
        }


        //check input q or Q
        if (check_q(selectedTime) == 0) return 0;

        int digitnum = 0;
        int j = 0;
        for (int i = 0; i < strlen(selectedTime); i++)
        {
            if (isdigit(selectedTime[i]) && !isspace(selectedTime[i]) && !ispunct(selectedTime[i]) && selectedTime[i] != '\0')
            {
                digitnum++;
                reserve.movietime[j] = selectedTime[i];
                j++;
            }
        }

        //사용자 입력이 파일에 존재하는 시간인지 확인
        int n = 0;
        for (int i = count; i < 4; i++)
        {
            if (!strcmp(reserve.movietime, rtime[i]))
            {
                n = 1;
            }
        }

        //문법 및 의미 규칙 확인, 오류 출력
        if (digitnum < 4)
        {
            printf("*Invalid type of input. Please enter 4 numbers for the right time.*\n");
        }
        else if (n != 1)
        {
            printf("*Non-existent time. Please re-enter.\n");
        }
        else
        {
            break;
        }
    }
    return 1;
}


static int selectAdult(int seatAvailable)
{
    while (1)
    {

        printf("How many adults are there?\n\n");
        printf("Enter: ");

        char selectedAdult[6]; //사용자 입력
        char adult[6];
        fgets(selectedAdult, sizeof(selectedAdult), stdin);

        // only enter
        if (strlen(selectedAdult) == 1 && selectedAdult[0] == '\n')
        {
            continue;
        }

        //개행문자 제거
        if (strchr(selectedAdult, '\n') == NULL)
            while (getchar() != '\n');
        if (selectedAdult[strlen(selectedAdult) - 1] == '\n')
            selectedAdult[strlen(selectedAdult) - 1] = '\0';
        selectedAdult[strcspn(selectedAdult, "\n")] = '\0';

        //check input q or Q
        if (check_q(selectedAdult) == 0) return 0;

        //숫자 또는 공백이 아닌 문자가 있는 경우
        int c = 0; //number count
        for (int i = 0; i < strlen(selectedAdult); i++) {
            if (!isdigit(selectedAdult[i]) && !isspace(selectedAdult[i])) {
                c = 1;
                break;
            }
        }
        if (c == 1) //not exist number
        {
            printf("*Invalid input. Only numbers can be entered. Please re-enter.*\n");
            continue;
        }

        //입력에서 숫자만 저장
        int count = 0;
        int j = 0;
        for (int i = 0; i < strlen(selectedAdult); i++) {
            if (isdigit(selectedAdult[i])) {
                adult[j] = selectedAdult[i];
                count++;
                j++;
            }
        }
        reserve.numAdult = atoi(adult);

        if (count == 0)
        {
            printf("*Invalid input. Only numbers can be entered. Please re-enter.*\n");
            continue;
        }

        else if (reserve.numAdult > seatAvailable || reserve.numAdult < 0)
        {
            printf("*Not enough seats available. Please re-enter.*\n");
            continue;
        }
        else
            break;
    }
    return 1;
}

static int selectTeenager(int seatAvailable)
{
    while (1)
    {
        printf("How many people are under 19 years old and over 15 years old?\n\n");
        printf("Enter: ");

        char selectedTeenager[5];
        char Teenager[5];
        fgets(selectedTeenager, sizeof(selectedTeenager), stdin);

        // only enter
        if (strlen(selectedTeenager) == 1 && selectedTeenager[0] == '\n')
        {
            continue;
        }

        //개행 문자 제거
        if (strchr(selectedTeenager, '\n') == NULL)
            while (getchar() != '\n');
        if (selectedTeenager[strlen(selectedTeenager) - 1] == '\n')
            selectedTeenager[strlen(selectedTeenager) - 1] = '\0';
        selectedTeenager[strcspn(selectedTeenager, "\n")] = '\0';

        //check input q or Q
        if (check_q(selectedTeenager) == 0) return 0;

        //숫자만 저장
        int j = 0;
        int count = 0;
        for (int i = 0; selectedTeenager[i] != '\0'; i++) {
            if (isdigit(selectedTeenager[i])) {
                Teenager[j] = selectedTeenager[i];
                count++;
                j++;
            }
        }

        if (count == 0) {
            printf("*Invalid input. Only numbers can be entered. Please re-enter.*\n");
            continue;
        }

        reserve.numTeenager = atoi(Teenager);

        if (reserve.numTeenager > seatAvailable || reserve.numTeenager < 0) {
            printf("*Not enough seats available. Please re-enter.*\n");
            continue;
        }

        else if (reserve.agelimit == 19) {
            if (reserve.numTeenager != 0) {
                printf("*Under %dyears old can't watch this movie. Please re-enter it.*\n", reserve.agelimit);
                continue;
            }
            else
                break;
        }
        else
            break;

    }
    return 1;
}

static int selectChild(int seatAvailable)
{
    while (1)
    {
        printf("How many adults are under 15 years old and over 12 years old?\n\n");
        printf("Enter: ");

        char selectedChild[5];
        char Child[5];
        fgets(selectedChild, sizeof(selectedChild), stdin);

        // only enter
        if (strlen(selectedChild) == 1 && selectedChild[0] == '\n')
        {
            continue;
        }

        //개행 문자 제거
        if (strchr(selectedChild, '\n') == NULL)
            while (getchar() != '\n');
        if (selectedChild[strlen(selectedChild) - 1] == '\n')
            selectedChild[strlen(selectedChild) - 1] = '\0';
        selectedChild[strcspn(selectedChild, "\n")] = '\0';

        //check input q or Q
        if (check_q(selectedChild) == 0) return 0;

        //숫자만 저장
        int count = 0;
        int j = 0;
        for (int i = 0; selectedChild[i] != '\0'; i++) {
            if (isdigit(selectedChild[i])) {
                Child[j] = selectedChild[i];
                count++;
                j++;
            }
        }

        if (count == 0) {
            printf("*Invalid input. Only numbers can be entered. Please re-enter.*\n");
            continue;
        }

        reserve.numChild = atoi(Child);

        if (reserve.numChild > seatAvailable || reserve.numChild < 0) {
            printf("*Not enough seats available. Please re-enter.*\n");
            continue;
        }
        else if (reserve.agelimit == 19 || reserve.agelimit == 15) {
            if (reserve.numChild != 0) {
                printf("*Under %d years old can’t watch this movie. Please re-enter it.*\n", reserve.agelimit);
                continue;
            }
            else break;
        }
        else break;
    }
    return 1;
}

static int selectKids(int seatAvailable)
{
    while (1)
    {
        printf("How many people are under the age of 12?\n\n");
        printf("Enter: ");

        char selectedKids[5];
        char Kids[5];
        fgets(selectedKids, sizeof(selectedKids), stdin);

        // only enter
        if (strlen(selectedKids) == 1 && selectedKids[0] == '\n')
        {
            continue;
        }

        //개행 문자 제거
        if (strchr(selectedKids, '\n') == NULL)
            while (getchar() != '\n');
        if (selectedKids[strlen(selectedKids) - 1] == '\n')
            selectedKids[strlen(selectedKids) - 1] = '\0';
        selectedKids[strcspn(selectedKids, "\n")] = '\0';

        //check input q or Q
        if (check_q(selectedKids) == 0) return 0;

        //숫자만 저장
        int count = 0;
        int j = 0;
        for (int i = 0; selectedKids[i] != '\0'; i++) {
            if (isdigit(selectedKids[i])) {
                Kids[j] = selectedKids[i];
                count++;
                j++;
            }
        }

        if (count == 0) {
            printf("*Invalid input. Only numbers can be entered. Please re-enter.*\n");
            continue;
        }

        reserve.numKids = atoi(Kids);
        //printf("numKids: %d\n", reserve.numKids);
        if (reserve.numKids > seatAvailable || reserve.numKids < 0) {
            printf("*Not enough seats available. Please re-enter.*\n");
            continue;
        }
        else if (reserve.agelimit == 19 || reserve.agelimit == 15 || reserve.agelimit == 12) {
            if (reserve.numKids != 0) {
                printf("*Under  %d years old can’t watch this movie. Please re-enter it.*\n", reserve.agelimit);
                continue;
            }
            else
                break;
        }
        else
            break;
    }
    return 1;
}

static int selectNum(struct CurrentTime ct, int seatAvailable)
{

    while (1) {
        printf("[Choose The Number Of People]\n");
        printf("Current date and time: %d.%d.%d %02d:%02d\n", ct.year, ct.month, ct.day, ct.hour, ct.minute);
        printf("Selected Movie: %s\n", reserve.movietitle);
        printf("Selected Time : %s\n", reserve.movietime);
        printf("Seats: %d\n", seatAvailable);
        printf("Please enter the number of people you want to reserve in order.\n");
        printf("(19years~, 15years~19years, 12years~15years, ~12years)\n");
        printf("(If you enter q, you will return to the main menu.)\n");

        //리턴 값이 0이라면 q 또는 Q 입력이므로 다시 0 리턴
        if (selectAdult(seatAvailable) == 0)
        {
            return 0;
        }
        if (selectTeenager(seatAvailable) == 0)
        {
            return 0;
        }

        if (selectChild(seatAvailable) == 0)
        {
            return 0;
        }

        if (selectKids(seatAvailable) == 0)
        {
            return 0;
        }


        reserve.numTotal = reserve.numAdult + reserve.numTeenager + reserve.numChild + reserve.numKids;


        if (reserve.numTotal > seatAvailable) {
            printf("*There are more people than available seats %d, so you cannot book. Please enter the number of people again.*\n", seatAvailable);
            continue;
        }
        else {
            printf("The Number of People : %d\n", reserve.numTotal);
            break;
        }

    }
    return 1;

}

static int selectSeat(struct CurrentTime ct, int index, int seatAvailable)
{

    char symbols[2] = { 'O', 'X' };

    char arr[11];
    for (int i = 0; i < 10; i++) {
        arr[i] = rseat[index][i];
    }

    char arr2[] = "0000000000";

    int successCount = 0;
    char input[11];

    printf("[Select Seats]\n");
    printf("Current date and time: %d.%d.%d %02d:%02d\n", ct.year, ct.month, ct.day, ct.hour, ct.minute);
    printf("Selected Movie: %s\n", reserve.movietitle);
    printf("Selected Time : %s\n", reserve.movietime);
    printf("Seats: %d\n", seatAvailable);
    printf("The Number of People : %d\n", reserve.numTotal);

    for (int i = 1; i < 6; i++) {
        printf("\t%d", i);
    }

    for (int i = 0; i < 2; i++) {
        printf("\n%c\t", 'A' + i);
        for (int j = 0; j < 5; j++) {
            if (i == 0) {
                if (arr[j] == '0') {
                    printf("%c\t", symbols[0]);
                }
                else printf("%c\t", symbols[1]);
            }
            else {
                if (arr[j + 5] == '0') {
                    printf("%c\t", symbols[0]);
                }
                else printf("%c\t", symbols[1]);
            }
        }
    }
    printf("\n(If you enter q, you will return to the main menu.)\n");

    char* seatArr[] = { "a1","a2","a3","a4","a5","b1","b2","b3","b4","b5" };
    char charSeat;

    while (successCount < reserve.numTotal) {
        int charCount = 0;
        int intCount = 0;
        int intSeat = 0;
        printf("(%d/%d)\n", successCount + 1, reserve.numTotal);
        printf("Enter: ");
        fgets(input, sizeof(input), stdin);

        // only enter
        if (strlen(input) == 1 && input[0] == '\n')
        {
            continue;
        }

        if (strchr(input, '\n') == NULL)
            while (getchar() != '\n');
        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';

        // 만약 문자열이 10자를 초과하면 나머지 입력을 지움
        if (strlen(input) == 11 && input[10] != '\n') {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        else {
            // 마지막 개행 문자 제거
            input[strcspn(input, "\n")] = '\0';
        }
        //check input q or Q
        if (check_q(input) == 0) return 0;

        for (int i = 0; i < strlen(input); i++) {
            if (input[i] == 'A' || input[i] == 'B' || input[i] == 'a' || input[i] == 'b' || input[i] == '1' || input[i] == '2' || input[i] == '3' || input[i] == '4' || input[i] == '5') {
                if (intCount == 0 && (input[i] == '1' || input[i] == '2' || input[i] == '3' || input[i] == '4' || input[i] == '5')) {
                    intCount++;
                    intSeat = input[i] - '0';

                }
                else if (charCount == 0 && (input[i] == 'A' || input[i] == 'B' || input[i] == 'a' || input[i] == 'b')) {
                    charCount++;
                    charSeat = toupper(input[i]);

                }
            }
        }

        if (intCount != 0 && charCount != 0) {
            if (charSeat == 'A' && arr[intSeat - 1] != '1') {
                arr[intSeat - 1] = '1';
                arr2[intSeat - 1] = '1';
                successCount++;
                printf("\n");
                continue;
            }
            else if (charSeat == 'B' && arr[intSeat + 4] != '1') {
                arr[intSeat + 4] = '1';
                arr2[intSeat + 4] = '1';
                successCount++;
                printf("\n");
                continue;
            }
            else if ((charSeat == 'A' && arr[intSeat - 1] == '1') || (charSeat == 'B' && arr[intSeat - 1] == '1')) {
                printf(" *This seat has already been booked.Please re - enter it.*\n\n");
                continue;
            }
        }
        else if (strcmp(input, "q") == 0)
        {
            return 0;
        }
        else {
            printf("*Invalid input. It must contain rows and columns unconditionally.*\n\n");
            continue;
        }
    }

    //예매 좌석 수 카운트
    int j = 0;
    for (int i = 0; i < 10 && j < 10; i++) {
        if (arr2[i] == '1') {
            strcpy(reserve.totalSeat[j], seatArr[i]);
            j++;
            reserve.numSeat++;
        }
    }

    //파일에 들어갈 좌석 여부 문자열 수정
    for (int i = 0; i < 10; i++) {
        if (reserve.seatcheck[i] != NULL)
            reserve.seatcheck[i] = "";
        reserve.seatcheck[i] = arr[i];
    }
    reserve.seatcheck[10] = '\0';

    return 1;
}


static int checkCharge(struct CurrentTime ct)
{
    //금액 계산
    reserve.baseCharge = reserve.numAdult * 13000 + reserve.numTeenager * 10000 + reserve.numChild * 10000 + reserve.numKids * 8000;
    reserve.accPoint = reserve.baseCharge * 0.1; //적립 포인트 계산

    while (1)
    {
        printf("[ Check A Total Charge  ]\n");
        printf("Current date and time: %d.%d.%d %02d:%02d\n", ct.year, ct.month, ct.day, ct.hour, ct.minute);
        printf("Selected Movie: %s\n", reserve.movietitle);
        printf("Selected Time : %s \n", reserve.movietime);
        printf("The Number of People : %d\n", reserve.numTotal);
        printf("Selected seat: ");
        for (int i = 0; i < reserve.numSeat; i++) {
            printf("%s", reserve.totalSeat[i]);
        }
        printf("\n\n\n");
        printf("A Total Charge : %.f\n", reserve.baseCharge);
        printf("Are you sure to make a reservation ? (Y/N)\n\n");
        printf("Enter : ");

        char input[4];
        fgets(input, 4, stdin);

        //only enter
        if (strlen(input) == 1 && input[0] == '\n')
        {
            continue;
        }

        //개행문자 제거
        if (strchr(input, '\n') == NULL)
            while (getchar() != '\n');
        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';

        if (strcmp(input, "n") != 0 && strcmp(input, "N") != 0 && strcmp(input, "y") != 0 && strcmp(input, "Y") != 0) {
            printf("*Invalid Input. Only “Y or y “ and “N or n” can be entered.*\n");
        }

        else if (strcmp(input, "n") == 0 || strcmp(input, "N") == 0)
        {
            return 0;
        }
        else if ((strcmp(input, "y") == 0 || strcmp(input, "Y") == 0))
        {
            return 1;
        }
    }
}


void printResInfo(struct CurrentTime ct) // 프롬프트 상단 예약 정보 출력, 매개변수 코드 이식하면서 수정하기
{
    printf("[ Using Coupon ]\n");
    printf("Current date and time: %d.%d.%d %02d:%02d\n", ct.year, ct.month, ct.day, ct.hour, ct.minute);
    printf("Selected Movie: %s\n", reserve.movietitle);
    printf("Selected Time : %s \n", reserve.movietime);
    printf("The Number of People : %d\n", reserve.numTotal);
    printf("Selected seat: ");
    for (int i = 0; i < reserve.numSeat; i++)
    {
        printf("%s", reserve.totalSeat[i]);
    }
    printf("\nA Total Charge : %.f\n\n", reserve.baseCharge);

}


void printCouponList() // 사용가능 쿠폰 리스트 출력 함수
{
    int listNum = 0;
    char* fileName[3] = { "CouponX.txt", "CouponY.txt", "CouponZ.txt" };
    for (int i = 0; i < 100; i++)
    {
        strcpy(cpnlist[i].cpnNum, "0");
    }

    for (int i = 0; i < 3; i++)
    {
        isValidDataFile(fileName[i]); //------> 코드 이식하고 주석 풀어주기(파일 무결성 검사 함수)
        FILE* fp = fopen(fileName[i], "r");
        while (fscanf(fp, "%s %s %s %s %s %d", cpn.cpnNum, cpn.userPhoneNum, cpn.issueDate, cpn.expireDate, cpn.kindOfCpn, &cpn.valid) != EOF)
        {
            if (strcmp(cpn.userPhoneNum, u.number) == 0 && cpn.valid == 0)
                cpnlist[listNum++] = cpn;
        }
        fclose(fp);
    }

    printf("Enter the number of coupon you want to use\n\n");
    for (int i = 0; i < listNum; i++)
    {
        printf("%d. %s   %s\n", i + 1, cpnlist[i].cpnNum, cpnlist[i].kindOfCpn);
    }
    printf("(If you don’t want to use the coupon, type 0)\n\n");
    printf("Enter: ");
}



// -1 쿠폰 입력 오류, -2 쿠폰 사용 X
int selectCoupon() // 쿠폰 입력 받고 사용
{
    char* kc[6] = { "X1", "Y1", "Y2", "Y3", "Y4", "Z1" };
    char input[50];

    //gets(input);
    if (scanf_s("%[^\n]s", input, sizeof(input)) == 0) // 쿠폰 입력 받음
    {
        while (getchar() != '\n');
        return -1;
    }

    if (strlen(input) > 20) // 20자 초과 판단
    {
        while (getchar() != '\n');
        return -1;
    }


    if (strcmp(input, "0") == 0) // 0 입력 판단(쿠폰 사용 x 판단)
    {
        while (getchar() != '\n');
        return -2;
    }

    if (input[0] >= 120 && input[0] <= 122) // 대소문자 구분하지 않으므로 소문자 입력시 대문자로 변환
        input[0] -= 32;

    if (strlen(input) == 2) // 두글자 입력했을때
    {
        for (int i = 0; i < 6; i++)
        {
            if (strcmp(input, kc[i]) == 0) // 쿠폰 형식 중 하나일때
            {
                char str[20];
                strcpy(str, u.number + 3);
                strcat(input, str);
                break;
            }
        }
    }
    else if (strlen(input) == 10)
    {
        input[12] = '\0';
        for (int i = 9; i > 5; i--)
            input[i + 2] = input[i];

        input[7] = '-';
        for (int i = 5; i > 1; i--)
            input[i + 1] = input[i];
        input[2] = '-';
    }

    for (int i = 0; i < 100; i++) // 리스트에 해당하는 쿠폰 있는지 확인
    {
        if (strcmp(input, cpnlist[i].cpnNum) == 0) // 입력받은 쿠폰번호 비교 후
        {
            printf("%s\n\n", cpnlist[i].cpnNum);
            while (getchar() != '\n');
            return i; // 리스트에 해당하는 번호(쿠폰 리스트 번호) 반환
        }
    }

    while (getchar() != '\n');
    return -1; // 리스트에 없는 입력이면 -1 반환
}



void setCharge(int num)
{
    char dis[10];
    if (cpnlist[num].kindOfCpn[0] == 'W')
    {
        int i;
        for (i = 0; cpnlist[num].kindOfCpn[i] != '\0'; i++)
            dis[i] = cpnlist[num].kindOfCpn[i + 1];
        dis[i] = '\0';
        int discount = atoi(dis);
        reserve.resultCharge = reserve.baseCharge - discount;
    }
    else if (cpnlist[num].kindOfCpn[0] == '%')
    {
        int i;
        for (i = 0; cpnlist[num].kindOfCpn[i] != '\0'; i++)
            dis[i] = cpnlist[num].kindOfCpn[i + 1];
        dis[i] = '\0';
        int discount = atoi(dis);
        reserve.resultCharge = reserve.baseCharge * (100 - discount) / 100;
    }
    else
    {
        reserve.resultCharge = reserve.baseCharge;
    }
}



void rewriteCouponFile(int num) // 쿠폰 리스트 번호 받아서 해당 쿠폰이 존재하는 파일에서 0 -> 1로 바꿔주기
{
    char f = cpnlist[num].cpnNum[0];
    char fileName[20] = "";
    //printf("%c\n\n", f);

    if (f == 'X') // 회원가입 쿠폰 파일 열기 (CouponA.txt) 2000 할인
        strcpy(fileName, "CouponX.txt");
    else if (f == 'Y') // 정기 쿠폰 파일 열기 (CouponB.txt) n% 할인
        strcpy(fileName, "CouponY.txt");
    else if (f == 'Z') // 생일 쿠폰 파일 열기 (CouponC.txt) 5000원 할인
        strcpy(fileName, "CouponZ.txt");

    isValidDataFile(fileName); //코드 이식 후 주석 풀어주기 (파일 무결성 검사
    FILE* originalfile = fopen(fileName, "r");
    FILE* copyfile = fopen("couponCopy.txt", "w");

    while (fscanf(originalfile, "%s %s %s %s %s %d", cpn.cpnNum, cpn.userPhoneNum, cpn.issueDate, cpn.expireDate, cpn.kindOfCpn, &cpn.valid) != EOF)
    {
        if (strcmp(cpn.cpnNum, cpnlist[num].cpnNum) == 0) // 동일한 쿠폰 번호 찾으면
            fprintf(copyfile, "%s %s %s %s %s %d\n", cpn.cpnNum, cpn.userPhoneNum, cpn.issueDate, cpn.expireDate, cpn.kindOfCpn, 1); // valid 1로 입력(기한내에 사용 했다는 플래그)
        else
            fprintf(copyfile, "%s %s %s %s %s %d\n", cpn.cpnNum, cpn.userPhoneNum, cpn.issueDate, cpn.expireDate, cpn.kindOfCpn, cpn.valid);
    }
    fclose(originalfile);
    fclose(copyfile);

    copyfile = fopen("couponCopy.txt", "r");
    originalfile = fopen(fileName, "w");

    if (originalfile == NULL || copyfile == NULL)
    {
        printf("error opening files.\n");
        return 1;
    }

    while (fscanf(copyfile, "%s %s %s %s %s %d", cpn.cpnNum, cpn.userPhoneNum, cpn.issueDate, cpn.expireDate, cpn.kindOfCpn, &cpn.valid) != EOF)
    {
        fprintf(originalfile, "%s %s %s %s %s %d\n", cpn.cpnNum, cpn.userPhoneNum, cpn.issueDate, cpn.expireDate, cpn.kindOfCpn, cpn.valid);
    }

    fclose(originalfile);
    fclose(copyfile);

    remove("couponCopy.txt");
}


//쿠폰 사용에 대한 함수**
static int usingCoupon(struct CurrentTime ct) // 사용하면 쿠폰 번호 반환, 사용안하면 -1 반환
{
    while (1)
    {
        printResInfo(ct);
        printCouponList();
        int a = selectCoupon();
        if (a == -1) // 쿠폰 입력 오류
        {
            printf("\n*Invalid coupon number. Please re-enter.\n\n");
        }
        else if (a == -2) // 쿠폰 사용 X
        {
            return -1;
        }
        else // 쿠폰 사용
        {
            return a;
        }
    }
}

//포인트 사용
static void usingPoint(struct CurrentTime ct)
{
    reserve.basePoint = u.point;
    reserve.usedPoint = 0;
    reserve.remainPoint = 0;

    while (1)
    {
        printf("[ Using Points ]\n");
        printf("Current date and time: %d.%d.%d %02d:%02d\n", ct.year, ct.month, ct.day, ct.hour, ct.minute);
        printf("Selected Movie: %s\n", reserve.movietitle);
        printf("Selected Time : %s \n", reserve.movietime);
        printf("The Number of People : %d\n", reserve.numTotal);
        printf("Selected seat: ");
        for (int i = 0; i < reserve.numSeat; i++) {
            printf("%s", reserve.totalSeat[i]);
        }
        printf("\n\n\n");
        printf("A Total Charge : %.f\n", reserve.resultCharge);
        printf("Points : %.f\n\n", reserve.basePoint);
        printf("How much points would you like to use?\n");
        printf("Enter: ");

        char input[5];
        fgets(input, sizeof(input), stdin);

        // only enter
        if (strlen(input) == 1 && input[0] == '\n')
        {
            continue;
        }

        if (strchr(input, '\n') == NULL)
            while (getchar() != '\n');
        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';

        int c = 0;
        for (int i = 0; i < strlen(input); i++)
        {
            if (!isdigit(input[i])) {

                c = 1;
                break;
            }
        }
        if (c == 0)
            reserve.usedPoint = atoi(input);
        else
        {
            printf("*Invalid Input. Only number can be entered. Please re-enter it*\n");
            continue;
        }

        reserve.remainPoint = reserve.basePoint - reserve.usedPoint;


        if (reserve.usedPoint > reserve.basePoint) {
            printf("*Please enter available points*\n");
            continue;
        }

        else if (reserve.remainPoint < 0) {
            reserve.remainPoint = 0;
        }
        else {
            reserve.resultCharge = reserve.resultCharge - reserve.usedPoint;
            if (reserve.resultCharge < 0)
            {
                reserve.resultCharge = 0;
            }

            printf("Remain Points : %.f\n", reserve.remainPoint);
            printf("A Charge To Be Paid: %.f\n\n\n", reserve.resultCharge);
            break;
        }

    }
}

// 정상이면 1 반환, 틀리면 0 반환
static int payCharge(struct CurrentTime ct)
{

    while (1)
    {
        printf("[Check Reservation]\n");
        printf("Current date and time: %d.%d.%d %02d:%02d\n", ct.year, ct.month, ct.day, ct.hour, ct.minute);
        printf("Selected Movie: %s\n", reserve.movietitle);
        printf("Selected Time : %s\n", reserve.movietime);
        printf("The Number of People : %d\n", reserve.numTotal);
        printf("Selected seat: ");
        for (int i = 0; i < reserve.numSeat; i++)
        {
            printf("%s", reserve.totalSeat[i]);
        }
        printf("\n\n\n");
        printf("A Total Charge : %.f\n", reserve.baseCharge + reserve.usedPoint);
        if (u.member == 1)
        {
            printf("Used Points: %.f\n\n\n", reserve.usedPoint);
            printf("A Charge To Be Paid: %.f\n", reserve.resultCharge);
            printf("Points to be accumulated: %.f\n\n\n", reserve.accPoint);
        }
        printf("Are you sure to make a reservation?(Y/N)\n");
        printf("Enter: ");

        char input[4];
        fgets(input, sizeof(input), stdin);

        // only enter
        if (strlen(input) == 1 && input[0] == '\n')
        {
            continue;
        }

        if (strchr(input, '\n') == NULL)
            while (getchar() != '\n');
        if (input[strlen(input) - 1] == '\n')
            input[strlen(input) - 1] = '\0';

        if (strcmp(input, "n") != 0 && strcmp(input, "N") != 0 && strcmp(input, "y") != 0 && strcmp(input, "Y") != 0) {
            printf("*Invalid Input. Only “Y or y “ and “N or n” can be entered.*\n");
        }

        else if (strcmp(input, "n") == 0 || strcmp(input, "N") == 0) return 0;
        else if (strcmp(input, "y") == 0 || strcmp(input, "Y") == 0) {
            u.point = reserve.remainPoint + reserve.accPoint;
            return 1;
        }
    }

}

//예매 내역 파일 업데이트
static void reserve_file_Update()
{
    isValidDataFile("Reservation.csv");
    FILE* ReservationFile = fopen("Reservation.csv", "a");
    if (ReservationFile == NULL) {
        printf("failed");
        return -1;
    }

    fprintf(ReservationFile, "%s,%s,%d,%s,", u.number, reserve.movietitle, reserve.moviedate, reserve.movietime);

    for (int i = 0; i < reserve.numSeat; i++) {
        fprintf(ReservationFile, "%s", reserve.totalSeat[i]);
    }
    fputs("\n", ReservationFile);
    fclose(ReservationFile);
}

//상영관 파일 업데이트 
static void theater_file_Update(int index, int cd)
{

    isValidDataFile(reserve.filename);
    FILE* tempfile = fopen(reserve.filename, "r");
    char temprecord[4][50];

    for (int i = 0; i < 4; i++) {
        temprecord[i][0] = '\0';
    }
    for (int i = 0; i < 4; i++) {
        fgets(temprecord[i], 50, tempfile);
    }
    fclose(tempfile);


    char strCD[10];
    sprintf(strCD, "%d", cd);

    for (int i = 0; i < 4; i++) {
        if (i == index) {
            sprintf(temprecord[i], "%s,%s,%s,60,%s\n", reserve.movietitle, strCD, reserve.movietime, reserve.seatcheck);
        }
    }
    isValidDataFile(reserve.filename);
    FILE* file = fopen(reserve.filename, "w");
    for (int i = 0; i < 4; i++)
    {
        fputs(temprecord[i], file);
    }
    fclose(file);
}

//멤버 파일 포인트 업데이트
static void mem_file_Update()
{
    char id[50];
    char name[50];
    char password[50];
    char birth[50];
    char phoneNumber[50];
    double point;

    isValidDataFile("Member.txt");
    FILE* originalFile = fopen("Member.txt", "r");
    FILE* copyFile = fopen("MemberCopy.txt", "w");

    while (fscanf(originalFile, "%s %s %s %s %s %lf", name, id, password, birth, phoneNumber, &point) != EOF)
    {
        if (strcmp(phoneNumber, u.number) == 0) // 찾아서 있으면
            fprintf(copyFile, "%s %s %s %s %s %f\n", name, id, password, birth, phoneNumber, u.point);
        else
            fprintf(copyFile, "%s %s %s %s %s %f\n", name, id, password, birth, phoneNumber, point);
    }
    fclose(originalFile);
    fclose(copyFile);

    originalFile = fopen("MemberCopy.txt", "r");
    copyFile = fopen("Member.txt", "w");

    if (originalFile == NULL || copyFile == NULL)
    {
        printf("Error opening files.\n");
        return 1;
    }

    while (fscanf(originalFile, "%s %s %s %s %s %lf", name, id, password, birth, phoneNumber, &point) != EOF)
    {
        if (strcmp(phoneNumber, u.number) == 0) // 찾아서 있으면
            fprintf(copyFile, "%s %s %s %s %s %f\n", name, id, password, birth, phoneNumber, u.point);
        else
            fprintf(copyFile, "%s %s %s %s %s %f\n", name, id, password, birth, phoneNumber, point);
    }

    fclose(originalFile);
    fclose(copyFile);

    remove("MemberCopy.txt");
    return 2;
}


int reservation(struct Datetime* dt)
{
    struct CurrentTime ct;
    CurrentTime(&ct, dt);

    //current date & current time 
    int cd = ct.year * 10000 + ct.month * 100 + ct.day;
    int ctt = ct.hour * 100 + ct.minute;

    reserve.moviedate = cd;

    //사용자 나이 계산
    int useryear = u.birth[0] * 1000 + u.birth[1] * 100 + u.birth[2] * 10 + u.birth[3];
    int usermonth = u.birth[4] * 10 + u.birth[5];
    int userday = u.birth[6] * 10 + u.birth[7];

    int age = 0; //user age
    if (usermonth < ct.month) age = ct.year - useryear;
    else if (usermonth == ct.month && userday >= ct.month) age = ct.year - useryear;
    else age = ct.year - useryear - 1;

    int numTotal = 0;

    //1.select Movie title
    if (selectMovie(ct, age) == 0)
    {
        return 0;
    }

    int count = TheaterFile(ct, dt, reserve.movietitle); //file open & update agelimit

    //2.select Time 
    if (selectTime(ct, count) == 0)
    {
        return 0;
    }

    //count seat Available
    int index = 0;      // *****선택한 시간의 인덱스******
    for (int i = 0; i < 4; i++)
    {
        if (!strcmp(reserve.movietime, rtime[i]))
        {
            index = i;
            break;
        }
    }

    int seatAvailable = 0;
    for (int i = 0; i < 10; i++)
    {
        if (rseat[index][i] == '0') seatAvailable++;
    }

    //3.select num
    if (selectNum(ct, seatAvailable) == 0)
    {
        return 0;
    }


    //4.select seat
    if (selectSeat(ct, index, seatAvailable) == 0)
    {
        return 0;
    }

    //5.check Charge
    if (checkCharge(ct) == 0)
    {
        return 0;
    }

    //i) user : Member
    int a;
    if (u.member == 1) // 회원으로 진행했을 때
    {
        //6-1. using coupon
        a = usingCoupon(ct);
        setCharge(a);
        //6-2. using point
        usingPoint(ct);
    }

    if (payCharge(ct) == 0)
    {
        return 0;
    }


    reserve_file_Update();
    theater_file_Update(index, cd);
    if (a != -1 && u.member == 1) // 쿠폰 고르면
        rewriteCouponFile(a);
    mem_file_Update();

    return 1;
}