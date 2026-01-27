/* This software is released into the public domain.
 *
 * Do whatever you want with it. I don't give a shit.
 * - AnmiTaliDev <anmitali198@gmail.com>, 2026
 */

#include <stdio.h>
#include <string.h>
#include "../include/parser.h"
#include "../include/common.h"

int parse_datetime(const char *datetime_str, struct tm *tm, int use_utc __attribute__((unused))) {
    memset(tm, 0, sizeof(struct tm));
    
    int year, month, day, hour, minute, second;
    int parsed = sscanf(datetime_str, "%d-%d-%d %d:%d:%d", 
                       &year, &month, &day, &hour, &minute, &second);
    
    if (parsed != 6) {
        return -1;
    }
    
    // Validate ranges
    if (hour < 0 || hour > 23 ||
        minute < 0 || minute > 59 || 
        second < 0 || second > 60) {  // 60 for leap seconds
        return -1;
    }
    
    // Validate date with month-aware day checking
    if (validate_date(year, month, day) != 0) {
        return -1;
    }
    
    tm->tm_year = year - 1900;  // years since 1900
    tm->tm_mon = month - 1;     // months since January (0-11)
    tm->tm_mday = day;
    tm->tm_hour = hour;
    tm->tm_min = minute;
    tm->tm_sec = second;
    tm->tm_isdst = -1;          // let mktime() determine DST
    
    return 0;
}
