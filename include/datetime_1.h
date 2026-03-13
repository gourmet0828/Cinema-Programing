#pragma once
#ifndef __DATETIME_H__
#define __DATETIME_H__

#include "base.h"

void datetime_to_string(char* date_org, char* date_dst, char* time_org, char* time_dst) {
    strncpy(date_dst, date_org, sizeof(char) * 4);
    strncpy(date_dst + 4, ".", sizeof(char));
    strncpy(date_dst + 5, date_org + 4, sizeof(char) * 2);
    strncpy(date_dst + 7, ".", sizeof(char));
    strncpy(date_dst + 8, date_org + 6, sizeof(char) * 2);
    date_dst[10] = '\0';

    strncpy(time_dst, time_org, sizeof(char) * 2);
    strncpy(time_dst + 2, ":", sizeof(char));
    strncpy(time_dst + 3, time_org + 2, sizeof(char) * 2);
    time_dst[5] = '\0';
}

void get_current_time_from_csv(const char* file_name, char* date, char* time) {
    isValidDataFile(file_name);
    FILE* file = fopen(file_name, "r");

    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_CHARS_PER_LINE];
    if (fgets(buffer, MAX_CHARS_PER_LINE, file) != NULL) {
        // csv format에 맞게 읽어들임
        sscanf(buffer, "%[^,],%[^,]",
            date,
            time
        );
    }
    fclose(file);
}

int compare_date(char* date_1, char* date_2) {
    int date_int_1 = atoi(date_1);
    int date_int_2 = atoi(date_2);

    if (date_int_1 > date_int_2) {
        return 1;
    }
    else if (date_int_1 == date_int_2) {
        return 0;
    }
    else if (date_int_1 < date_int_2) {
        return -1;
    }
}

int compare_time(char* time_1, char* time_2) {
    int time_int_1 = atoi(time_1);
    int time_int_2 = atoi(time_2);

    if (time_int_1 > time_int_2) {
        return 1;
    }
    else if (time_int_1 == time_int_2) {
        return 0;
    }
    else if (time_int_1 < time_int_2) {
        return -1;
    }
}
#endif // !__DATETIME_H__

