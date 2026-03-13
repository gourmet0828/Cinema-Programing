#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "dateTime.h"
#include "isValidDataFile.h"
// 문자열이 공백이나 탭만으로 이루어져 있는지 검사하는 함수
int isOnlyWhitespace(const char* str) {
    while (*str != '\0') {
        if (!isspace((unsigned char)*str))
            return 0;
        str++;
    }
    return 1;
}
int check;

// 입력 버퍼를 비우는 함수 추가
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}


void formatPhoneNumber(const char* input, char* formattedPhoneNumber) {
    int i, j = 0, digitCount = 0;
    char digits[12] = { 0 }; // 입력된 숫자를 저장할 배열

    // 입력 문자열에서 숫자만 추출
    for (i = 0; input[i] != '\0' && digitCount < 12; ++i) {
        if (isdigit(input[i])) {
            digits[digitCount++] = input[i];
        }
    }

    // 추출된 숫자의 개수에 따라 올바른 형식으로 변환
    if (digitCount == 8) { // 8자리 숫자인 경우 (xxxx-yyyy 형식)
        strcpy(formattedPhoneNumber, "010-");
        j = 4;
        for (i = 0; i < 8; ++i) {
            if (i == 4) {
                formattedPhoneNumber[j++] = '-'; // 4자리 후에 '-' 추가
            }
            formattedPhoneNumber[j++] = digits[i];
        }
    }
    else if (digitCount == 11) { // 11자리 숫자인 경우 (010-xxxx-yyyy 형식)
        for (i = 0; i < 11; ++i) {
            if (i == 3 || i == 7) {
                formattedPhoneNumber[j++] = '-'; // 3자리와 7자리 후에 '-' 추가
            }
            formattedPhoneNumber[j++] = digits[i];
        }
    }
    else {
        // 올바른 숫자 개수가 아닌 경우, 오류 메시지
        strcpy(formattedPhoneNumber, "Invalid input");
        return;
    }

    formattedPhoneNumber[j] = '\0'; // 문자열 종료
}


