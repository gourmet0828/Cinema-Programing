#pragma once
#include <stdio.h>
//#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
//#include "dateTime.h"

bool FileExist(const char* filename)
{
    DWORD dwAtt = GetFileAttributesA(filename);
    return (dwAtt != INVALID_FILE_ATTRIBUTES && !(dwAtt & FILE_ATTRIBUTE_DIRECTORY));
}
bool canReadFile(const char* filename) {
    DWORD dwAttrib = GetFileAttributesA(filename);
    return (FileExist(filename) && !(dwAttrib & FILE_ATTRIBUTE_READONLY));
}
//bool canWriteFile(const char* filename) {
//    DWORD dwAttrib = GetFileAttributesA(filename);
//    return (FileExist(filename) && (dwAttrib & FILE_ATTRIBUTE_READONLY) == 0);
//}

bool canWriteFile(const char* filename)
{
    FILE* fp = NULL;
    fp = fopen(filename, "r");
    if (!fp)
        return false; // 접근 불가할때
    else // 접근 가능할때
    {
        fclose(fp);
        return true;
    }
}

bool isValidDataFile(const char* filename)
{
    if (!FileExist(filename))
    {
        printf("Error: DataFile not found.\n");
        printf("Terminate the program.\n");
        exit(0);
        // critircal error
    }
    if (!(canReadFile(filename) && canWriteFile(filename)))
    {
        printf("Error: Cannot access to DataFile.\n");
        printf("Terminate the program.\n");
        exit(0);
        // critircal error
    }
    /*if (!canAccess(filename))
    {
        printf("Error: Cannot access to DataFile.\n");
        printf("Terminate the program123.\n");
        exit(0);
    }*/
    return true;
}

void setResvAndTheat()
{
    int date, time;
    char c;
    char strdateAfter[50];
    FILE* fp = fopen("DateTime.csv", "r");
    if (fp)
    {
        fscanf(fp, "%s", strdateAfter);
        fclose(fp);
        strdateAfter[8] = '\0';
    }
    else
    {
        strcpy(strdateAfter, "19500101");
    }
    //sprintf(strdateAfter, "%d", date);


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
    fclose(bfile);
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
    fclose(cfile);
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

    fp = fopen("Reservation.csv", "w");
    fclose(fp);

    fclose(Afile);
    fclose(Bfile);
    fclose(Cfile);

}

int checkFiles()
{
    int hasErr = 0;
    int init = 0;
    FILE* fp;
    if (!FileExist("DateTime.csv"))
    {
        fp = fopen("DateTime.csv", "w"); // 시간 파일 초기화
        fprintf(fp, "19500101,0000");
        hasErr = 1;
        init = 1;
        fclose(fp);
        fp = fopen("TheaterA.csv", "w");
        fclose(fp);
        fp = fopen("TheaterB.csv", "w");
        fclose(fp);
        fp = fopen("TheaterC.csv", "w");
        fclose(fp);
        fp = fopen("Reservation.csv", "w");
        fclose(fp);
        fp = fopen("Guest.txt", "w");
        fclose(fp);
        fp = fopen("Member.txt", "w");
        fclose(fp);
        fp = fopen("CouponX.txt", "w");
        fclose(fp);
        fp = fopen("CouponY.txt", "w");
        fclose(fp);
        fp = fopen("CoupomZ.txt", "w");
        fclose(fp);
    }
    if (!FileExist("Member.txt") || !FileExist("CouponX.txt") || !FileExist("CouponY.txt") || !FileExist("CouponZ.txt"))
    {
        fp = fopen("Member.txt", "w");
        fclose(fp);
        fp = fopen("CouponX.txt", "w");
        fclose(fp);
        fp = fopen("CouponY.txt", "w");
        fclose(fp);
        fp = fopen("CouponZ.txt", "w");
        fclose(fp);
        hasErr = 1;
    }
    if (!FileExist("Guest.txt"))
    {
        fp = fopen("Guest.txt", "a");
        hasErr = 1;
        fclose(fp);
    }
    if (!FileExist("TheaterA.csv") || !FileExist("TheaterB.csv") || !FileExist("TheaterC.csv") || !FileExist("Reservation.csv"))
    {
        hasErr = 1;
        init = 1;
        fp = fopen("TheaterA.csv", "w");
        fclose(fp);
        fp = fopen("TheaterB.csv", "w");
        fclose(fp);
        fp = fopen("TheaterC.csv", "w");
        fclose(fp);
        fp = fopen("Reservation.csv", "w");
        fclose(fp);
    }
    if (!FileExist("LogData.csv"))
    {
        fp = fopen("LogData.csv", "a");
        hasErr = 1;
        fclose(fp);
    }

    if (init)
    {
        setResvAndTheat();
    }

    if (hasErr)
        printf("Error. DataFile Not Found. Create missed file and initialize existing file.\n");

    if (!(canReadFile("DateTime.csv") && canWriteFile("DateTime.csv")))
    {
        printf("Error: Cannot access to DataFile.\n");
        //printf("1\n");
        printf("Terminate the program.\n");
        exit(0);
    }
    if (!(canReadFile("Member.txt") && canWriteFile("Member.txt")))
    {
        printf("Error: Cannot access to DataFile.\n");
        //printf("1\n");
        printf("Terminate the program.\n");
        exit(0);
    }
    if (!(canReadFile("Guest.txt") && canWriteFile("Guest.txt")))
    {
        printf("Error: Cannot access to DataFile.\n");
        //printf("1\n");
        printf("Terminate the program.\n");
        exit(0);
    }
    if (!(canReadFile("TheaterA.csv") && canWriteFile("TheaterA.csv")))
    {
        printf("Error: Cannot access to DataFile.\n");
        //printf("1\n");
        printf("Terminate the program.\n");
        exit(0);
    }
    if (!(canReadFile("TheaterB.csv") && canWriteFile("TheaterB.csv")))
    {
        printf("Error: Cannot access to DataFile.\n");
        printf("Terminate the program.\n");
        exit(0);
    }
    if (!(canReadFile("TheaterC.csv") && canWriteFile("TheaterC.csv")))
    {
        printf("Error: Cannot access to DataFile.\n");
        printf("Terminate the program.\n");
        exit(0);
    }
    if (!(canReadFile("Reservation.csv") && canWriteFile("Reservation.csv")))
    {
        printf("Error: Cannot access to DataFile.\n");
        printf("Terminate the program.\n");
        exit(0);
    }
    if (!(canReadFile("LogData.csv") && canWriteFile("LogData.csv")))
    {
        printf("Error: Cannot access to DataFile.\n");
        //printf("12\n");
        printf("Terminate the program.\n");
        exit(0);
    }
    if (!(canReadFile("CouponX.txt") && canWriteFile("CouponX.txt")))
    {
        printf("Error: Cannot access to DataFile.\n");
        //printf("12\n");
        printf("Terminate the program.\n");
        exit(0);
    }
    if (!(canReadFile("CouponY.txt") && canWriteFile("CouponY.txt")))
    {
        printf("Error: Cannot access to DataFile.\n");
        //printf("12\n");
        printf("Terminate the program.\n");
        exit(0);
    }
    if (!(canReadFile("CouponZ.txt") && canWriteFile("CouponZ.txt")))
    {
        printf("Error: Cannot access to DataFile.\n");
        //printf("12\n");
        printf("Terminate the program.\n");
        exit(0);
    }

    return 0;
}