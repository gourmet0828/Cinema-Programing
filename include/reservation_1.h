#pragma once
#ifndef __RESERVATION_H__
#define __RESERVATION_H__

#include "base.h"
#include "datetime_1.h"
#define LINE_BREAK "****************************"

typedef struct _reservation {
    char phone_number[MAX_CHARS_PHONE_NUMBER];
    char title[MAX_CHARS_TITLE];
    char date[9];
    char time[5];
    char position[MAX_CHARS_SEAT];
    bool cancelled;
} Reservation;


void update_reservation_to_csv(const char* file_name, Reservation* reservations, int* num_of_reservations) {
    isValidDataFile(file_name);
    FILE* temp_file = fopen(file_name, "r");

    if (temp_file == NULL) {
        fprintf(stderr, "Error: Cannot access to DataFile. Terminate the program: %s\n", file_name);
        exit(-1);
    }

    Reservation temp_reservations[MAX_RESERVATIONS];
    int num_of_temp_reservations = 0;

    char* user_phone_number = u.number;

    char buffer[MAX_CHARS_PER_LINE];

    while (fgets(buffer, MAX_CHARS_PER_LINE, temp_file) != NULL) {
        // csv format에 맞게 읽어들임
        sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%s",
            temp_reservations[num_of_temp_reservations].phone_number,
            temp_reservations[num_of_temp_reservations].title,
            temp_reservations[num_of_temp_reservations].date,
            temp_reservations[num_of_temp_reservations].time,
            temp_reservations[num_of_temp_reservations].position);

        char current_phone_number[MAX_CHARS_PHONE_NUMBER];
        sprintf(current_phone_number, "%s", temp_reservations[num_of_temp_reservations].phone_number);
        if (strcmp(user_phone_number, current_phone_number) == 0) {
            continue;
        }

        temp_reservations[num_of_temp_reservations].cancelled = false;

        num_of_temp_reservations++;

        if (num_of_temp_reservations >= MAX_RESERVATIONS) break;
    }
    fclose(temp_file);

    isValidDataFile(file_name);
    FILE* file = fopen(file_name, "w");

    if (file == NULL) {
        fprintf(stderr, "Error: Cannot access to DataFile. Terminate the program: %s\n", file_name);
        exit(-1);
    }


    for (int i = 0; i < num_of_temp_reservations; i++) {
        fprintf(file, "%s,%s,%s,%s,%s\n",
            temp_reservations[i].phone_number,
            temp_reservations[i].title,
            temp_reservations[i].date,
            temp_reservations[i].time,
            temp_reservations[i].position
        );
    }

    for (int i = 0; i < *num_of_reservations; i++) {
        fprintf(file, "%s,%s,%s,%s,%s\n",
            reservations[i].phone_number,
            reservations[i].title,
            reservations[i].date,
            reservations[i].time,
            reservations[i].position
        );
    }
    fclose(file);
}
void print_cancel_menu() {
    printf(LINE_BREAK);
    printf("\n[Check Menu] Please select the menu number to run\n\n");

    char date[12];
    char time[8];
    get_current_time_from_csv(DATETIME_FILE, date, time);

    char date_formatted[100];
    char time_formatted[50];
    datetime_to_string(date, date_formatted, time, time_formatted);

    printf("Current Date and Time: %s %s\n\n", date_formatted, time_formatted);

    printf("1. Cancellation\n\n2. Go Back");
}

void read_reservation_from_csv(const char* file_name, Reservation* reservations, int* num_of_reservations) {
    isValidDataFile(file_name);
    FILE* file = fopen(file_name, "r");

    if (file == NULL) {
        fprintf(stderr, "Error: Cannot access to DataFile. Terminate the program: %s\n", file_name);
        exit(-1);
    }

    char* user_phone_number = u.number;

    char buffer[MAX_CHARS_PER_LINE];
    *num_of_reservations = 0;
    while (fgets(buffer, MAX_CHARS_PER_LINE, file) != NULL) {
        // csv format에 맞게 읽어들임
        sscanf(buffer, "%[^,],%[^,],%[^,],%[^,],%s",
            reservations[*num_of_reservations].phone_number,
            reservations[*num_of_reservations].title,
            reservations[*num_of_reservations].date,
            reservations[*num_of_reservations].time,
            reservations[*num_of_reservations].position);

        char current_phone_number[MAX_CHARS_PHONE_NUMBER];
        sprintf(current_phone_number, "%s", reservations[*num_of_reservations].phone_number);
        if (strcmp(user_phone_number, current_phone_number) != 0) {
            continue;
        }

        reservations[*num_of_reservations].cancelled = false;

        (*num_of_reservations)++;

        if (*num_of_reservations >= MAX_RESERVATIONS) break;
    }
    fclose(file);
}