// 전화번호 형식 검사 함수
int isValidFormat(char* phoneNumber) {
    int i = 0;
    int digitCountBeforeHyphen = 0;
    int digitCountAfterHyphen = 0;
    int digitCountMiddleHypen = 0;
    int hyphenCount1 = 0;
    int hyphenCount2 = 0;
    int isStartWith010 = 0;
    int digitCount = 0;

    // 전화번호 앞부분의 숫자 개수 세기



    if (phoneNumber[0] == '0' && phoneNumber[1] == '1' && phoneNumber[2] == '0') {
        if (phoneNumber[3] >= '0' && phoneNumber[3] <= '9') {
            while (phoneNumber[i] != '\0' && phoneNumber[i] != '-') {
                if (phoneNumber[i] >= '0' && phoneNumber[i] <= '9') {
                    digitCountMiddleHypen++;
                }
                else if (phoneNumber[i] == ' ' || phoneNumber[i] == '\t') {
                    return 0;
                }
                else {
                    return 0; // 숫자가 아닌 문자가 있으면 형식 불만족
                }
                i++;
            }

            // 하이픈 개수 세기
            while (phoneNumber[i] == '-') {
                hyphenCount2++;
                i++;
            }

            // 전화번호 뒷부분의 숫자 개수 세기
            while (phoneNumber[i] != '\0') {
                if (phoneNumber[i] >= '0' && phoneNumber[i] <= '9') {
                    digitCountAfterHyphen++;
                }
                else if (phoneNumber[i] == ' ' || phoneNumber[i] == '\t') {
                    return 0;
                }
                else {
                    return 0; // 숫자가 아닌 문자가 있으면 형식 불만족
                }
                i++;
            }
        }
        else if (phoneNumber[3] == '-') {
            digitCountBeforeHyphen = 3;
            i = 3;
            while (phoneNumber[i] == '-') {
                hyphenCount1++;
                i++;
            }

            while (phoneNumber[i] != '\0' && phoneNumber[i] != '-') {
                if (phoneNumber[i] >= '0' && phoneNumber[i] <= '9') {
                    digitCountMiddleHypen++;
                }
                else if (phoneNumber[i] == ' ' || phoneNumber[i] == '\t') {
                    return 0;
                }
                else {
                    return 0; // 숫자가 아닌 문자가 있으면 형식 불만족
                }
                i++;
            }

            // 하이픈 개수 세기
            while (phoneNumber[i] == '-') {
                hyphenCount2++;
                i++;
            }

            // 전화번호 뒷부분의 숫자 개수 세기
            while (phoneNumber[i] != '\0') {
                if (phoneNumber[i] >= '0' && phoneNumber[i] <= '9') {
                    digitCountAfterHyphen++;
                }
                else if (phoneNumber[i] == ' ' || phoneNumber[i] == '\t') {
                    return 0;
                }
                else {
                    return 0; // 숫자가 아닌 문자가 있으면 형식 불만족
                }
                i++;
            }
        }
        else  return 0;


    }
    else {
        while (phoneNumber[i] != '\0' && phoneNumber[i] != '-') {
            if (phoneNumber[i] >= '0' && phoneNumber[i] <= '9') {
                digitCountMiddleHypen++;
            }
            else if (phoneNumber[i] == ' ' || phoneNumber[i] == '\t') {
                return 0;
            }
            else {
                return 0; // 숫자가 아닌 문자가 있으면 형식 불만족
            }
            i++;
        }

        // 하이픈 개수 세기
        while (phoneNumber[i] == '-') {
            hyphenCount2++;
            i++;
        }

        // 전화번호 뒷부분의 숫자 개수 세기
        while (phoneNumber[i] != '\0') {
            if (phoneNumber[i] >= '0' && phoneNumber[i] <= '9') {
                digitCountAfterHyphen++;
            }
            else if (phoneNumber[i] == ' ' || phoneNumber[i] == '\t') {
                return 0;
            }
            else {
                return 0; // 숫자가 아닌 문자가 있으면 형식 불만족
            }
            i++;
        }

    }



    //형식 검사
    if (digitCountMiddleHypen == 4 && hyphenCount1 >= 1 && hyphenCount2 >= 1 && digitCountBeforeHyphen == 3 && digitCountAfterHyphen == 4) {
        return 1; // 형식 만족
    }
    else if (digitCountMiddleHypen == 11 && hyphenCount2 == 0 && hyphenCount1 == 0 && phoneNumber[0] == '0' && phoneNumber[1] == '1' && phoneNumber[2] == '0') {
        return 1;
    }
    else if (digitCountMiddleHypen == 8 && hyphenCount2 == 0 && hyphenCount1 == 0) {
        return 1;
    }
    else if (digitCountAfterHyphen == 4 && digitCountMiddleHypen == 4 && hyphenCount2 >= 1) {
        return 1;
    }

    else {
        return 0; // 형식 불만족
    }
}

// 해당 월의 최대 일수 반환
int getMaxDay(int month, int year) {
    int maxDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // 윤년 검사 (2월의 경우)
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
            return 29; // 윤년일 때 2월은 29일까지 있음
        }
    }

    return maxDays[month - 1];
}

