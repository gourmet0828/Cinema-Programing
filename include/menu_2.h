#pragma once

int isOnlySpacesAndDigits(const char* str) {
    while (*str) {
        if (!isdigit(*str) && !isspace(*str)) {
            return 0; // 숫자나 공백이 아닌 다른 문자가 있는 경우 0을 반환
        }
        str++; // 다음 문자로 이동
    }
    return 1; // 모든 문자가 숫자 또는 공백인 경우 1을 반환
}

int main_Reservation_Menu(void) {
    while (1) {
        printf("*************************\n");
        printf("Please choose a menu.\n");
        printf("1. Making a reservation\n");
        printf("2. Booking inquiry and cancellation\n");
        printf("3. Log Out\n");
        printf("4. Terminate\n");
        printf("*************************\n");

        char menu[31];
        int num = 0;
        if (scanf_s("%[^\n]s", menu, sizeof(menu)) == 0) {
            while (getchar() != '\n');
            printf("The number of characters exceeds or just input Enter.\n");
            continue;
        }
        while (getchar() != '\n');


        if (isOnlySpacesAndDigits(menu)) {//공백이랑 숫자만 있는지 판단
            //공백이랑 숫자만 있으면 숫자가 하나인지 판단
            int isDigitonly = 0;
            for (int i = 0; i < strlen(menu); i++) {
                if (isdigit(menu[i])) {
                    num = menu[i] - '0';
                    isDigitonly++;
                }
                if (isDigitonly > 1) {//입력한 숫자가 하나인지 판단
                    num = 0;
                    printf("Only one number can be entered.\n");
                    break;
                }
            }
        }
        else {
            printf("You can only enter a number and spaces.\n");
            continue;
        }


        switch (num) {
        case 1:
            return 1;
            break;
        case 2:
            return 2;
            break;
        case 3:
            return 3;
            break;
        case 4:
            return -1;
        default:
            printf("Please choose one out of 1, 2, 3, 4\n");
        }
    }
}