#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define MemberFile "Member.txt"
#define GuestFile "Guest.txt"

#include "isValidDataFile.h"
#include "CouponManager.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

static char sTrash[50];
static char cTrash;
static int iTrash;
static int hasMatter;
static double dTrash;

static int pDate;
static int pTime;

typedef struct getInfo
{
    char name[50];
    char id[50];
    char password[50];
    char phoneNumber[50];
    char birth[50];

} getInfo;

static getInfo user; // 전역변수로 선언

static int checkAlpha(char c) // 알파벳(대/소문자) 체크 함수, 대문자 2, 소문자 1, 아니면 0
{
    if (65 <= c && c <= 91) // 대문자
        return 2;
    else if (97 <= c && c <= 122) // 소문자
        return 1;
    return 0;
}

static int checkNum(char c) // 숫자인지 체크 함수, 맞으면 1, 아니면 0
{
    if ((48 <= c && c <= 57))
        return 1;
    return 0;
}

//int checkKor(char* str, int len) // 한글 있는지 체크 함수, 있으면 1, 없으면 0
//{
//    for (int i = 0; i < len; i++)
//    {
//        if (user.name[i] < 0 && user.name[i]>256)
//        {
//            printf("\n한글발견\n");
//            return 1;
//        }
//    }
//    return 0;
//}

// 문법오류 -2, 파일오류 -1, 정상종료 0, q종료 1

static int getName() // 이름 입력 함수
{
    hasMatter = 0;
    int hasAlpha = 0;
    int hasSpace = 0;
    int spaceTime = 0;
    size_t strLength = 0;
    int f, e;

    /*
    <예외처리리스트> +는 기획서에 없는 내용, 즉 기획서에 추가해야 할 내용
    1. 길이가 3자 미만이거나, 30자리 초과인 경우
    2. 입력에 공백과 영어가 아닌 다른 요소 포함
    3. 공백열(1)이 여러개 존재할 경우
    4. 공백열(1)이 존재하지 않을 경우
    5. 공백열(1)을 기준으로 앞과 뒤 둘 중 하나라도 영어가 포함되지 않은 경우
    */

    // <입력>
    printf("\n****************************\n");
    printf("[Please enter name]\n\n\n");
    printf("Enter: ");
    //gets(user.name);////////////////////////////////////////////////////////////////////
    if (scanf_s("%[^\n]s", user.name, sizeof(user.name)) == 0) {
        while (getchar() != '\n');
        printf("The number of characters exceeds or just input Enter.\n");
        return -2;
    }

    while (getchar() != '\n');

    strLength = strlen(user.name);
    //printf("길이: %u\n", strLength);
    //printf("크기: %u\n", sizeof(strLength));

    // <입력된 요소 확인>

    // 알파벳과 공백 개수 확인
    for (int i = 0; i < strLength; i++)
    {
        if (checkAlpha(user.name[i]))
        {
            hasAlpha++;
        }
        else if (user.name[i] == 32 || user.name[i] == 9)
        {
            hasSpace++;
        }
    }

    // 공백열(1) 두 개 이상 있는지 확인
    for (int i = 0, j; i < strLength; i++)
    {
        if (user.name[i] == 32 || user.name[i] == 9)
        {
            if (spaceTime == 0)
                f = i;
            spaceTime++;
            j = i + 1;
            while ((user.name[j] == 32 || user.name[j] == 9) && j < strLength)
            {
                j++;
            }
            e = i = j;
        }
    }


    // < q 탈출 >

    // q 누르면 시간입력으로 탈출
    if (strcmp(user.name, "q") == 0)
    {
        return 1;
    }


    // <문법규칙 검사>

    // 길이가 3 자리 미만이거나 30 자리 초과인 경우
    if (strLength > 30 || strLength < 3)
    {
        printf("The length of ID must be 3 ~ 30\n");
        hasMatter = 1;
    }

    // 입력에 공백과 영어가 아닌 다른 요소가 포함되었을 경우
    if (hasAlpha + hasSpace != strLength)
    {

        printf("Use spacebar and English.\n");
        hasMatter = 1;
    }

    // 공백열(1)이 여러개 존재할 경우
    if (spaceTime > 1)
    {
        printf("Can contain more than one space at a time. But space shouldn’t be seperated\n");
        hasMatter = 1;
    }

    // 공백열(1)이 존재하지 않을 경우
    if (!hasSpace)
    {
        printf("Must contain at least one-space\n");
        hasMatter = 1;
    }

    //공백열(1)을 기준으로 앞과 뒤 둘 중 하나라도 영어가 포함되지 않는 경우 --> 영어가 없는 경우
    if (user.name[0] == 32 || user.name[0] == 9 || user.name[strLength - 1] == 9 || user.name[strLength - 1] == 32)
    {
        printf("Back and forth blank, English must be included\n");
        hasMatter = 1;
    }


    // < 옳은 이름인지 판단 >

    // 위에서 모든 문제 없으면 옳은 이름 -> Member.txt 파일 문법규칙으로 변환
    if (!hasMatter)
    {
        //printf("before name: %s\n", user.name);
        for (int i = f, j = e; i < e; i++, j++)
        {
            user.name[i + 1] = user.name[j];
        }
        //printf("가운데 name: %s\n", user.name);
        for (int i = 0; i < strLength; i++)
        {
            if (user.name[i] == 32 || user.name[i] == 9)
                user.name[i] = 38;
            else if (97 <= user.name[i] && user.name[i] <= 122)
                user.name[i] = user.name[i] - 32;
            else
                user.name[i] = user.name[i];
        }
        //printf("after name: %s\n", user.name);

        return 0;
    }
    // 문제 있으면 옳지 않은 이름 -> 다시 입력받음 -2 반환
    else if (hasMatter)
    {
        printf("Please check it again.\n");
        return -2;
    }
}