// 입력된 생년월일이 유효한지 검사하는 함수
int isValidDate(char* date, struct DateTime* dt) {
    int year, month, day;
    //int currentYear, lastTwoDigitsOfCurrentYear;
    int len = strlen(date);

    /*  현재 입력받는 시간이랑 연결해야 한다.
    // 현재 시간을 얻기 위한 time_t와 tm 구조체
    time_t now;
    struct tm* tm_now;

    // 현재 시간 얻기
    time(&now);
    tm_now = localtime(&now);

    // 현재 년도의 마지막 두 자리 얻기
    currentYear = tm_now->tm_year + 1900; // tm_year는 1900년부터의 년수를 나타냄
    lastTwoDigitsOfCurrentYear = currentYear % 100;
    */


    // 길이 검사
    if (len != 6 && len != 8) {
        printf("birth's length should be 6 or 8\n");
        return 0;
    }

    // 모든 문자가 숫자인지 검사
    for (int i = 0; i < len; i++) {
        if (!isdigit(date[i])) {
            printf("birth should be digits \n");
            return 0;
        }
    }

    // 길이에 따라 연, 월, 일 분리
    if (len == 6) {
        sscanf(date, "%2d%2d%2d", &year, &month, &day);
        // 월 검사
        if (month < 1 || month > 12) {
            printf("month range is wrong\n");
            return 0;
        }

        // 일 검사
        if (day < 1 || day > getMaxDay(month, year)) {
            printf("day range is wrong\n");
            return 0;
        }

        // 입력된 년도가 현재 년도의 마지막 두 자리보다 작으면 재입력 요청
        if (dt->year >= 2000) {
            if (dt->year % 100 > year) {
                printf("Please enter your date of birth in 8 digits\n");
                check = 1;
                return 0;
            }
            else if (dt->year % 100 == year && dt->month > month) {
                printf("Please enter your date of birth in 8 digits\n");
                check = 1;
                return 0;
            }
            else if (dt->year % 100 == year && dt->month == month && dt->day > day) {
                printf("Please enter your date of birth in 8 digits\n");
                check = 1;
                return 0;
            }


        }
        else {
            if (dt->year % 100 < year) {
                printf("The date of birth cannot be the future than the current date.\n");
                return 0;
            }
            else if (dt->year % 100 == year && dt->month < month) {
                printf("The date of birth cannot be the future than the current date.\n");
                return 0;
            }
            else if (dt->year % 100 == year && dt->month % 100 == month && dt->day < day) {
                printf("The date of birth cannot be the future than the current date.\n");
                return 0;
            }
        }
    }
    else {
        sscanf(date, "%4d%2d%2d", &year, &month, &day);
        if (year < 1900) {
            printf("Birth date must be after 1900.\n");
            return 0;
        }


        // 월 검사
        if (month < 1 || month > 12) {
            printf("month range is wrong.\n");
            return 0;
        }

        // 일 검사
        if (day < 1 || day > getMaxDay(month, year)) {
            printf("day range is wrong.\n");
            return 0;
        }

        if (dt->year < year) {
            printf("The date of birth cannot be the future than the current date.\n");
            return 0;
        }
        else if (dt->year == year && dt->month < month) {
            printf("The date of birth cannot be the future than the current date.\n");
            return 0;
        }
        else if (dt->year == year && dt->month % 100 == month && dt->day < day) {
            printf("The date of birth cannot be the future than the current date.\n");
            return 0;
        }
    }



    return 1;
}

// 입력된 생년월일을 8자리 형태로 변환하는 함수
void formattedBirthDate(char* birthDate, char* formattedbirthDate) {
    int year, month, day;
    int len = strlen(birthDate);

    // 6자리 생년월일일 경우
    if (len == 6) {
        sscanf(birthDate, "%2d%2d%2d", &year, &month, &day);
        year += 1900;
        sprintf(formattedbirthDate, "%04d%02d%02d", year, month, day);
    }
    else if (len == 8) {
        // 8자리 생년월일은 그대로 복사
        strcpy(formattedbirthDate, birthDate);
    }
    else {
        // 유효하지 않은 길이의 입력이 들어오면, 출력 문자열을 비움
        strcpy(formattedbirthDate, "");
    }
}

// 기존 함수들(isOnlyWhitespace, isValidFormat, formatPhoneNumber, isValidDate, formattedBirthDate) 여기에 추가

int isDuplicateInFile(const char* data, const char* fileName) {
    isValidDataFile(fileName);
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL) {
        return 0; // 파일이 없으면 중복 없음
    }

    char buffer[200];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, data)) {
            fclose(fp);
            return 1; // 중복 발견
        }
    }

    fclose(fp);
    return 0; // 중복 없음
}

int compare(const char* phoneNumber, const char* birthDate) {
    char line[1024];
    char sTrash[14];
    char birth[9];
    isValidDataFile("Guest.txt");
    FILE* fp = fopen("Guest.txt", "r");
    if (fp == NULL) {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }
    while (fgets(line, 1024, fp) != NULL) {
        if (strstr(line, phoneNumber)) {
            sscanf(line, "%s %s %s", sTrash, birth, sTrash);
            if (strcmp(birth, birthDate) == 0) {
                fclose(fp);
                return 1;
            }
            else {
                fclose(fp);
                return 0;
            }
        }
    }
    fclose(fp);
    return 1;


}

int guestLog(struct DateTime* dt) {
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
    fprintf(fp, "[GuestLogin],%s%s%s,%s%s,%s\n", year, month, day, hour, minute, u.number);
    fclose(fp);
    return 1;

}


