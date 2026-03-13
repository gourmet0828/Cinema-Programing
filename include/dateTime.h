// dateTIme.h
#pragma once
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <Windows.h>
#include "isValidDataFile.h"

#define INITIAL_YEAR 1950
#define INITIAL_MONTH 1
#define INITIAL_DAY 1
#define INITIAL_HOUR 0
#define INITIAL_MINUTE 0
#define RECORD_MAXLENGTH 100;

struct DateTime
{
    int year, month, day;
    int hour, minute;
};
struct user {
    int member;
    int birth[8];
    char number[14];
    double point;
}u;
void DateTime(struct DateTime* dT);
int inputDateTime(struct DateTime* cDT);
void setDateTime(struct DateTime* cDT, char* userinput);
char* rightDTFormat(char* userinput);
void updateDateTime(struct DateTime* cDT, char* userinput);
void updateDateTimeFile(char* userinput);
bool isValidDateTime(const char* userinput);
bool isLeapYear(int year);
bool isLaterDateTime(struct DateTime* cDT, char* userinput);
void printDateTime(struct DateTime* dateTime);

int Sales_Cal(char* PhoneNumber, struct DateTime* cDT) {
    int reservation_cnt = 0; // 예매 횟수를 저장하는 변수
    char date[11];  // "YYYY-MM-DD" 형식의 날짜를 저장하기 위해 11 크기로 선언
    int year, month, day;  // date 변수에서 년, 월, 일 정보를 저장하는 변수
    char phone[14];  // 전화번호를 저장하는 변수
    char movie[50];  // 영화 이름을 저장하는 변수
    int time;  // 예매 시간을 저장하는 변수
    char seat[10];  // 좌석 정보를 저장하는 변수

    isValidDataFile("Reservation.csv");
    FILE* reservationFile = fopen("Reservation.csv", "r");
    if (reservationFile == NULL) {
        printf("fail to read file");
        return 0;
    }

    // reservation.csv 파일을 읽어 현재 날짜 기준 이전 분기의 예매 내역을 확인
    while (fscanf(reservationFile, "%[^,],%[^,],%[^,],%d,%[^\n]\n", phone, movie, date, &time, seat) == 5) {
        sscanf(date, "%4d%2d%2d", &year, &month, &day);
        if (strcmp(PhoneNumber, phone) == 0) {
            //    printf("전화번호: %s",phone);
                // 현재 분기의 이전 분기 계산

            if (cDT->month % 3 == 1) {  //1,4,7,10월
                //      printf("%d", month);
                if (cDT->month == 1 && (cDT->year - 1) == year) {
                    if ((cDT->month + 9) <= month && (cDT->month + 11) >= month) {
                        reservation_cnt++;
                    }

                }
                else if ((cDT->month - 3) <= month && (cDT->month - 1) >= month && cDT->year == year) {
                    reservation_cnt++;

                }
            }

            else if (cDT->month % 3 == 2) { //2,5,8,11월
                if (cDT->month == 2 && (cDT->year - 1) == year) {
                    if ((cDT->month + 8) <= month && (cDT->month + 10) >= month) {
                        reservation_cnt++;
                    }
                }
                else if ((cDT->month - 4) <= month && (cDT->month - 2) >= month && cDT->year == year) {
                    reservation_cnt++;
                }

            }
            else if (cDT->month % 3 == 0) { //3,6,9,12월
                if (cDT->month == 3 && (cDT->year - 1) == year) {
                    if ((cDT->month + 7) <= month && (cDT->month + 9) >= month) {
                        reservation_cnt++;
                    }
                }
                else if ((cDT->month - 5) <= month && (cDT->month - 3) >= month && cDT->year == year) {
                    reservation_cnt++;
                }
            }

        }
    }

    fclose(reservationFile);
    if (reservation_cnt < 5) {
        return 0;
    }
    else if (reservation_cnt >= 5 && reservation_cnt < 7) {
        return 5;
    }
    else if (reservation_cnt >= 7 && reservation_cnt < 10) {
        return 7;
    }
    else if (reservation_cnt >= 10) {
        return 10;
    }

    return 0;
}