static int getId() // 아이디 입력 함수
{
    hasMatter = 0;
    char fId[50];
    int hasAlpha = 0;
    int hasNum = 0;
    size_t strLength = 0;

    /*
    <예외처리리스트> +는 기획서에 없는 내용, 즉 기획서에 추가해야 할 내용
    1. 길이가 4자 미만이거나, 15자 초과
    2. 영어 와 숫자 둘 중 하나라도 포함되지 않았을 경우
    3. 영어나 숫자 외에 다른 문자가 있는 경우
    4. 동치 비교 후 같은 아이디가 이미 존재하는 경우
    */

    // < 입력 >
    printf("\n****************************\n");
    printf("[ Please enter user ID ]\n\n\n");
    printf("Enter: ");
    //gets(user.id);////////////////////////////////////////////////////////////////////
    if (scanf_s("%[^\n]s", user.id, sizeof(user.id)) == 0) {
        while (getchar() != '\n');
        printf("The number of characters exceeds or just input Enter.\n");
        return -2;
    }

    while (getchar() != '\n');
    strLength = strlen(user.id);



    //////////////////////////
    //  < 입력된 요소 확인 >  //
    //////////////////////////

    // 알파벳과 공백 개수 확인
    for (int i = 0; i < strLength; i++)
    {
        if (checkAlpha(user.id[i]))
        {
            hasAlpha++;
        }
        if (checkNum(user.id[i]))
        {
            hasNum++;
        }
    }
    //printf("영어: %d, 숫자: %d\n", hasAlpha, hasNum);


    ///////////////
    // < q 탈출 > //
    ///////////////

    // q 누르면 시간입력으로 탈출
    if (strcmp(user.id, "q") == 0)
    {
        return 1;
    }


    /////////////////////
    // < 문법규칙 검사 > //
    /////////////////////

    // 길이가 4 자리 미만이거나 15 자리 초과인 경우
    if (strLength > 15 || strLength < 4)
    {
        printf("The length of ID must be 4 ~ 15.\n");
        hasMatter = 1;
    }

    // 영어나 숫자 둘 중 하나라도 포함되지 않았을 경우
    if (!hasAlpha || !hasNum)
    {
        printf("At least one number and English alphabet should be included.\n");
        hasMatter = 1;
    }

    // 영어나 숫자 외에 다른 문자가 있는 경우
    if (hasNum + hasAlpha != strLength)
    {
        printf("This ID must be consist of English and number only.\n");
        hasMatter = 1;
    }


    //////////////////
    // < 의미 규칙 > //
    //////////////////

    FILE* fp = 0;
    if (isValidDataFile(MemberFile))
    {
        fp = fopen(MemberFile, "r");
    }

    // 문법규칙에 오류가 없었으면 의미규칙 검사 -> Member.txt로부터 데이터 읽어와 동일한 아이디가 있는지 동치비교
    while (!hasMatter && fscanf(fp, "%s %s %s %s %s %lf", sTrash, fId, sTrash, sTrash, sTrash, &dTrash) != EOF)
    {
        //printf("Id from file: %s\n", fId);
        //printf("sTrash: %s\n", sTrash);
        if (strcmp(user.id, fId) == 0) // 동일한 아이디 찾을 경우
        {
            printf("This ID exists already\n");
            hasMatter = 1;
            break;
        }
    }
    fclose(fp);



    //////////////////////////
    // < 옳은 아이디인지 판단 > //
    //////////////////////////

    // 문제 있으면 옳지 않은 아이디 -> 다시 입력받음 -2 반환
    if (hasMatter)
    {
        printf("Please check it again.\n");
        return -2;
    }
    else if (!hasMatter)
    {
        return 0;
    }

}

