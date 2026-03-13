// CouponManager.h
#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "join.h"
#include "dateTime.h"
#include "isValidDataFile.h"

struct Coupon
{
    char cSymbol[13];           // 쿠폰번호
    char phoneNumber[14];       // 전화번호
    char registrationDate[9];   // 등록일
    char expirationDate[9];       // 만료일
    int status;                // 사용여부 (0: 사용안함, 1: 사용함, 2: 만료)
};


// constructor
void Coupon(struct Coupon* Coupon, char* phoneNumber, struct DateTime* dt)
{
    // Initialize cSymbol
    Coupon->cSymbol[0] = 'X';
    Coupon->cSymbol[1] = '1';
    for (int i = 2; i < 12; i++)
    {
        Coupon->cSymbol[i] = phoneNumber[i + 1];

    }
    Coupon->cSymbol[12] = '\0';

    // Initialize phoneNumber
    strncpy(Coupon->phoneNumber, phoneNumber, 13);
    Coupon->phoneNumber[13] = '\0';

    // Initialize registrationDate
    snprintf(Coupon->registrationDate, sizeof(Coupon->registrationDate), "%04d%02d%02d", dt->year, dt->month, dt->day);
    Coupon->registrationDate[8] = '\0';

    // Calculate expirationDate
    int year = dt->year;
    int month = dt->month;
    int day = dt->day + 7;
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
    }

    while (day > daysinMonth)
    {
        day -= daysinMonth;
        month++;
        if (month > 12)
        {
            month = 1;
            year++;
        }
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
        }
    }
    snprintf(Coupon->expirationDate, sizeof(Coupon->expirationDate), "%04d%02d%02d", year, month, day);
    Coupon->expirationDate[8] = '\0';

    // Initialize benefit, status
    char benefit[] = "W2000";
    char record[100];
    snprintf(record, sizeof(record), "%s %s %s %s %s 0\n",
        Coupon->cSymbol, Coupon->phoneNumber, Coupon->registrationDate,
        Coupon->expirationDate, benefit);

    // 회원가입 쿠폰
    isValidDataFile("CouponX.txt");
    FILE* fp1 = fopen("CouponX.txt", "a");
    fputs(record, fp1);
    fclose(fp1);

    // 생일 쿠폰
    FILE* fp2 = fopen("CouponZ.txt", "a");

    Coupon->cSymbol[0] = 'Z';
    Coupon->cSymbol[1] = '1';

    for (int i = 0; i < 100; i++)
    {
        record[i] = '\0';
    }
    snprintf(record, sizeof(record), "%s %s 00000000 00000000 W5000 2\n",
        Coupon->cSymbol, Coupon->phoneNumber);
    fputs(record, fp2);
    fclose(fp2);

    FILE* fp3 = fopen("CouponY.txt", "a");

    Coupon->cSymbol[0] = 'Y';
    Coupon->cSymbol[1] = '1';
    for (int i = 0; i < 100; i++)
    {
        record[i] = '\0';
    }
    snprintf(record, sizeof(record), "%s %s %d0101 %d0630 %%5 2\n",
        Coupon->cSymbol, Coupon->phoneNumber, year, year);
    fputs(record, fp3);

    Coupon->cSymbol[0] = 'Y';
    Coupon->cSymbol[1] = '2';
    for (int i = 0; i < 100; i++)
    {
        record[i] = '\0';
    }
    snprintf(record, sizeof(record), "%s %s %d0401 %d0930 %%5 2\n",
        Coupon->cSymbol, Coupon->phoneNumber, year, year);
    fputs(record, fp3);

    Coupon->cSymbol[0] = 'Y';
    Coupon->cSymbol[1] = '3';
    for (int i = 0; i < 100; i++)
    {
        record[i] = '\0';
    }
    snprintf(record, sizeof(record), "%s %s %d0701 %d1231 %%5 2\n",
        Coupon->cSymbol, Coupon->phoneNumber, year, year);
    fputs(record, fp3);

    Coupon->cSymbol[0] = 'Y';
    Coupon->cSymbol[1] = '4';
    for (int i = 0; i < 100; i++)
    {
        record[i] = '\0';
    }
    snprintf(record, sizeof(record), "%s %s %d1001 %d0331 %%5 2\n",
        Coupon->cSymbol, Coupon->phoneNumber, year, year + 1);
    fputs(record, fp3);


    fclose(fp3);
}
// function implementation
void updateCouponStatus(struct Coupon* Coupon, char* cSymbol, int status)
{
    FILE* fp = fopen("CouponX.txt", "r+");  // r+ : read and write

    char buffer[53];    // record size: 52
    long pos;           // ftell() return type is long

    // Find Coupon line number (pos) with cSymbol
    // Update Coupon Usage
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        if (strncmp(buffer, cSymbol, strlen(cSymbol)) == 0)
        {
            pos = ftell(fp) - (long)strlen(buffer);
            fseek(fp, pos + 51, SEEK_SET);  // 51: index of used
            fprintf(fp, "%1d", status);
            break;
        }
    }
    fclose(fp);
}
void updateAllCouponStatus(struct Coupon* Coupon, struct DateTime* dt)
{
    char cSymbols[1000][13];
    char expirationDates[1000][9];
    char status[1000][2];
    FILE* fp = fopen("CouponX.txt", "r");
    int lines = lineNum(fp);

    for (int i = 0; i < lines; i++)
    {
        status[i][1] = '\0';
        if (fscanf(fp, "%12s %*s %*s %8s %*s %s", cSymbols[i], expirationDates[i], status[i]) != 3)
        {
            fclose(fp);
            return;
        }
    }

    int year = dt->year;
    int month = dt->month;
    int day = dt->day;

    int intCurrentDate = year * 10000 + month * 100 + day;

    for (int i = 0; i < lines; i++)
    {
        int intExpirationDate = atoi(expirationDates[i]);
        if ((intExpirationDate < intCurrentDate) && (strcmp(status[i], "0") == 0))
        {
            updateCouponStatus(Coupon, cSymbols[i], 2);
        }
    }
}
// helper funtion
int lineNum(FILE* fp) // for updateAllCouponUsage 
{
    int num = 0;
    char ch;
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n')
        {
            num++;
        }
    }
    rewind(fp);
    return num;
}