void editCouponY(int sales_result, char* PhoneNumber, struct DateTime* dT, int flag) {
    char buf[150];
    int quater;
    int couponQuater = 0;
    int exQuater;
    char* p_temp;
    char issueMonth[3];
    char dueDate[9];
    int state = 2;
    if (dT->month / 4 == 0) { quater = 1; }
    else if (dT->month / 7 == 0) { quater = 2; }
    else if (dT->month / 10 == 0) { quater = 3; }
    else { quater = 4; }
    if (sales_result != 0) {
        state = 0;
    }

    switch (quater) {
    case 1:
        strcpy(issueMonth, "01");
        exQuater = 4;
        sprintf(dueDate, "%d0630", dT->year);
        break;
    case 2:
        strcpy(issueMonth, "04");
        exQuater = quater - 1;
        sprintf(dueDate, "%d0930", dT->year);
        break;
    case 3:
        strcpy(issueMonth, "07");
        exQuater = quater - 1;
        sprintf(dueDate, "%d1231", dT->year);
        break;
    case 4:
        strcpy(issueMonth, "10");
        exQuater = quater - 1;
        sprintf(dueDate, "%d0331", dT->year + 1);
    }
    char* restOfNumber = PhoneNumber;
    char* dash = strchr(PhoneNumber, '-');
    if (dash != NULL) {
        restOfNumber = dash + 1;
    }
    char txt[150];

    sprintf(txt, "Y%d-%s %s %d%s01 %s %%%%%d %d\n", quater, restOfNumber, PhoneNumber, dT->year, issueMonth, dueDate, sales_result, state);


    FILE* couponFile = fopen("CouponY.txt", "r");
    FILE* outputFile = fopen("newCouponY.txt", "w");
    isValidDataFile("CouponY.txt");

    if (flag == 1) {

        while (fgets(buf, 150, couponFile) != NULL) {
            p_temp = strstr(buf, PhoneNumber);
            if (p_temp != NULL) {
                couponQuater++;
                if (couponQuater == quater) {
                    fprintf(outputFile, txt);
                }
                else if (couponQuater == exQuater) {
                    fprintf(outputFile, "%s", buf);

                }
                else {
                    buf[strlen(buf) - 2] = '2';
                    fprintf(outputFile, "%s", buf);
                }

            }
            else {

                fprintf(outputFile, "%s", buf);

            }
        }
    }
    else if (flag == 2 || flag == 3) {
        while (fgets(buf, 128, couponFile) != NULL) {
            p_temp = strstr(buf, PhoneNumber);
            if (p_temp != NULL) {
                couponQuater++;
                if (couponQuater == quater) {
                    fprintf(outputFile, txt);
                }
                else {
                    buf[strlen(buf) - 2] = '2';
                    fprintf(outputFile, "%s", buf);
                }
            }
            else {
                fprintf(outputFile, "%s", buf);
            }
        }

    }
    fclose(couponFile);
    fclose(outputFile);

    remove("CouponY.txt");
    rename("newCouponY.txt", "CouponY.txt");

}
void manageCouponY(struct DateTime* dT, int conditionResult) {
    isValidDataFile("Member.txt");
    FILE* memberFile = fopen("Member.txt", "r");
    char phone_number[14];
    char sTrash[31];
    int sales_result;
    if (conditionResult == 1) {
        while (fscanf(memberFile, "%s %s %s %s %s %lf ", sTrash, sTrash, sTrash, sTrash, phone_number, sTrash) != EOF) {
            sales_result = Sales_Cal(phone_number, dT);
            editCouponY(sales_result, phone_number, dT, 1);
        }
    }
    else if (conditionResult == 2) {
        printf("2차이남\n");
        struct DateTime* temp = (struct DateTime*)malloc(sizeof(struct DateTime));
        if (dT->month == 1 || dT->month == 2 || dT->month == 3) {
            temp->year = dT->year - 1;
            temp->month = dT->month + 9;
        }
        else {
            temp->year = dT->year;
            temp->month = dT->month - 3;
        }
        while (fscanf(memberFile, "%s %s %s %s %s %lf ", sTrash, sTrash, sTrash, sTrash, phone_number, sTrash) != EOF) {
            sales_result = Sales_Cal(phone_number, temp);
            editCouponY(sales_result, phone_number, temp, 2);
        }
        free(temp);
    }
    else if (conditionResult == 3) {
        while (fscanf(memberFile, "%s %s %s %s %s %lf ", sTrash, sTrash, sTrash, sTrash, phone_number, sTrash) != EOF) {
            sales_result = Sales_Cal(phone_number, dT);
            editCouponY(sales_result, phone_number, dT, 3);
        }
    }
    fclose(memberFile);
}