int main_Guest(struct DateTime* dt) {
    char phoneNumber[100];
    char formattedPhoneNumber[100];
    char birthDate[100]; // 8자리 숫자 + NULL
    char formattedbirthDate[100];
    bool isduplicate = false;
    while (1) {
        printf("****************************\n[ Please enter Phone number ]\n\n");
        printf("Enter : ");
        fgets(phoneNumber, sizeof(phoneNumber), stdin);
        //clearInputBuffer();
        phoneNumber[strcspn(phoneNumber, "\n")] = 0; // 개행 문자 제거
        if (strcmp(phoneNumber, "q") == 0) { // 'q'를 입력 받으면 종료
            return 0;
        }

        // 입력 길이 제한 30자 초과 검사 기획서에 추가해야됨 (길이제한(입력제한)이 기획서에는 없음)
        if (strlen(phoneNumber) > 30)
        {
            printf("Input must be at most 30\n");
            continue;
        }

        if (isOnlyWhitespace(phoneNumber)) {
            printf("Phone number is empty.\n");
            continue; // 공백만 있으면 다시 입력
        }
        // 전화번호 형식 검사
        if (!isValidFormat(phoneNumber)) {
            printf("The phone number is not in the correct format.\n");
            continue; // 올바른 형식이 아니면 다시 입력
        }

        // 전화번호를 원하는 형식으로 변환
        formatPhoneNumber(phoneNumber, formattedPhoneNumber);

        if (isDuplicateInFile(formattedPhoneNumber, "Member.txt")) {
            printf("The phone number you entered already exists in Member.\n");
            continue;
        }
        if (isDuplicateInFile(formattedPhoneNumber, "Guest.txt")) {
            isduplicate = true;
        }
        break; // 올바른 형식이면 while 루프를 벗어남
    }


    while (1) {
        check = 0;
        printf("Please enter your date of birth. (6 or 8 digits): ");
        fgets(birthDate, sizeof(birthDate), stdin);
        birthDate[strcspn(birthDate, "\n")] = 0; // 개행 문자 제거
        if (strcmp(birthDate, "q") == 0) { // 'q'를 입력 받으면 종료
            return 0;
        }
        if (strlen(birthDate) > 30)
        {
            printf("Input must be at most 30\n");
            continue;
        }

        if (isOnlyWhitespace(birthDate)) {
            printf("Date of birth is empty.\n");
            continue; // 공백만 있으면 다시 입력
        }

        // 생년월일 형식 검사
        if (!isValidDate(birthDate, dt)) {
            if (check == 1) {
                continue;
            }
            printf("The date of birth is not in the correct format.\n");
            clearInputBuffer(); // 여기도 추가
            continue; // 올바른 형식이 아니면 다시 입력
        }


        formattedBirthDate(birthDate, formattedbirthDate);

        /*  if (isDuplicateBirthDate(formattedbirthDate)) {
              printf("입력하신 생년월일이 이미 파일에 존재합니다. 다시 입력해주세요.\n");
              continue;
          }*/

        if (isduplicate == true) {
            if (compare(formattedPhoneNumber, formattedbirthDate) == 0) {
                printf("It is not correspond with birth when you had joined Non-Membership.\n");
                continue;
            }
        }
        break; // 올바른 형식이면 while 루프를 벗어남
    }
    if (isduplicate == false) {
        isValidDataFile("Guest.txt");
        FILE* fp = fopen("Guest.txt", "a");
        if (fp == NULL) {
            printf("Could not open the file.\n");
            return -1;
        }
        char year[5];
        char month[3];
        char day[3];
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




        fprintf(fp, "%s %s %s%s%s\n", formattedPhoneNumber, formattedbirthDate, year, month, day);
        fclose(fp);

        printf("Phonenumber %s and date of birth %s are saved.\n", formattedPhoneNumber, formattedbirthDate);
    }
    u.member = 0;
    int j = 0;
    for (int i = 0; i < 8; i++) {
        u.birth[i] = formattedbirthDate[i] - '0';
    }

    for (int i = 0; i < strlen(formattedPhoneNumber); i++) {
        u.number[i] = formattedPhoneNumber[i];
    }
    if (guestLog(dt) == 0) {
        return -1;
    }
    return 1;

}