static int getPassword() // 비밀번호 입력 함수
{
    hasMatter = 0;
    int hasUpper = 0; // 대문자
    int hasLower = 0; // 소문자
    int hasNum = 0; // 숫자
    int hasSpeChr = 0; // 특수문자
    size_t strLength = 0;
    char speChr[] = { '~', '!', '#', '$', '&', '@' };


    /*
    <예외처리리스트> +는 기획서에 없는 내용, 즉 기획서에 추가해야 할 내용
    1. 길이가 8 자리 미만이거나 15 자리 초과인 경우
    2. 영어, 숫자, 지정한 특수문자 이외의 다른 문자가 존재할 경우
    3. 알파벳 대문자, 알파벳 소문자, 숫자, 지정된 특수문자 중 하나라도 없을 경우
    4.
    */


    // < 입력 >
    printf("\n****************************\n");
    printf("[ Please enter Password ]\n\n\n");
    printf("Enter: ");
    //gets(user.password);//////////////////////////////////////////////////////////////////
    if (scanf_s("%[^\n]s", user.password, sizeof(user.password)) == 0) {
        while (getchar() != '\n');
        printf("The number of characters exceeds or just input Enter.\n");
        return -2;
    }

    while (getchar() != '\n');
    strLength = strlen(user.password);


    //////////////////////////
    //  < 입력된 요소 확인 >  //
    //////////////////////////

    // 대문자, 소문자, 숫자, 특수문자 확인
    for (int i = 0; i < strLength; i++)
    {
        // 대문자 인지 확인
        if (checkAlpha(user.password[i]) == 2)
            hasUpper++;

        // 소문자 인지 확인
        if (checkAlpha(user.password[i]) == 1)
            hasLower++;

        // 숫자인지 확인
        if (checkNum(user.password[i]))
            hasNum++;

        // 특수문자인지 확인
        for (int j = 0; j < sizeof(speChr); j++)
        {
            if (user.password[i] == speChr[j])
                hasSpeChr++;
        }
    }
    //printf("upper case: %d lower case: %d 숫자: %d 특수문자: %d\n", hasUpper, hasLower, hasNum, hasSpeChr);



    // < q 탈출 > //
    // q 누르면 시간입력으로 탈출
    if (strcmp(user.password, "q") == 0)
    {
        return 1;
    }



    // < 문법규칙 검사 >
    // 길이가 8 자리 미만이거나 15 자리 초과인 경우
    if (strLength > 15 || strLength < 8)
    {
        printf("The length of Password must be 8 ~ 15.\n");
        hasMatter = 1;
    }

    // 영어, 숫자, 지정한 특수문자 이외의 다른 문자가 존재할 경우
    if (hasUpper + hasLower + hasNum + hasSpeChr != strLength)
    {
        printf(" It must contain only alphabets, numbers and Special character(~, !, #, $, &, @)\n");
        hasMatter = 1;
    }

    // 알파벳 대문자, 알파벳 소문자, 숫자, 지정된 특수문자 중 하나라도 없을 경우
    if (!hasUpper || !hasLower || !hasSpeChr || !hasNum)
    {
        printf("Capital letters, Small letters, numbers, and Special characters must be contained at least one.\n");
        hasMatter = 1;
    }


    // < 옳은 비밀번호인지 판단 > //
    // 문제 있으면 옳지 않은 비밀번호 -> 다시 입력받음 -2 반환
    if (hasMatter)
    {
        printf("Please check it again.\n");
        return -2;
    }
    else if (!hasMatter)
    {
        return 0;
    }
}