int editCondition(int byear, int bmonth, struct DateTime* dt) {
    int bQuater;
    if (bmonth / 4 == 0) { bQuater = 1; }
    else if (bmonth / 7 == 0) { bQuater = 2; }
    else if (bmonth / 10 == 0) { bQuater = 3; }
    else { bQuater = 4; }
    int cQuater;
    if (dt->month / 4 == 0) { cQuater = 1; }
    else if (dt->month / 7 == 0) { cQuater = 2; }
    else if (dt->month / 10 == 0) { cQuater = 3; }
    else { cQuater = 4; }

    if (byear == dt->year && bQuater == cQuater) return 0;
    if (byear == dt->year && cQuater - bQuater == 2) return 2;
    if (byear == dt->year && cQuater - bQuater == 1) return 1;
    if (byear == dt->year - 1 && bQuater - cQuater == 2) return 2;
    if (byear == dt->year - 1 && bQuater - cQuater == 3) return 1;
    else return 3;
}


void DateTime(struct DateTime* dT)
{
    dT->year = INITIAL_YEAR;
    dT->month = INITIAL_MONTH;
    dT->day = INITIAL_DAY;
    dT->hour = INITIAL_HOUR;
    dT->minute = INITIAL_MINUTE;


    if (FileExist("DateTime.csv"))
    {
        if (canReadFile("DateTime.csv") && canWriteFile("DateTime.csv"))
        {
            FILE* testfile = fopen("DateTime.csv", "r");
            char record[100];
            fgets(record, 100, testfile);
            fclose(testfile);

            for (int i = 0; i < strlen(record); i++)
            {
                if (record[i] == ',')
                {
                    record[i] = '/';
                }
            }

            if (isValidDateTime(record))
            {
                FILE* file = fopen("DateTime.csv", "r");
                char record[100];
                fgets(record, 100, file);
                fclose(file);

                for (int i = 0; i < strlen(record); i++)
                {
                    if (record[i] == ',')
                    {
                        record[i] = '/';
                    }
                }
                updateDateTime(dT, rightDTFormat(record));
            }
            else
            {
                printf("Error: DataFile against the rule. Initialize all file\n");
                FILE* file = fopen("DateTime.csv", "w");
                fputs("19500101,0000", file);
            }
        }
        else
        {
            printf("> Error: Cannot access to DataFile. Terminate the program");
            exit(0);
        }
    }
    else
    {
        printf(" Error: DataFile Not Found. Create missed file and initialize existing file.\n");
        FILE* file = fopen("DateTime.csv", "w");
        fputs("19500101,0000", file);
        fclose(file);
    }


    printf("Initial Date/Time\n");
    printDateTime(dT);

}
// 요구함수
int inputDateTime(struct DateTime* cDT)
{
    char userinput[22];
    while (true)
    {
        printf("[Please enter the current date and time]\n\n");
        printf("Enter : ");
        fgets(userinput, 22, stdin);
        if (strchr(userinput, '\n') == NULL)
            while (getchar() != '\n');
        if (userinput[strlen(userinput) - 1] == '\n')
            userinput[strlen(userinput) - 1] = '\0';
        if (!isValidDateTime(userinput))
        {
            printf("It's not the correct format of date and time.\n");
            printf("Format: Date(000000) / Time(0000)\n");
            printf("Please check it again\n");
            continue;
        }
        if (!isLaterDateTime(cDT, userinput))
        {
            printf("Input datetime is past from the current.(126)\n");
            continue;
        }
        if (isValidDateTime(userinput))
        {
            setDateTime(cDT, userinput);
            printDateTime(cDT);
            break;
        }
    }

}

// 문자열이 규칙에 맞는지 확인
// 문자열로 받은 날짜/시간을 구조체에 저장
// 문자열로 받은 날짜/시간을 파일에 저장

