#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "isValidDataFile.h"

int authenticateID(int* index, char* id) {
    FILE* fp;
    char sTrash[31];
    char ID[31];
    double point = 0;
    *index = 0;
    isValidDataFile("Member.txt");
    fopen_s(&fp, "Member.txt", "r");
    if (fp == NULL) {
        return -1;
    }

    while (fscanf(fp, "%s %s %s %s %s %lf ", sTrash, ID, sTrash, sTrash, sTrash, &point) != EOF) {


        if (strcmp(id, ID) == 0)
        {
            fclose(fp);
            return 1;
        }
        (*index)++;
    }

    fclose(fp);
    return 0;


}


int authenticatePassword(int* row, char* data) {
    FILE* fp;
    char sTrash[31];
    char password[31];
    char number[14];
    char birth[30];
    double point = 0;
    int count = 0;
    isValidDataFile("Member.txt");
    fopen_s(&fp, "Member.txt", "r");
    if (fp == NULL) {
        return -1;
    }

    while (fscanf(fp, "%s %s %s %s %s %lf", sTrash, sTrash, password, birth, number, &point) != EOF) {
        if (count == *row && strcmp(password, data) == 0) {
            u.member = 1;
            for (int i = 0; i < 8; i++) {
                u.birth[i] = birth[i] - '0';
            }
            for (int i = 0; i < 14; i++) {
                u.number[i] = number[i];
            }

            u.point = point;
            fclose(fp);
            return 1;
        }
        count++;
    }
    fclose(fp);
    return 0;

}

int loginLog(struct DateTime* dt) {
    FILE* fp;
    isValidDataFile("LogData.csv");
    fopen_s(&fp, "LogData.csv", "a");
    if (fp == NULL) {
        return -1;
    }

    char year[5];
    char month[3];
    char day[3];
    char hour[3];
    char minute[3];
            
    sprintf(year, "%d", dt->year);
           
    if (dt->month < 10) {
        sprintf(month, "0%d", dt->month);
                
    }
    else {
        sprintf(month, "%d", dt->month);
    }
    if (dt->day < 10) {
        sprintf(day, "0%d", dt->day);
    }
    else {
        sprintf(day, "%d", dt->day);
    }
    if (dt->hour < 10) {
        sprintf(hour, "0%d", dt->hour);
    }
    else {
        sprintf(hour, "%d", dt->hour);
    }
    if (dt->minute < 10) {
        sprintf(minute, "0%d", dt->minute);
    }
    else {
        sprintf(minute, "%d", dt->minute);
    }
    fprintf(fp, "[Login],%s%s%s,%s%s,%s\n", year, month, day, hour,minute, u.number);
    fclose(fp);
    return 1;

}
int logoutLog(struct DateTime* dt, int member) {
    FILE* fp;
    isValidDataFile("LogData.csv");
    fopen_s(&fp, "LogData.csv", "a");
    if (fp == NULL) {
        return -1;
    }

    char year[5];
    char month[3];
    char day[3];
    char hour[3];
    char minute[3];

    sprintf(year, "%d", dt->year);

    if (dt->month < 10) {
        sprintf(month, "0%d", dt->month);

    }
    else {
        sprintf(month, "%d", dt->month);
    }
    if (dt->day < 10) {
        sprintf(day, "0%d", dt->day);
    }
    else {
        sprintf(day, "%d", dt->day);
    }
    if (dt->hour < 10) {
        sprintf(hour, "0%d", dt->hour);
    }
    else {
        sprintf(hour, "%d", dt->hour);
    }
    if (dt->minute < 10) {
        sprintf(minute, "0%d", dt->minute);
    }
    else {
        sprintf(minute, "%d", dt->minute);
    }
    if (member == 1) {
        fprintf(fp, "[Logout],%s%s%s,%s%s,%s\n", year, month, day, hour, minute, u.number);
    }
    if (member == 0) {
        fprintf(fp, "[GuestLogout],%s%s%s,%s%s,%s\n", year, month, day, hour, minute, u.number);
    }
    fclose(fp);
    return 1;

}

int main_Login(struct DateTime* dt) {
    char ID[31];
    char password[31];
    int passwordAttempts = 0;
    int row = 0;

    while (1) {
        printf("****************************\n[Please enter ID]\n\nEnter :");
        if (scanf_s("%[^\n]s", ID, sizeof(ID)) == 0) {
            while (getchar() != '\n');
            printf("The number of characters exceeds or just input Enter.\n");
            continue;
        }
        // 엔터 이외의 모든 문자열 입력 받기

        //입력 버퍼 제거 
        while (getchar() != '\n');


        if (strcmp(ID, "q") == 0) {
            return 0;
        }
        else {
            int result = authenticateID(&row, ID);
            if (result == 1) {
                break;
            }
            else if (result == 0) {
                printf("ID doesn’t exist. Please check it again.\n");
                continue;
            }
            else {
                return -1;
            }
        }
    }
    while (1) {
        printf("****************************\n[Please enter Password]\n");
        printf("wrong times: %d\nremains:%d\n\nEnter :", passwordAttempts, 4 - passwordAttempts);
        if (scanf_s("%[^\n]s", password, sizeof(password)) == 0) {
            while (getchar() != '\n');
            passwordAttempts++;
            printf("The number of characters exceeds or just input Enter.\n");
            if (passwordAttempts == 4) {
                printf("You wrong Password more than 3 times.\n");
                return 0;
            }
            continue;
        } // 엔터 이외의 모든 문자열 입력 받기

        while (getchar() != '\n');//버퍼 초기화
        if (strcmp(password, "q") == 0) {
            return 0;
        }

        int result = authenticatePassword(&row, password);
        if (result == 1) {
            if (loginLog(dt) == 0) {
                return -1;
            }
            return 1;
        }
        else if (result == 0) {
            ++passwordAttempts;
            if (passwordAttempts == 4) {
                printf("You wrong Password more than 3 times.\n");
                return 0;
            }
            printf("Password is wrong. Please check it again.\n");
            continue;
        }
        else {
            return -1;
        }
    }

}