int hadEverNonMembership = 0; // 비회원으로 진행한적이 있는지
char nonMembershipBirth[50]; // 있다면 여기에 그 비회원의 생일 저장
static int getPhoneNumber() // 전화번호 입력 함수
{
    hasMatter = 0;
    char fphoneNumber[50];
    int countNum = 0;
    int dashNum = 0;
    size_t strLength = 0;

    // < 입력 > 
    printf("\n****************************\n");
    printf("[ Please enter Phone Number ]\n\n\n");
    printf("Enter: ");
    //gets(user.phoneNumber);//////////////////////////////////////////////////////////
    if (scanf_s("%[^\n]s", user.phoneNumber, sizeof(user.phoneNumber)) == 0) {
        while (getchar() != '\n');
        printf("The number of characters exceeds or just input Enter.\n");
        return -2;
    }

    while (getchar() != '\n');
    strLength = strlen(user.phoneNumber);
    // printf("문자열 길이: %d\n", strLength);

     /*
     <예외처리리스트> (+) 표시는 기획서에 없는 내용, 즉 기획서에 추가해야 할 내용
     1. (+) 입력이 30자리 초과인 경우/ O
     2. 숫자가 8 개 혹은 11 개가 아닐 경우
     3. 숫자 혹은 ‘-’가 아닌 다른 문자들이 포함되었을 경우
     4. (+) 숫자가 8개이고 '-'가 있을때 <숫자 4자><'-'열(0)><숫자 4자>가 아닐 경우
     5. (+) 숫자가 11개이고 '-'가 있을 때 <010><'-'열(0)><숫자 4자><'-'열(0)><숫자 4자>가 아닐 경우
     6. (+) 숫자가 11개이고 '-'가 없을 때, 010으로 시작하지 않을 경우 검사(기획서 추가해야됨)->5번과 사실상 같은 내용
     */


     // < 입력 요소 확인 >
    for (int i = 0; i < strLength; i++)
    {
        if (checkNum(user.phoneNumber[i]))
        {
            countNum++;
        }
        if (user.phoneNumber[i] == '-')
        {
            dashNum++;
        }
    }
    //printf("CountNum %d\n", countNum);
    //printf("dashtNum %d\n", dashNum);


    // q 탈출 
    if (strcmp(user.phoneNumber, "q") == 0)
    {
        return 1;
    }


    // 입력 길이 제한 30자 초과 검사 기획서에 추가해야됨 (길이제한(입력제한)이 기획서에는 없음)
    if (strLength > 30)
    {
        printf("Input must be at most 30\n");
        hasMatter = 1;
    }


    //////////////////
    // < 문법 규칙 >

     // 숫자가 8 개 혹은 11 개가 아닐 경우 검사
    if (countNum != 8 && countNum != 11)
    {
        printf("Only 8 or 11 number is allowed\n");

        hasMatter = 1;
    }

    // 숫자 혹은 ‘-’가 아닌 다른 문자들이 포함되었을 경우 검사
    if (countNum + dashNum != strLength)
    {
        printf("It is not allowed element\n");
        hasMatter = 1;
    }

    // 숫자가 8개 이고 '-'가 포함되어있을때 <숫자 4자><'-'열(0)><숫자 4자>가 아닐 경우 검사(기획서 추가해야됨)
    if (countNum == 8 && dashNum && dashNum + countNum == strLength)
    {
        for (int i = 4; i < strlen(user.phoneNumber) - 4; i++)
        {
            if (user.phoneNumber[i] != '-')
            {

                printf("Invalid form, must be in the form '4 digits - 4 digits'\n");
                hasMatter = 1;
                break;
            }
        }
    }

    // 숫자가 11개 이고 '-' 가 있을 때 <010><'-'열(0)><숫자 4자><'-'열(0)><숫자 4자>가 아닐 경우 검사(기획서 추가해야됨)
    if (countNum == 11 && dashNum && dashNum + countNum == strLength)
    {
        int isWrongNum = 0;
        int checkTwo = 0;

        // 010으로 시작하면
        if (strncmp(user.phoneNumber, "010", 3) == 0)
        {
            int i = 3;

            // 010 바로 뒤에 붙은 '-' 갯수 카운팅 -> 0101234-1223 과 같은 틀린 입력 잡기 위해
            while (user.phoneNumber[i] == '-')
                i++;

            // 010 바로 뒤에 '-'가 하나라도 있고 그 바로 뒤 숫자가 4자인지 확인
            if (i != 3 && checkNum(user.phoneNumber[i]) && checkNum(user.phoneNumber[i + 1]) && checkNum(user.phoneNumber[i + 2]) && checkNum(user.phoneNumber[i + 3]) && user.phoneNumber[i + 4] == '-')
                checkTwo++;

            // 가장 마지막 4자리가 숫자 4자인지 확인
            if (checkNum(user.phoneNumber[strLength - 1]) && checkNum(user.phoneNumber[strLength - 2]) && checkNum(user.phoneNumber[strLength - 3]) && checkNum(user.phoneNumber[strLength - 4]) && user.phoneNumber[strLength - 5] == '-')
                checkTwo++;

            if (checkTwo != 2 || user.phoneNumber[strLength - 1] == '-')
                isWrongNum = 1;
        }
        // 010으로 시작안하면
        else
        {
            isWrongNum = 1;
        }

        if (isWrongNum == 1)
        {
            printf("Invalid form, must be in the form '010 - 4 digits - 4 digits'\n");
            hasMatter = 1;
        }
    }

    // 숫자가 11개 이고, '-'가 없을 때, 010으로 시작하지 않을 경우 검사(기획서 추가해야됨)
    if (countNum == 11 && !dashNum && countNum == strLength)
    {
        if (strncmp(user.phoneNumber, "010", 3) != 0) // 010으로 시작하지 않음
        {
            printf("Invalid form, must be in the form '010 4 digits 4 digits'\n");
            hasMatter = 1;
        }
    }

    //printf("user: %s\n", user.phoneNumber);


    ////////////////
    // <의미 규칙>

    // 전화번호를 비교하기 위해 사용자로부터 입력받은 전화번호(user.phoneNumber)를 <010><숫자4자><숫자4자>형식으로 변환해서
    // nonDashNum에 저장
    char nonDashNum[50] = "010";
    int n = 0;
    if (!hasMatter) // 위에서 문법 규칙 검사 후 이상이 없을 경우 의미규칙 검사
    {
        n = 3;
        if (countNum == 8) // 사용자로부터 입력받은 숫자가 8자일 경우
        {
            for (int i = 0; i < strLength; i++)
            {
                if (checkNum(user.phoneNumber[i]))
                {
                    nonDashNum[n++] = user.phoneNumber[i];
                }
            }
        }
        else if (countNum == 11) // 사용자로부터 입력받은 숫자가 11자일 경우
        {
            for (int i = 3; i < strLength; i++)
            {
                if (checkNum(user.phoneNumber[i]))
                {
                    nonDashNum[n++] = user.phoneNumber[i];
                }
            }
        }
        nonDashNum[n] = '\0'; // nonDashNum에 형식 변환된 번호 저장
        //printf("nondash: %s\n", nonDashNum);
    }


    FILE* fp = 0;
    if (isValidDataFile(MemberFile))
    {
        fp = fopen(MemberFile, "r");
    }

    //Member.txt 파일로부터 한줄씩 동일한 전화번호가 있는지 검사
    while (hasMatter == 0 && fscanf(fp, "%s %s %s %s %s %lf", sTrash, sTrash, sTrash, sTrash, fphoneNumber, &dTrash) != EOF)
    {
        char fnonDashNum[50]; // 전화번호를 비교하기 위해 Member.txt에서 입력받은 fphoneNumber로부터 '-'를 모두 제거
        n = 0;
        for (int i = 0; i < strlen(fphoneNumber); i++) // '-'을 모두 제거한 후 fnonDashNum에 초기화
        {
            if (checkNum(fphoneNumber[i]))
            {
                fnonDashNum[n++] = fphoneNumber[i];
            }
        }
        fnonDashNum[n] = '\0';

        // nonDashNum과 fnonDashNum 이 같은 문자열인지(동일한 전화번호인지) 검사
        if (strcmp(fnonDashNum, nonDashNum) == 0)
        {
            //printf("사용자로부터 입력받은 번호: %s\n", nonDashNum);
            //printf("파일로부터 입력받은 번호: %s\n", fnonDashNum);
            hasMatter = 1;
            printf("This Phone number exists already\n");
        }
    }
    fclose(fp);

    if (isValidDataFile(GuestFile))
    {
        fp = fopen(GuestFile, "r");
    }

    // Guest.txt 파일로부터 한줄씩 동일한 전화번호가 있는지 검사 -> 비회원으로 진행 여부 기록 확인하기 위해
    while (hasMatter == 0 && fscanf(fp, "%s %s %s", fphoneNumber, nonMembershipBirth, sTrash) != EOF)
    {
        char fnonDashNum[50]; // 전화번호를 비교하기 위해 Guest.txt 파일에서 입력받은 fphoneNumber로부터 '-'를 모두 제거
        n = 0;
        for (int i = 0; i < strlen(fphoneNumber); i++) // Guest.txt 파일로부터 입력받은 전화번호에서 '-'을 모두 제거한 후 fnonDashNum에 초기화
        {
            if (checkNum(fphoneNumber[i]))
            {
                fnonDashNum[n++] = fphoneNumber[i];
            }
        }
        fnonDashNum[n] = '\0';

        // 만약 동일한 번호가 있으면 해당 유저의 생년월일 저장
        if (strcmp(fnonDashNum, nonDashNum) == 0)
        {
            //printf("사용자로부터 입력받은 번호: %s\n", nonDashNum);
            //printf("파일로부터 입력받은 번호: %s\n", fnonDashNum);
            //printf("비회원으로 진행한적이 있음\n");
            //printf("%s\n", nonMembershipBirth);
            hadEverNonMembership = 1;
            break;
        }
    }
    fclose(fp);
    strcpy(user.phoneNumber, nonDashNum);

    //printf("before: %s\n", user.phoneNumber);
    n = 10;

    // Member.txt 파일에 저장하기 위해 <010><'-'><숫자 4자><'-'><숫자 4자>로 형식 변환
    for (int i = 12; i > 0; i--)
    {
        if (i == 3 || i == 8)
            user.phoneNumber[i] = '-';
        else
            user.phoneNumber[i] = user.phoneNumber[n--];
    }
    user.phoneNumber[13] = '\0';

    //printf("after: %s\n", user.phoneNumber);


    // < 옳은 전화번호인지 판단 > //
    // 문제 있으면 옳지 않은 전화번호 -> 다시 입력받음 -2 반환
    if (hasMatter)
    {
        printf("Please check it again.\n");
        return -2;
    }
    else if (!hasMatter)
    {
        return 0;
    }

}