void setDateTime(struct DateTime* cDT, char* userinput)
{
    int dateBefore = cDT->year * 10000 + cDT->month * 100 + cDT->day;


    if (isValidDateTime(userinput))
    {
        if (isLaterDateTime(cDT, rightDTFormat(userinput)))
        {

            char dateTimeBefore[14];

            isValidDataFile("DateTime.csv");
            FILE* beforeFile = fopen("DateTime.csv", "r");
            fgets(dateTimeBefore, 14, beforeFile);
            fclose(beforeFile);
            int yearBefore = cDT->year;
            int monthBefore = cDT->month;

            updateDateTime(cDT, rightDTFormat(userinput));
            updateDateTimeFile(rightDTFormat(userinput));
            int conditionResult = editCondition(yearBefore, monthBefore, cDT);
            printf("conditionResult: %d\n", conditionResult);
            if (conditionResult != 0)
                manageCouponY(cDT, conditionResult);



            char dateTimeAfter[14];
            strcpy(dateTimeAfter, rightDTFormat(userinput));
            for (int i = 0; i < 14; i++)
            {
                if (dateTimeAfter[i] == '/') {
                    dateTimeAfter[i] = ',';
                }
            }
            dateTimeAfter[13] = '\0';



            char record[100] = "";

            strcat(record, "[changeDateTime]");
            strcat(record, ",");
            strcat(record, dateTimeBefore);
            strcat(record, ",");
            strcat(record, dateTimeAfter);
            strcat(record, "\n");

            isValidDataFile("LogData.csv");
            FILE* logFile = fopen("LogData.csv", "a");
            fputs(record, logFile);
            fclose(logFile);

        }
        else
        {
            printf("Input datetime is past from the current.\n");
        }
    }
    else
    {
        printf("It's not the correct format of date and time.");
        printf("Format : Date(000000) / Time(0000) Please check it again.");
    }


    // 날짜가 바뀌면, 좌석정보파일 초기화
    static bool flag = false;

    if (flag)
    {
        int dateAfter = cDT->year * 10000 + cDT->month * 100 + cDT->day;
        char strdateAfter[9];
        sprintf(strdateAfter, "%d", dateAfter);
        if (dateBefore < dateAfter)
        {
            FILE* afile = fopen("TheaterA.csv", "w");
            fclose(afile);
            FILE* Afile = fopen("TheaterA.csv", "a");
            char Arecord[4][50];
            for (int i = 0; i < 4; i++)
                strcpy(Arecord[i], "");

            for (int i = 0; i < 4; i++)
            {
                strcat(Arecord[i], "Dune,");
                strcat(Arecord[i], strdateAfter);
                if (i == 0)
                    strcat(Arecord[i], ",1200");
                else if (i == 1)
                    strcat(Arecord[i], ",1400");
                else if (i == 2)
                    strcat(Arecord[i], ",1600");
                else if (i == 3)
                    strcat(Arecord[i], ",1800");
                strcat(Arecord[i], ",60");
                strcat(Arecord[i], ",0000000000");
                strcat(Arecord[i], "\n");
            }

            for (int i = 0; i < 4; i++)
            {
                fputs(Arecord[i], Afile);
            }


            FILE* bfile = fopen("TheaterB.csv", "w");
            fclose(afile);
            FILE* Bfile = fopen("TheaterB.csv", "a");
            char Brecord[4][50];
            for (int i = 0; i < 4; i++)
                strcpy(Brecord[i], "");

            for (int i = 0; i < 4; i++)
            {
                strcat(Brecord[i], "Comment Unit,");
                strcat(Brecord[i], strdateAfter);
                if (i == 0)
                    strcat(Brecord[i], ",1200");
                else if (i == 1)
                    strcat(Brecord[i], ",1400");
                else if (i == 2)
                    strcat(Brecord[i], ",1600");
                else if (i == 3)
                    strcat(Brecord[i], ",1800");
                strcat(Brecord[i], ",60");
                strcat(Brecord[i], ",0000000000");
                strcat(Brecord[i], "\n");
            }

            for (int i = 0; i < 4; i++)
            {
                fputs(Brecord[i], Bfile);
            }

            FILE* cfile = fopen("TheaterC.csv", "w");
            fclose(afile);
            FILE* Cfile = fopen("TheaterC.csv", "a");
            char Crecord[4][50];
            for (int i = 0; i < 4; i++)
                strcpy(Crecord[i], "");

            for (int i = 0; i < 4; i++)
            {
                strcat(Crecord[i], "The Beekeeper,");
                strcat(Crecord[i], strdateAfter);
                if (i == 0)
                    strcat(Crecord[i], ",1200");
                else if (i == 1)
                    strcat(Crecord[i], ",1400");
                else if (i == 2)
                    strcat(Crecord[i], ",1600");
                else if (i == 3)
                    strcat(Crecord[i], ",1800");
                strcat(Crecord[i], ",60");
                strcat(Crecord[i], ",0000000000");
                strcat(Crecord[i], "\n");
            }

            for (int i = 0; i < 4; i++)
            {
                fputs(Crecord[i], Cfile);
            }

            fclose(Afile);
            fclose(Bfile);
            fclose(Cfile);
        }
    }
    else
    {
        flag = true;
    }




}



