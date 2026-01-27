/* This software is released into the public domain.
 *
 * Do whatever you want with it. I don't give a shit.
 * - AnmiTaliDev <anmitali198@gmail.com>, 2026
 */

#ifndef UTIME_COMMON_H
#define UTIME_COMMON_H

#include <time.h>

#define VERSION "2.1.0"
#define AUTHOR "AnmiTaliDev"
#define AUTHOR_EMAIL "anmitali198@gmail.com"

/* Time format epochs */
#define UNIX_EPOCH_YEAR 1970
#define NTP_EPOCH_YEAR 1900
#define APPLE_EPOCH_YEAR 2001
#define JULIAN_DAY_EPOCH_YEAR -4713

/* Seconds between epochs */
#define SECONDS_1900_TO_1970 2208988800LL    /* NTP to Unix */
#define SECONDS_1970_TO_2001 978307200LL     /* Unix to Apple */
#define JULIAN_DAY_UNIX_EPOCH 2440588.0      /* JD of 1970-01-01 00:00:00 */

/* Time format identifiers */
typedef enum {
    TIME_FORMAT_UNIX,        /* Unix timestamp (1970-01-01) */
    TIME_FORMAT_NTP,         /* NTP timestamp (1900-01-01) */
    TIME_FORMAT_APPLE,       /* Apple CFAbsoluteTime (2001-01-01) */
    TIME_FORMAT_JULIAN       /* Julian Day Number */
} time_format_t;

/* 64-bit time type for systems with 32-bit time_t */
typedef long long time64_t;

/* Error value for time64_t functions */
#define TIME64_ERROR ((time64_t)(LLONG_MIN + 1))

/**
 * Convert time64_t to struct tm (works on 32-bit systems beyond 2038)
 * @param t Time in seconds since Unix epoch
 * @param tm Output struct tm
 * @param use_utc Whether to use UTC
 * @return 0 on success, -1 on error
 */
int time64_to_tm(time64_t t, struct tm *tm, int use_utc);

/**
 * Convert struct tm to time64_t (works on 32-bit systems beyond 2038)
 * @param tm Input struct tm
 * @param use_utc Whether to use UTC
 * @return Time in seconds since Unix epoch, or -1 on error
 */
time64_t tm_to_time64(struct tm *tm, int use_utc);

/**
 * Check if a year is a leap year
 */
static inline int is_leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/**
 * Get the number of days in a given month
 * @param month Month (1-12)
 * @param year Year (for leap year calculation)
 * @return Number of days in the month
 */
static inline int days_in_month(int month, int year) {
    static const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month < 1 || month > 12) {
        return 0;
    }
    
    if (month == 2 && is_leap_year(year)) {
        return 29;
    }
    
    return days[month - 1];
}

/**
 * Validate a date
 * @param year Year
 * @param month Month (1-12)
 * @param day Day (1-31)
 * @return 0 if valid, -1 if invalid
 */
static inline int validate_date(int year, int month, int day) {
    if (month < 1 || month > 12) {
        return -1;
    }
    
    int max_days = days_in_month(month, year);
    if (day < 1 || day > max_days) {
        return -1;
    }
    
    return 0;
}

#endif /* UTIME_COMMON_H */
