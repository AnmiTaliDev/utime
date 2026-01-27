/* This software is released into the public domain.
 *
 * Do whatever you want with it. I don't give a shit.
 * - AnmiTaliDev <anmitali198@gmail.com>, 2026
 */

#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include "../include/conversion.h"
#include "../include/parser.h"

/* Convert Unix timestamp to other formats */
long long unix_to_ntp(long long unix_time) {
    return unix_time + SECONDS_1900_TO_1970;
}

long long unix_to_apple(long long unix_time) {
    return unix_time - SECONDS_1970_TO_2001;
}

double unix_to_julian(long long unix_time) {
    return JULIAN_DAY_UNIX_EPOCH + (double)unix_time / 86400.0;
}

/* Convert other formats to Unix timestamp */
long long ntp_to_unix(long long ntp_time) {
    return ntp_time - SECONDS_1900_TO_1970;
}

long long apple_to_unix(long long apple_time) {
    return apple_time + SECONDS_1970_TO_2001;
}

long long julian_to_unix(double jd) {
    return (long long)((jd - JULIAN_DAY_UNIX_EPOCH) * 86400.0);
}

int convert_human_to_timestamp(const char *datetime_str, int use_utc, time_format_t format) {
    struct tm tm;
    if (parse_datetime(datetime_str, &tm, use_utc) != 0) {
        fprintf(stderr, "Error: Invalid datetime format\n");
        fprintf(stderr, "Expected: YYYY-MM-DD HH:MM:SS\n");
        fprintf(stderr, "Example: \"2024-05-24 15:30:45\"\n");
        return 1;
    }
    
    // Use 64-bit time for all conversions
    time64_t unix_time = tm_to_time64(&tm, use_utc);
    
    if (unix_time == TIME64_ERROR) {
        fprintf(stderr, "Error: Failed to convert datetime\n");
        fprintf(stderr, "The date may be invalid\n");
        return 1;
    }
    
    // Convert to requested format
    switch (format) {
        case TIME_FORMAT_UNIX:
            printf("%lld\n", unix_time);
            break;
        case TIME_FORMAT_NTP:
            printf("%lld\n", unix_to_ntp(unix_time));
            break;
        case TIME_FORMAT_APPLE:
            printf("%lld\n", unix_to_apple(unix_time));
            break;
        case TIME_FORMAT_JULIAN:
            printf("%.6f\n", unix_to_julian(unix_time));
            break;
    }
    
    return 0;
}

int convert_timestamp_to_human(const char *timestamp_str, int use_utc, time_format_t format) {
    long long unix_time;
    
    // Parse based on format
    if (format == TIME_FORMAT_JULIAN) {
        // Julian Day is a decimal number
        char *endptr;
        errno = 0;
        double jd = strtod(timestamp_str, &endptr);
        
        if (errno != 0 || *endptr != '\0' || endptr == timestamp_str) {
            fprintf(stderr, "Error: Invalid Julian Day number\n");
            fprintf(stderr, "Must be a valid decimal number\n");
            fprintf(stderr, "Example: 2460000.5\n");
            return 1;
        }
        
        unix_time = julian_to_unix(jd);
    } else {
        // Other formats are integers
        char *endptr;
        errno = 0;
        long long timestamp_ll = strtoll(timestamp_str, &endptr, 10);
        
        if (errno != 0 || *endptr != '\0' || endptr == timestamp_str) {
            fprintf(stderr, "Error: Invalid timestamp\n");
            fprintf(stderr, "Must be a valid integer\n");
            fprintf(stderr, "Example: 1716556245\n");
            return 1;
        }
        
        // Convert to Unix time
        switch (format) {
            case TIME_FORMAT_UNIX:
                unix_time = timestamp_ll;
                break;
            case TIME_FORMAT_NTP:
                unix_time = ntp_to_unix(timestamp_ll);
                break;
            case TIME_FORMAT_APPLE:
                unix_time = apple_to_unix(timestamp_ll);
                break;
            default:
                unix_time = timestamp_ll;
                break;
        }
    }
    
    // Use 64-bit time conversion
    struct tm tm;
    if (time64_to_tm(unix_time, &tm, use_utc) != 0) {
        fprintf(stderr, "Error: Failed to convert timestamp\n");
        fprintf(stderr, "Value may be out of range\n");
        return 1;
    }
    
    char formatted_time[64];
    snprintf(formatted_time, sizeof(formatted_time), 
             "%04d-%02d-%02d %02d:%02d:%02d",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
    
    if (use_utc) {
        printf("%s UTC\n", formatted_time);
    } else {
        printf("%s\n", formatted_time);
    }
    
    return 0;
}