// "YYYYMMDD/HHMM" 을 리턴합니다. 사용자 입력이 정확해야 합니다.
char* rightDTFormat(char* userinput)
{
    // 숫자만 남기기
    char* dateTemp = strtok(_strdup(userinput), "/");
    char date[9];
    char* timeTemp = strtok(NULL, "/");
    char time[5];
    int j = 0;
    for (int i = 0; i < strlen(dateTemp); i++)   // 문법규칙 : 10자리 초과하는 이후 문자는 처리x
    {
        // date에 숫자만 남기기
        if (isdigit(dateTemp[i]))
        {
            date[j++] = dateTemp[i];
        }
    }
    date[j] = '\0';

    // 길이가 6이면 19나20 붙이기 
    if (strlen(date) == 6)
    {
        char temp[9];
        if (date[0] >= '5')
        {
            strcpy(temp, date);
            strcpy(date, "19");
            strcat(date, temp);
        }
        else
        {
            strcpy(temp, date);
            strcpy(date, "20");
            strcat(date, temp);
        }
    }

    int k = 0;
    for (int i = 0; i < strlen(timeTemp); i++)   // 문법규칙 : 10자리 초과하는 이후 문자는 처리x
    {
        // date에 숫자만 남기기
        if (isdigit(timeTemp[i]))
        {
            time[k++] = timeTemp[i];
        }
    }
    time[k] = '\0';

    char* rightDT = (char*)malloc(sizeof(char) * 14);
    strcpy(rightDT, date);
    strcat(rightDT, "/");
    strcat(rightDT, time);

    return rightDT;
}
// setDateTime에서만 사용되므로, userinput 형식이 "YYYYMMDD/HHMM" 이라고 가정합니다.
void updateDateTime(struct DateTime* cDT, char* userinput)
{
    char* date = strtok(_strdup(userinput), "/");;
    char* time = strtok(NULL, "/");

    char strYear[5], strMonth[3], strDay[3];
    strncpy(strYear, date, 4);
    strncpy(strMonth, date + 4, 2);
    strncpy(strDay, date + 6, 2);

    cDT->year = atoi(strYear);
    cDT->month = atoi(strMonth);
    cDT->day = atoi(strDay);

    char strHour[3], strMinute[3];
    strncpy(strHour, time, 2);
    strncpy(strMinute, time + 2, 2);

    cDT->hour = atoi(strHour);
    cDT->minute = atoi(strMinute);
}

// setDateTime에서만 사용되므로, userinput 형식이 "YYYYMMDD/HHMM" 이라고 가정합니다.
void updateDateTimeFile(char* userinput)
{
    if (isValidDataFile("DateTime.csv") && isValidDateTime(userinput))
    {
        FILE* file = fopen("DateTime.csv", "w");
        for (int i = 0; i < strlen(userinput); i++)
            if (userinput[i] == '/')
                userinput[i] = ',';
        fputs(userinput, file);
        fclose(file);
    }
}


