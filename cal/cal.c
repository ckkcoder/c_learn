#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// 想写跨平台代码， 但是找不到教程
// #if defined(_LINUX)
#include <unistd.h>
// 设置反显
#define HIGHT_LIGHT() printf("\033[7m")
#define UN_HIGHT_LIGHT() printf("\033[27m")
// #endif

// #if defined(_WIN64)
// #include <windows.h>
// #define HIGHT_LIGHT()
// #define UN_HIGHT_LIGHT()
// #endif
//
// #if defined(_MSC_VER) && !defined(__cplusplus) && !defined(inline)
//     #define inline __inline
// #endif



#define MONTH_MAX 12

const char month_str[MONTH_MAX][10] = {"一月","二月","三月","四月","五月","六月",
                            "七月","八月","九月","十月","十一月","十二月"};
const int month_day[MONTH_MAX] = {31, 28, 31, 30, 31, 30,
                                    31, 31, 30, 31, 30, 31};

void show_year_cal(int year);
void show_month_cal(int year, int mon);
void show_now();

int main(int argc, char *argv[])
{
    int year = 0,mon = 0;
    int i,j;

    if(argc == 2 || argc == 3){
        for(i = 0; i < strlen(argv[argc - 1]); i++){
            if( argv[argc - 1][i] >= '0' && argv[argc - 1][i] <= '9' ){
                year = year * 10 + (argv[argc - 1][i] - '0');
            }else{
                printf("cal: not a valid year %s\n",argv[argc - 1]);
                exit(1);
            }
        }
        if(!(year >= 1900 && year <= 9999)){
            printf("cal: year `%d' not in range 1900..9999\n", year);
            exit(1);
        }
        if(argc == 3){
            for(i = 0; i < strlen(argv[argc - 2]); i++){
                if( argv[argc - 2][i] >= '0' && argv[argc - 2][0] <= '9' ){
                    mon = mon * 10 + (argv[argc - 2][i] - '0');
                }else{
                    printf("cal: %s is neither a month number (1..12)"
                            "nor a name\n", argv[argc - 2]);
                    exit(1);
                }
            }
            if(mon > 0 && mon <= 12)
                show_month_cal(year, mon);
            else{
                printf("cal: %s is neither a month number (1..12)"
                        "nor a name\n", argv[argc - 2]);
                exit(1);
            }
        }else
            show_year_cal(year);
    }else{
        show_now();
    }
    return 0;
}

bool is_leapyear(int year)
{
    if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        return true;
    return false;
}

int month_days(int year, int month)
{
    if(month == 2 && is_leapyear(year))
        return month_day[month - 1] + 1;
    return month_day[month - 1];
}

int get_first_wday_year(int year)
{
    int i, wday = 1;
    for(i = 1900; i < year; i++){
        if(is_leapyear(i)) wday = (wday + 366) % 7;
        else wday = (wday + 365) % 7;
    }
    return wday;
}

int get_first_wday_mon(int year, int mon)
{
    int wday = get_first_wday_year(year);
    int i;
    for(i = 1; i < mon; i++)
        wday = (wday + month_days(year, i)) % 7;
    return wday;
}

struct tm * now_time()
{
    time_t time_second = time(0);
    struct tm *pnow_time = localtime(&time_second);
    pnow_time->tm_year += 1900;
    return pnow_time;
}

inline bool is_same_day(const struct tm *ptime, int year, int mon, int mday)
{
    if(ptime->tm_year == year && ptime->tm_mon == mon
                && ptime->tm_mday == mday)
        return true;
    return false;
}

void show_month_cal(int year, int mon)
{
    int f_wday, mdays, i;
    struct tm *pnow_time = now_time();

    f_wday = get_first_wday_mon(year, mon);
    mdays = month_days(year, mon);
    printf("      %s %d\n", month_str[mon - 1], year);
    printf("日 一 二 三 四 五 六\n");
    for(i = 0; i < f_wday; i++)
        printf("   ");
    for(i = 1; i <= mdays; i++){
        if(is_same_day(pnow_time, year, mon - 1, i)) HIGHT_LIGHT();
        if(i < 10)  printf(" ");
        printf("%d", i);
        if(is_same_day(pnow_time, year, mon - 1, i)) UN_HIGHT_LIGHT();
        printf("%c", ((f_wday == 6)?'\n':' '));
        f_wday = (f_wday + 1) % 7;
    }
    printf("\n");
}

void show_year_cal(int year)
{
    int f_mon, f_wday, f_mday, s_wday, s_mday, t_wday, t_mday;
    int f_days, s_days, t_days;
    struct tm *pnow_time = now_time();
    int i,j,k;

    t_wday = get_first_wday_year(year);

    printf("\t\t\t    %d\n", year);
    for(f_mon = 1; f_mon <= 10; f_mon += 3){
        f_mday = month_days(year, f_mon);
        f_wday = t_wday % 7;
        s_mday = month_days(year, f_mon + 1);
        s_wday = (f_wday + f_mday) % 7;
        t_mday = month_days(year, f_mon + 2);
        t_wday = (s_wday + s_mday) % 7;
        f_days = s_days = t_days = 0;

        printf("         %s        ", month_str[f_mon - 1]);
        printf("         %s        ", month_str[f_mon]);
        printf("          %s\n", month_str[f_mon + 1]);
        printf("日 一 二 三 四 五 六  ");
        printf("日 一 二 三 四 五 六  ");
        printf("日 一 二 三 四 五 六  \n");

        do{
            for(i = 0; i < f_wday; i++)
                printf("   ");
            while(f_wday < 7 && ++f_days <= f_mday){
                if(is_same_day(pnow_time, year, f_mon - 1, f_days)) HIGHT_LIGHT();
                if(f_wday < 10)  printf(" ");
                printf("%d", f_days);
                if(is_same_day(pnow_time, year, f_mon - 1, f_days)) UN_HIGHT_LIGHT();
                ++f_wday;
                printf(" ");
            }
            for(;f_wday < 7; f_wday++)
                printf("   ");
            f_wday %= 7;
            printf(" ");

            for(i = 0; i < s_wday; i++)
                printf("   ");
            while(s_wday < 7 && ++s_days <= s_mday){
                if(is_same_day(pnow_time, year, f_mon, s_days)) HIGHT_LIGHT();
                if(s_wday < 10)  printf(" ");
                printf("%d", s_days);
                if(is_same_day(pnow_time, year, f_mon, s_days)) UN_HIGHT_LIGHT();
                ++s_wday;
                printf(" ");
            }
            for(;s_wday < 7; s_wday++)
                printf("   ");
            s_wday %= 7;
            printf(" ");

            for(i = 0; i < t_wday; i++)
                printf("   ");
            while(t_wday < 7 && ++t_days <= t_mday){
                if(is_same_day(pnow_time, year, f_mon + 1, t_days)) HIGHT_LIGHT();
                if(t_wday < 10)  printf(" ");
                printf("%d", t_days);
                if(is_same_day(pnow_time, year, f_mon + 1, t_days)) UN_HIGHT_LIGHT();
                ++t_wday;
                printf(" ");
            }
            t_wday %= 7;
            printf("\n");
        }while(f_days <= f_mday || s_days <= s_mday || t_days <= t_mday);
        printf("\n");
    }
}

void show_now()
{
    struct tm *pnow_time = now_time();
    show_month_cal(pnow_time->tm_year, pnow_time->tm_mon + 1);
}
