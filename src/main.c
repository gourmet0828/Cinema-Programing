//4.1
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "guest.h"
#include "menu_1.h"
#include "join.h"
#include "login.h"
#include "time.h"
#include "menu_2.h"
#include "base.h"
#include "reservation_1.h" 
#include "datetime_1.h"
#include "dateTime.h"
#include "reservation.h"
//#include "isValidDataFile.h"
#include "CouponManager.h"
//#include "xCouponManager.h"
#include "zCouponManager.h"



struct DateTime dt;


int main(void) {
    //전역변수 - char currentDate[] 현재날짜, char currentTime[] 현재 시간,구조체 user { char tel[] = 전화번호,char birth[] = 생년월일, int member = 0/1 회원이면 1 비회원이면 0 }
    // 종료 비정상일 경우 -1/정상일경우(나머지 숫자)/ 0은 현재 날짜 및 시간 입력 다시 출력
    
    checkFiles();
    DateTime(&dt);
    int loginResult;
    int joinResult;
    int guestResult;
    int reservationResult;
    int checkAndCancelResult;
    
  
    while (1) 
    {
        while (1) 
        {
            int inputDateTimeResult = inputDateTime(&dt);
            if (inputDateTimeResult == -1) { //날짜 시간 입력 함수 호출
                return 0;
            }
            else if (inputDateTimeResult == 0) 
            {
                continue;
            }
            else 
            {
                struct Coupon xc;
                updateAllCouponStatus(&xc, &dt);
                // 정상종료 -1, 1,2,3
                switch (main_Initial_Menu()) 
                {//메뉴 호출 함수
                case -1: //종료
                    printf("****************************\n[ The program is terminated ]\n****************************\n");
                    return 0;
                case 1:
                    loginResult = main_Login(&dt); //로그인 함수 호출
                    // 로그 파일오류 = -1, 정상종료(1)일 경우 예매 파트로 넘어감 , 3번틀리면 0을 반환 continue;
                    // 1일 경우 break;
                    if (loginResult == -1)
                    {
                        printf("****************************\n[ The program is terminated ]\n****************************\n"); printf("프로그램을 종료합니다!\n");
                        return 0;
                    }
                    else if (loginResult == 0) 
                    {
                        continue;
                    }
                    break;
                case 2:
                    joinResult = main_Join(&dt); //회원가입 함수 호출
                    // 파일 오류 = -1, 정상 종료(0)
                    if (joinResult == -1) {
                        printf("****************************\n[ The program is terminated ]\n****************************\n");
                        return 0;
                    }
                    else 
                    {
                        continue;
                    }
                case 3:
                    guestResult = main_Guest(&dt); //비회원 함수 호출
                    // 파일 오류 = -1, 정상 종료(1)일 경우 예매 파트로 넘어감
                    // 1일 경우 break;
                    if (guestResult == -1) {
                        printf("****************************\n[ The program is terminated ]\n****************************\n");
                        return 0;
                    }
                    else if (guestResult == 0) {
                        continue;
                    }
                    {
                        break;
                    }
                }
                break;
            }
        }

        while (1) 
        {
            int inputDateTimeResult = inputDateTime(&dt);
            if (inputDateTimeResult == -1) { //날짜 시간 입력 함수 호출
                return 0;
            }
            else if (inputDateTimeResult == 0) 
            {
                continue;
            }
            else {

                struct Coupon xc;
                updateAllCouponStatus(&xc, &dt);
                ZCoupon(&dt);
                int reservation_Menu_Result = main_Reservation_Menu();
                if (reservation_Menu_Result == -1) { //메인 메뉴 입력 프롬프트 함수

                    printf("****************************\n[ The program is terminated ]\n****************************\n");
                    return 0;
                }
                else if (reservation_Menu_Result == 1)
                {
                    reservationResult = reservation(&dt); //결과값 = 예매함수
                    if (reservationResult == -1) { //파일 중대 결함
                        return 0;
                    }
                    else {// 정상결과(-1, 0제외 정수) + 사용자가 q를입력해서 현재날짜 및 시간입력으로 가고 싶을 때(결과값 == 0) 
                        continue;
                    }
                }
                else if (reservation_Menu_Result == 2) 
                {
                    checkAndCancelResult = check_and_cancel(); //예매조회 및 취소 함수( //안에서 반복문으로 메뉴 출력및 삭제할 영화 선택까지);
                    if (checkAndCancelResult == 0) // 돌아가기 2번 or 삭제할 영화 선택에서 q를 입력시 0을 반환)
                    {
                        continue;
                    }
                    else 
                    {
                        return 0;
                    }
                }
                else if (reservation_Menu_Result == 3)
                {
                    if (logoutLog(&dt, u.member) == 0) 
                    {
                        return 0;
                    }
                    break;
                }

            }
        }
    }
    
}