// 사용자입력이 맞는지 확인
bool isValidDateTime(const char* userinput)
{
    // 공백이 없으면 true
    bool flag = true;
    for (int i = 0; i < strlen(userinput); i++)
    {
        if (isspace(userinput[i]))
        {
            flag = false;
            break;
        }
    }

    // input (길이가 11 혹은 13) 이고 공백이 없어야 함
    if (!((strlen(userinput) == 11 || strlen(userinput) == 13) && flag))
        return false;



    int count = 0;
    for (int i = 0; i < strlen(userinput); i++)
    {
        if (userinput[i] == '/')
        {
            count++;
        }
    }
    if (count != 1)
    {
        return false;
    }

    char* dateTemp = strtok(_strdup(userinput), "/");
    char date[9];
    char* timeTemp = strtok(NULL, "/");
    char time[5];

    int j = 0;
    for (int i = 0; i < strlen(dateTemp); i++)   // 문법규칙 : 10자리 초과하는 이후 문자는 처리x
    {
        // date에 숫자만 남기기
        if (isdigit(dateTemp[i]))
        {
            date[j++] = dateTemp[i];
        }
    }
    date[j] = '\0';

    // 숫자 길이가 6,8 아니면 false
    if (strlen(date) != 6 && strlen(date) != 8)
    {
        return false;
    }


    // 길이가 6이면 19나20 붙이기 
    if (strlen(date) == 6)
    {
        char temp[9];
        if (date[0] >= '5')
        {
            strcpy(temp, date);
            strcpy(date, "19");
            strcat(date, temp);
        }
        else
        {
            strcpy(temp, date);
            strcpy(date, "20");
            strcat(date, temp);
        }
    }


    // date가 1950-2049사이의 그레고리력에 있는 날짜인지 확인
    char strYear[5], strMonth[3], strDay[3];
    strncpy(strYear, date, 4);
    strncpy(strMonth, date + 4, 2);
    strncpy(strDay, date + 6, 2);

    int year = atoi(strYear);
    int month = atoi(strMonth);
    int day = atoi(strDay);

    if (year < 1950 || year > 2049)   return false;
    if (month < 1 || month > 12)   return false;

    int daysinMonth = 0;
    switch (month)
    {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        daysinMonth = 31;
        break;
    case 4: case 6: case 9: case 11:
        daysinMonth = 30;
        break;
    case 2:
        daysinMonth = isLeapYear(year) ? 29 : 28;
        break;
    default:
        break;
    }
    if (day < 1 || day > daysinMonth)   return false;




    if (strlen(timeTemp) != 4)
    {
        printf("\"/\" next, only 4 digits can be entered\n");
        return false;
    }

    // 시간
    int k = 0;
    for (int i = 0; i < strlen(timeTemp); i++)   // 문법규칙 : 10자리 초과하는 이후 문자는 처리x
    {
        // date에 숫자만 남기기
        if (isdigit(timeTemp[i]))
        {
            time[k++] = timeTemp[i];
        }
    }
    time[k] = '\0';

    // newcode
    if (strlen(time) != 4)
    {
        printf("\"/\" next, only 4 digits can be entered\n");
        return false;
    }
    for (int i = 0; i < strlen(time); i++)
    {
        if (!isdigit(time[i]))
        {
            printf("\"/\" next, only 4 digits can be entered\n");
            return false;
        }
    }
    // newcode

    // 숫자 길이가 6,8 아니면 false

    // 시간이 0~23 분이 0~59 사이인지 확인
    char strHour[3], strMinute[3];
    strncpy(strHour, time, 2);
    strncpy(strMinute, time + 2, 2);

    int hour = atoi(strHour);
    int minute = atoi(strMinute);

    if (hour < 0 || hour > 23)   return false;
    if (minute < 0 || minute > 59)   return false;
    return true;


}

bool isLeapYear(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isLaterDateTime(struct DateTime* cDT, char* userinput)
{
    char* date = strtok(_strdup(userinput), "/");;
    char* time = strtok(NULL, "/");
    char strYear[5], strMonth[3], strDay[3];
    char strHour[3], strMinute[3];
    if (strlen(userinput) == 13)
    {
        strncpy(strYear, date, 4);
        strncpy(strMonth, date + 4, 2);
        strncpy(strDay, date + 6, 2);
        strncpy(strHour, time, 2);
        strncpy(strMinute, time + 2, 2);
    }

    if (strlen(userinput) == 11)
    {
        strcpy(strYear, "20");
        strncat(strYear, date, 2);
        strncpy(strMonth, date + 2, 2);
        strncpy(strDay, date + 4, 2);
        strncpy(strHour, time, 2);
        strncpy(strMinute, time + 2, 2);
    }

    int year = atoi(strYear);
    int month = atoi(strMonth);
    int day = atoi(strDay);
    int hour = atoi(strHour);
    int minute = atoi(strMinute);

    if (cDT->year > year)   return false;
    if (cDT->year == year && cDT->month > month)   return false;
    if (cDT->year == year && cDT->month == month && cDT->day > day)   return false;
    if (cDT->year == year && cDT->month == month && cDT->day == day && cDT->hour > hour)   return false;
    if (cDT->year == year && cDT->month == month && cDT->day == day && cDT->hour == hour && cDT->minute > minute)   return false;
    if (cDT->year == year && cDT->month == month && cDT->day == day && cDT->hour == hour && cDT->minute == minute)   return true;


    return true;

}

void printDateTime(struct DateTime* dateTime)
{
    printf("Year: %d\n", dateTime->year);
    printf("Month: %d\n", dateTime->month);
    printf("Day: %d\n", dateTime->day);
    printf("Hour: %d\n", dateTime->hour);
    printf("Minute: %d\n", dateTime->minute);
}