char* trim_whitespace(char* str) {
    char* end;

    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';
    return str;
}
void print_reservations(Reservation* reservations, int num_of_reservations) {
    // 예매 내역 하나씩 출력
    for (int i = 0; i < num_of_reservations; i++) {
        // 기획서랑 format 통일
        char date_formatted[100];
        char time_formatted[50];
        datetime_to_string(reservations[i].date, date_formatted, reservations[i].time, time_formatted);

        printf("%d. %s, %s, %s, ",
            i + 1,
            reservations[i].title,
            date_formatted,
            time_formatted);


        int seats = strlen(reservations[i].position) / 2;

        char* position = reservations[i].position;

        for (int k = 0; k < seats; k++) {
            printf("%c%c", *(position + k * 2 + 0), *(position + k * 2 + 1));
            if (k != seats - 1) {
                printf(", ");
            }
        }

        printf("\n");
    }
}
int check_and_cancel(void) {
    while (1) {
        // 예약 불러오기 및 출력
        Reservation reservations[MAX_RESERVATIONS];
        int num_of_reservations = 0;

        read_reservation_from_csv(RESERVATION_FILE, reservations, &num_of_reservations);


        // 메뉴 출력 및 유저 선택
        print_cancel_menu();

        printf("\n\n");

        print_reservations(reservations, num_of_reservations);

        char input[MAX_CHARS_PER_LINE];
        int choice;

        printf("\n\nEnter: ");
        //////////////////////////////////////////////////////////////////////////////
        if (scanf_s("%[^\n]s", input, sizeof(input)) == 0) {
            while (getchar() != '\n');
            printf("The number of characters exceeds or just input Enter.\n");
            continue;
        }

        while (getchar() != '\n');
        //////////////////////////////////////////////////////////////////////////////ppp

        //while (scanf("%[^\n]%*c", input) != 1);

        char* trimmedInput = trim_whitespace(input);
        if (tolower(trimmedInput[0]) == 'q' && trimmedInput[1] == '\0') {
            return 0;
        }

        // choice가 invalid한 경우
        if (sscanf(trimmedInput, "%d", &choice) != 1) {
            printf("*Invalid Input. Only Integer number can be entered.*\n");
            continue;
        }

        char temp_buffer_choice[10];
        sprintf(temp_buffer_choice, "%d", choice);
        if (strcmp(trimmedInput, temp_buffer_choice) != 0) {
            printf("*Invalid Input. Only Integer number can be entered.*\n");
            continue;
        }

        printf("\n\n");

        // choice에 따른 분기    
        switch (choice) {
        case 1:
            ;
            int cancel_not_done = 1;

            while (cancel_not_done) {
                // 예약 출력 및 취소할 예약 선택
                printf(LINE_BREAK);
                printf("\n[Cancellation] Please select the number to cancel\n\n");
                char date[12];
                char time[8];
                get_current_time_from_csv(DATETIME_FILE, date, time);

                char date_formatted[100];
                char time_formatted[50];
                datetime_to_string(date, date_formatted, time, time_formatted);

                printf("Current Date and Time: %s %s\n\n", date_formatted, time_formatted);
                print_reservations(reservations, num_of_reservations);
                printf("\n\n(If you enter q, you will return to the main menu.)\n\n");
                printf("Enter: ");

                //while (scanf("%[^\n]%*c", input) != 1);
                ///////////////////////////////////////////////////////////////////////
                if (scanf_s("%[^\n]s", input, sizeof(input)) == 0) {
                    while (getchar() != '\n');
                    printf("The number of characters exceeds or just input Enter.\n");
                    continue;
                }

                while (getchar() != '\n');
                ///////////////////////////////////////////////////////////////////////

                printf("\n\n");

                int selected_index;

                char* trimmedInput_reserv = trim_whitespace(input);
                if (tolower(trimmedInput_reserv[0]) == 'q' && trimmedInput_reserv[1] == '\0') {
                    return 0;
                }

                // choice가 invalid한 경우
                if (sscanf(trimmedInput_reserv, "%d", &selected_index) != 1) {
                    printf("*Invalid Input. Only Integer number can be entered.*\n\n");
                    continue;
                }

                char temp_buffer_reserv[10];
                sprintf(temp_buffer_reserv, "%d", selected_index);
                if (strcmp(trimmedInput_reserv, temp_buffer_reserv) != 0) {
                    printf("*Invalid Input. Only Integer number can be entered.*\n\n");
                    continue;
                }

                if (selected_index >= 1 && selected_index <= num_of_reservations) {
                    selected_index -= 1; // index starts from 0, not 1
                    Reservation selected = reservations[selected_index];

                    char date[12];
                    char time[8];
                    get_current_time_from_csv(DATETIME_FILE, date, time);

                    if ((compare_date(selected.date, date) == 1) ||
                        ((compare_date(selected.date, date) == 0) && (compare_time(selected.time, time) == 1))) {

                        Reservation deleted = selected;

                        for (int i = selected_index; i < num_of_reservations - 1; i++) {
                            reservations[i] = reservations[i + 1];
                        }
                        num_of_reservations--;
                        printf("Reservation number %d has been cancelled.\n", selected_index + 1);
                        isValidDataFile(LOG_FILE);
                        FILE* log_file = fopen(LOG_FILE, "a");

                        if (log_file == NULL) {
                            fprintf(stderr, "Error: Cannot access to DataFile. Terminate the program: %s\n", LOG_FILE);
                            exit(-1);
                        }

                        // Format : [CancelTicket],20240403,1700,010-1234-5678,TheaterA,b2 
                        fprintf(log_file, "[CancelTicket],%s,%s,%s,%s,%s\n",
                            deleted.date,
                            deleted.time,
                            deleted.phone_number,
                            deleted.title,
                            deleted.position
                        );

                        fclose(log_file);

                        // Theater csv에 update
                        char* filename = "";
                        char title[MAX_CHARS_TITLE];

                        for (int i = 0; deleted.title[i]; i++) {
                            if (deleted.title[i] == ' ') {
                                continue;
                            }
                            title[i] = tolower(deleted.title[i]);
                            title[i + 1] = '\0';
                        }

                        if (!strcmp(title, "dune"))
                        {
                            filename = "TheaterA.csv";
                            isValidDataFile(filename);
                        }
                        else if (!strcmp(title, "commentunit"))
                        {
                            filename = "TheaterB.csv";
                            isValidDataFile(filename);
                        }
                        else if (!strcmp(title, "thebeekeeper"))
                        {
                            filename = "TheaterC.csv";
                            isValidDataFile(filename);
                        }

                        int seats = strlen(deleted.position) / 2;

                        char* position = deleted.position;

                        int to_delete_A = 0b00011111;
                        int to_delete_B = 0b00011111;

                        int seats_binary[] = {
                            0b11111111,
                            0b00001111,
                            0b00010111,
                            0b00011011,
                            0b00011101,
                            0b00011110
                        };

                        for (int k = 0; k < seats; k++) {

                            char key = tolower(*(position + k * 2 + 0));

                            char to_delete_str[2];
                            to_delete_str[0] = *(position + k * 2 + 1);
                            to_delete_str[1] = '\0';

                            int to_delete = atoi(to_delete_str);

                            if (key == 'a') {
                                to_delete_A = to_delete_A & seats_binary[to_delete];
                            }
                            else if (key == 'b') {
                                to_delete_B = to_delete_B & seats_binary[to_delete];
                            }
                        }

                        FILE* theater_file = fopen(filename, "r");

                        if (theater_file == NULL) {
                            fprintf(stderr, "Error: Cannot access to DataFile. Terminate the program: %s\n", filename);
                            exit(-1);
                        }

                        FILE* temp_theater = tmpfile();
                        if (temp_theater == NULL) {
                            fprintf(stderr, "Error: Cannot access to DataFile. Terminate the program: tmpfile()\n");
                            exit(-1);
                        }


                        char buffer[MAX_CHARS_PER_LINE];
                        while (fgets(buffer, MAX_CHARS_PER_LINE, theater_file) != NULL) {
                            // csv format에 맞게 읽어들임
                            char delete_date[9];
                            char delete_time[5];
                            char temp_movie[100];
                            int temp_running_time;
                            char seats_str[20];

                            sscanf(buffer, "%[^,],%[^,],%[^,],%d,%s", // Dune, 20240403, 1200, 60, 0100000010
                                temp_movie,
                                delete_date,
                                delete_time,
                                &temp_running_time,
                                seats_str);


                            int j = 0;
                            for (int i = 0; delete_date[i]; i++) {
                                if (delete_date[i] == ' ') {
                                    continue;
                                }
                                delete_date[j] = delete_date[i];
                                j++;
                            }
                            delete_date[j] = '\0';


                            j = 0;
                            for (int i = 0; delete_time[i]; i++) {
                                if (delete_time[i] == ' ') {
                                    continue;
                                }
                                delete_time[j] = delete_time[i];
                                j++;
                            }
                            delete_time[j] = '\0';

                            j = 0;
                            for (int i = 0; seats_str[i]; i++) {
                                if (seats_str[i] == ' ') {
                                    continue;
                                }
                                seats_str[j] = seats_str[i];
                                j++;
                            }
                            seats_str[j] = '\0';

                            if (strcmp(delete_date, deleted.date) == 0 && strcmp(delete_time, deleted.time) == 0) {
                                char seats_A_str[10];
                                char seats_B_str[10];

                                strncpy(seats_A_str, seats_str, 5);
                                seats_A_str[5] = '\0';

                                strncpy(seats_B_str, seats_str + 5, 5);
                                seats_B_str[5] = '\0';

                                int seats_A = (int)strtol(seats_A_str, NULL, 2);
                                int seats_B = (int)strtol(seats_B_str, NULL, 2);

                                seats_A = seats_A & to_delete_A;
                                seats_B = seats_B & to_delete_B;

                                for (int idx = 4; idx >= 0; idx--) {
                                    if (seats_A & 1) {
                                        seats_A_str[idx] = '1';
                                    }
                                    else {
                                        seats_A_str[idx] = '0';
                                    }

                                    seats_A >>= 1;
                                }

                                for (int idx = 4; idx >= 0; idx--) {
                                    if (seats_B & 1) {
                                        seats_B_str[idx] = '1';
                                    }
                                    else {
                                        seats_B_str[idx] = '0';
                                    }

                                    seats_B >>= 1;
                                }

                                fprintf(temp_theater, "%s,%s,%s,%d,%s%s\n", temp_movie, delete_date, delete_time, temp_running_time, seats_A_str, seats_B_str); // Dune, 20240403, 1200, 60, 0100000010
                            }
                            else {
                                fprintf(temp_theater, "%s", buffer);
                            }
                        }

                        fclose(theater_file);

                        theater_file = fopen(filename, "wb");

                        if (theater_file == NULL) {
                            fprintf(stderr, "Error: Cannot access to DataFile. Terminate the program: %s\n", filename);
                            exit(-1);
                        }

                        fseek(temp_theater, 0, SEEK_SET);

                        int n, m;
                        unsigned char file_buffer[1024];

                        do {
                            n = fread(file_buffer, 1, sizeof(file_buffer), temp_theater);
                            if (n) {
                                m = fwrite(file_buffer, 1, n, theater_file);
                            }
                            else {
                                m = 0;
                            }
                        } while ((n > 0) && (n == m));

                        fclose(theater_file);
                        fclose(temp_theater);

                        update_reservation_to_csv(RESERVATION_FILE, reservations, &num_of_reservations);

                        cancel_not_done = 0;
                    }
                    else {
                        printf("*Cancellable time has passed.*\n\n");
                    }
                }

                else {
                    printf("*Invalid Input. Only valid reservation numbers can be entered.*\n\n");
                }
            }

            break;

        case 2:
            return 0;

        default:
            printf("*Invalid Input. Only 1 and 2 can be entered. Please re-enter it.*\n");
            continue;
        }
    }

    return 0;
}






#endif // !__RESERVATION_H__