static int getBirth() // 생년월일 입력함수
{
    hasMatter = 0;
    size_t strLength = 0;
    int birthInt = 0;


    // < 입력 >
    printf("\n****************************\n");
    printf("[ Please enter Birth ] \n\n\n");
    printf("Enter: ");
    //gets(user.birth);///////////////////////////////////////////////////////////////
    if (scanf_s("%[^\n]s", user.birth, sizeof(user.birth)) == 0) {
        while (getchar() != '\n');
        printf("The number of characters exceeds or just input Enter.\n");
        return -2;
    }

    while (getchar() != '\n');
    strLength = strlen(user.birth);


    // < q 탈출 >
    if (strcmp(user.birth, "q") == 0)
    {
        return 1;
    }


    //////////////////
    // < 문법 규칙 검사 >
    // 
    // 길이가 6 혹은 8이 아닐 경우
    if (strLength != 6 && strLength != 8)
    {
        //printf("It must contain only 6 numbers and 8 numbers\n");
        hasMatter = 1;
    }
    // 숫자가 아닌 다른 요소가 포함되어 있는지 확인
    for (int i = 0; i < strLength; i++)
    {
        if (!checkNum(user.birth[i]))
        {
            //printf("It must contain only 6 numbers and 8 numbers\n");
            hasMatter = 1;
            break;
        }
    }

    if (hasMatter)
        printf("It must contain only 6 numbers and 8 numbers\n");


    /// <의미규칙 검사>

    birthInt = atoi(user.birth); // 그래고리력 검사하기 위해 정수형으로 변환


    if (!hasMatter)
    {
        int birthIntCopy = birthInt;
        int year = birthIntCopy / 10000;
        birthIntCopy %= 10000;
        int month = birthIntCopy / 100;
        birthIntCopy %= 100;


        // 그레고리안력 따르는지 검사
        if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
        {
            if (birthIntCopy > 31 || birthIntCopy == 0)
            {
                printf("Follow Gregorian Calendar\n");
                hasMatter = 1;
            }
        }
        else if (month == 2)
        {
            int l = 28;
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
                l = 29;

            if (birthIntCopy > l || birthIntCopy == 0)
            {
                printf("Follow Gregorian Calendar\n");
                hasMatter = 1;
            }
        }
        else if (month == 4 || month == 6 || month == 9 || month == 11)
        {
            if (birthIntCopy > 30 || birthIntCopy == 0)
            {
                printf("Follow Gregorian Calendar\n");
                hasMatter = 1;
            }
        }
        else
        {
            printf("Follow Gregorian Calendar\n");
            hasMatter = 1;
        }
    }

    if (!hasMatter && (pDate >= 20000000) && strLength == 6) // 현재가 21세기이고 입력받은 수 6자리일때
    {
        if ((birthInt + 20000000) <= pDate) // 생일을 정할 수 없을 때
        {
            printf("It can’t determine whether the Birth is 1900s or 2000s.\n");
            hasMatter = 1;
        }
        else // 정할 수 있을 때 (20세기 사람)
        {
            for (int i = 5; i >= 0; i--)
            {
                user.birth[i + 2] = user.birth[i];
            }
            user.birth[8] = '\0';
            user.birth[0] = '1';
            user.birth[1] = '9';
        }
        birthInt = atoi(user.birth);
    }
    else if (!hasMatter && (pDate < 20000000) && strLength == 6) // 현재가 20세기이고 입력받은 수 6자리일때
    {
        for (int i = 5; i >= 0; i--)
        {
            user.birth[i + 2] = user.birth[i];
        }
        user.birth[8] = '\0';
        user.birth[0] = '1';
        user.birth[1] = '9';
        birthInt = atoi(user.birth);
    } // 정수형으로 변환 끝

    //printf("정수로 저장된 생일: %d\n", birthInt);

    // 생일이 미래일때
    if (!hasMatter && (birthInt > pDate))
    {
        printf("The Birth cannot be future than input date.\n");
        hasMatter = 1;
    }
    // 생일이 1900년도 보다 이전 일때
    if (!hasMatter && (birthInt < 19000000))
    {
        printf("Birth of year must be between 1900~now.\n");
        hasMatter = 1;
    }

    if (hadEverNonMembership && !hasMatter) // 비회원으로 진행한적이 있는지
    {
        if (strcmp(user.birth, nonMembershipBirth) == 0) // 비회원으로 진행했을 당시의 생일과 같을 경우 문제없음
            hasMatter = 0;
        else
        {
            hasMatter = 1;
            printf("It is not correspond with Birth when you had joined Non - Membership.\n");
        }
    }

    // < 옳은 생년월일인지 판단 > //
    // 문제 있으면 옳지 않은 생년월일 -> 다시 입력받음 -2 반환
    if (hasMatter)
    {
        printf("Please check it again.\n");
        // printf("잘못됨\n");
        return -2;
    }
    else if (!hasMatter)
    {
        // printf("옳게됨\n");
        return 0;
    }
}


