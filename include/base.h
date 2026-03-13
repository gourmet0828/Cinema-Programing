#pragma once
#ifndef __BASE_H__
#define __BASE_H__


#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define RESERVATION_FILE "Reservation.csv"
#define DATETIME_FILE "DateTime.csv"
#define LOG_FILE "LogData.csv"

#define MAX_RESERVATIONS 128
#define MAX_CHARS_PER_LINE 1024
#define MAX_CHARS_PHONE_NUMBER 32 
#define MAX_CHARS_TITLE 256
#define MAX_NUMS_SEAT 32
#define MAX_CHARS_SEAT 32

#endif __BASE_H__