/* This software is released into the public domain.
 *
 * Do whatever you want with it. I don't give a shit.
 * - AnmiTaliDev <anmitali198@gmail.com>, 2026
 */

#include <string.h>
#include <limits.h>
#include "../include/common.h"

/* Days in each month (non-leap year) */
static const int days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/* Error value for time64_t functions (LLONG_MIN + 1 to avoid conflict with valid timestamps) */
#define TIME64_ERROR ((time64_t)(LLONG_MIN + 1))

int time64_to_tm(time64_t t, struct tm *tm, int use_utc __attribute__((unused))) {
    memset(tm, 0, sizeof(struct tm));
    
    // Days since epoch
    time64_t days = t / 86400;
    time64_t seconds_in_day = t % 86400;
    
    if (seconds_in_day < 0) {
        seconds_in_day += 86400;
        days--;
    }
    
    // Calculate time of day
    tm->tm_hour = (int)(seconds_in_day / 3600);
    tm->tm_min = (int)((seconds_in_day % 3600) / 60);
    tm->tm_sec = (int)(seconds_in_day % 60);
    
    // Calculate year
    int year = 1970;
    time64_t days_left = days;
    
    if (days >= 0) {
        // Forward from 1970
        while (1) {
            int year_days = is_leap_year(year) ? 366 : 365;
            if (days_left < year_days) {
                break;
            }
            days_left -= year_days;
            year++;
        }
    } else {
        // Backward from 1970
        days_left++;  // Adjust for day calculation
        while (days_left <= 0) {
            year--;
            int year_days = is_leap_year(year) ? 366 : 365;
            days_left += year_days;
        }
        days_left--;  // Restore adjustment
    }
    
    tm->tm_year = year - 1900;
    tm->tm_yday = (int)days_left;
    
    // Calculate month and day
    int month = 0;
    int leap = is_leap_year(year);
    
    for (month = 0; month < 12; month++) {
        int month_days = days_in_months[month];
        if (month == 1 && leap) {
            month_days = 29;
        }
        
        if (days_left < month_days) {
            break;
        }
        days_left -= month_days;
    }
    
    tm->tm_mon = month;
    tm->tm_mday = (int)days_left + 1;
    
    // Calculate day of week (0 = Sunday)
    // January 1, 1970 was Thursday (4)
    time64_t total_days = t / 86400;
    tm->tm_wday = (int)((total_days + 4) % 7);
    if (tm->tm_wday < 0) {
        tm->tm_wday += 7;
    }
    
    tm->tm_isdst = -1;
    
    return 0;
}

time64_t tm_to_time64(struct tm *tm, int use_utc __attribute__((unused))) {
    int year = tm->tm_year + 1900;
    int month = tm->tm_mon;
    int day = tm->tm_mday;
    
    // Validate
    if (month < 0 || month > 11) {
        return TIME64_ERROR;
    }
    
    int max_day = days_in_months[month];
    if (month == 1 && is_leap_year(year)) {
        max_day = 29;
    }
    
    if (day < 1 || day > max_day) {
        return TIME64_ERROR;
    }
    
    // Calculate days from epoch
    time64_t days = 0;
    
    if (year >= 1970) {
        // Forward from 1970
        for (int y = 1970; y < year; y++) {
            days += is_leap_year(y) ? 366 : 365;
        }
    } else {
        // Backward from 1970
        for (int y = year; y < 1970; y++) {
            days -= is_leap_year(y) ? 366 : 365;
        }
    }
    
    // Add days for months
    for (int m = 0; m < month; m++) {
        days += days_in_months[m];
        if (m == 1 && is_leap_year(year)) {
            days++;
        }
    }
    
    // Add day of month
    days += day - 1;
    
    // Calculate total seconds
    time64_t seconds = days * 86400LL;
    seconds += tm->tm_hour * 3600LL;
    seconds += tm->tm_min * 60LL;
    seconds += tm->tm_sec;
    
    return seconds;
}