//파일오류 -1, 정상종료 0

int main_Join(struct DateTime* dt) // 회원가입
{
    // 입력으로 엄~~청 많이 들어오면 런타임에러 -> 어떻게 해결?
    int name;
    int id;
    int ps;
    int pn;
    int bd;

    pDate = dt->year * 10000 + dt->month * 100 + dt->day;
    pTime = dt->hour * 100 + dt->minute;

    FILE* fp = 0;
    //fp = fopen(MemberFile, "r");
    //if (!fp)
    //{
    //    printf("파일 없음\n");
    //    return -1;
    //}
    //fclose(fp);
    //fp = fopen(GuestFile, "r");
    //if (!fp)
    //{
    //    printf("파일 없음\n");
    //    return -1;
    //}
    //fclose(fp);

    //while (1)
    //{
    //    int a;
    //    //a = getName();
    //    //a = getId();
    //    //a = getPassword();
    //    //a = getPhoneNumber();
    //    a = getBirth();

    //    // 문법오류 -2, 파일오류 -1, 정상종료 0, q종료 1
    //    if (a == 1)
    //    {
    //        printf("q입력 종료\n");

    //    }
    //    else if (a == -2)
    //    {
    //        printf("규칙 오류\n");
    //    }
    //    else if (a == 0)
    //    {
    //        printf("정상종료\n");
    //    }
    //    else if (a == -1)
    //    {
    //        printf("파일 오류\n");
    //    }
    //    else
    //    {
    //        printf("알수업는 오류\n");
    //    }
    //}

    while ((name = getName()) == -2)
    {
    }
    if (name == -1)
        return -1;
    else if (name == 1)
        return 0;

    while ((id = getId()) == -2)
    {
    }
    if (id == -1)
        return -1;
    else if (id == 1)
        return 0;

    while ((ps = getPassword()) == -2)
    {
    }
    if (ps == -1)
        return -1;
    else if (ps == 1)
        return 0;

    while ((pn = getPhoneNumber()) == -2)
    {
    }
    if (pn == -1)
        return -1;
    else if (pn == 1)
        return 0;

    while ((bd = getBirth()) == -2)
    {
    }
    if (bd == -1)
        return -1;
    else if (bd == 1)
        return 0;



    double point = 0.0;
    //printf("%s %s %s %s %s %d\n", user.name, user.id, user.password, user.birth, user.phoneNumber, point);

    isValidDataFile(MemberFile);
    isValidDataFile("LogData.csv");
    fp = fopen(MemberFile, "a");
    fprintf(fp, "%s %s %s %s %s %f\n", user.name, user.id, user.password, user.birth, user.phoneNumber, point);
    fclose(fp);

    fp = fopen("LogData.csv", "a");
    fprintf(fp, "[Register],%d,%d,%s\n", pDate, pTime, user.phoneNumber);
    fclose(fp);



    printf("\n****************************\n\n");
    printf("**Successfully signed up for membership!**\n\n");
    printf("****************************\n");

    struct Coupon xc;
    Coupon(&xc, user.phoneNumber, dt);

    return 0;
}