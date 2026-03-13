// zCouponManager
#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "dateTime.h"

typedef struct coupon
{
    char cpnNum[20];
    char userPhoneNum[20];
    char issueDate[20];
    char expireDate[20];
    char kindOfCpn[20];
    int valid; // 0 사용 가능, 1 기한내에 사용, 2 유효하지 않음 

} zCpn;

zCpn zcpn;
int birth;
int currentDate;
int afterBirth;

void getCouponZ()
{
    FILE* fp = fopen("CouponZ.txt", "r");
    while (fscanf(fp, "%s %s %s %s %s %d", zcpn.cpnNum, zcpn.userPhoneNum, zcpn.issueDate, zcpn.expireDate, zcpn.kindOfCpn, &zcpn.valid) != EOF)
    {
        if (strcmp(zcpn.userPhoneNum, u.number) == 0)
        {
            break;
        }
    }
    fclose(fp);
}

void updateCouponZFile(int p)
{
    FILE* originalfile = fopen("CouponZ.txt", "r");
    FILE* copyfile = fopen("CouponZCopy.txt", "w");

    zCpn tmp;
    while (fscanf(originalfile, "%s %s %s %s %s %d", tmp.cpnNum, tmp.userPhoneNum, tmp.issueDate, tmp.expireDate, tmp.kindOfCpn, &tmp.valid) != EOF)
    {
        if (strcmp(tmp.userPhoneNum, u.number) == 0) // 동일한 쿠폰 번호 찾으면
        {
            if (p == 1)
                fprintf(copyfile, "%s %s %d %d %s %d\n", tmp.cpnNum, tmp.userPhoneNum, birth, afterBirth, tmp.kindOfCpn, 0); // valid 1로 입력(기한내에 사용 했다는 플래그)
            else if (p == 2)
                fprintf(copyfile, "%s %s %s %s %s %d\n", tmp.cpnNum, tmp.userPhoneNum, tmp.issueDate, tmp.expireDate, tmp.kindOfCpn, 2);
        }
        else
            fprintf(copyfile, "%s %s %s %s %s %d\n", tmp.cpnNum, tmp.userPhoneNum, tmp.issueDate, tmp.expireDate, tmp.kindOfCpn, tmp.valid);
    }
    fclose(originalfile);
    fclose(copyfile);

    copyfile = fopen("CouponZCopy.txt", "r");
    originalfile = fopen("CouponZ.txt", "w");

    while (fscanf(copyfile, "%s %s %s %s %s %d", tmp.cpnNum, tmp.userPhoneNum, tmp.issueDate, tmp.expireDate, tmp.kindOfCpn, &tmp.valid) != EOF)
    {
        fprintf(originalfile, "%s %s %s %s %s %d\n", tmp.cpnNum, tmp.userPhoneNum, tmp.issueDate, tmp.expireDate, tmp.kindOfCpn, tmp.valid);
    }

    fclose(originalfile);
    fclose(copyfile);

    remove("CouponZCopy.txt");
}

int decideUpdateZ(struct DateTime* dt) // 
{
    // < 생일 >
    int year = dt->year;
    int month = 10 * u.birth[4] + u.birth[5];
    int day = 10 * u.birth[6] + u.birth[7];

    birth = year * 10000 + month * 100 + day;

    // < 현재날짜 >
    currentDate = 0;
    currentDate = dt->year * 10000 + dt->month * 100 + dt->day;

    // < 생일 + 7 >
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

    day += 7;

    while (day > daysinMonth)
    {
        day -= daysinMonth;
        month++;
        if (month > 12)
        {
            month = 1;
            year++;
        }
        daysinMonth = 0;
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
    }

    afterBirth = year * 10000 + month * 100 + day;

    int c = birth % 10000;
    if ((c == 1225 || c == 1226 || c == 1227 || c == 1228 || c == 1229 || c == 1230 || c == 1231))
    {
        if (dt->month == 1)
        {
            birth = birth - 10000;
            afterBirth = afterBirth - 10000;
        }
    }

    printf("<쿠폰>\n");
    printf("생일: %s\n", zcpn.issueDate);
    printf("만료: %s\n", zcpn.expireDate);

    printf("<현재날짜기준>\n");
    printf("생일: %d\n", birth);
    printf("현재: %d\n", currentDate);
    printf("만료: %d\n", afterBirth);

    int fileDate = atoi(zcpn.issueDate);
    /*if (fileDate/10000 != birth/10000)
        updateCouponZFile(1);*/

    if (((birth <= currentDate && currentDate <= afterBirth) && zcpn.valid == 2) || ((birth <= currentDate && currentDate <= afterBirth) && zcpn.valid == 1 && fileDate / 10000 != birth / 10000))
        return 1;
    else if (!(birth <= currentDate && currentDate <= afterBirth) && ((zcpn.valid == 0) || (zcpn.valid == 1)))
        return 2;
    else
        return 0;
}

void ZCoupon(struct DateTime* dt)
{
    // 쿠폰 발급 여부 판단
    getCouponZ();
    int p = decideUpdateZ(dt);

    // < 파일업데이트 >
    if (p == 1 || p == 2)
        updateCouponZFile(p);
}