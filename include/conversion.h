/* This software is released into the public domain.
 *
 * Do whatever you want with it. I don't give a shit.
 * - AnmiTaliDev <anmitali198@gmail.com>, 2026
 */

#ifndef UTIME_CONVERSION_H
#define UTIME_CONVERSION_H

#include "common.h"

/**
 * Convert Unix timestamp to NTP timestamp
 */
long long unix_to_ntp(long long unix_time);

/**
 * Convert Unix timestamp to Apple CFAbsoluteTime
 */
long long unix_to_apple(long long unix_time);

/**
 * Convert Unix timestamp to Julian Day
 */
double unix_to_julian(long long unix_time);

/**
 * Convert NTP timestamp to Unix timestamp
 */
long long ntp_to_unix(long long ntp_time);

/**
 * Convert Apple CFAbsoluteTime to Unix timestamp
 */
long long apple_to_unix(long long apple_time);

/**
 * Convert Julian Day to Unix timestamp
 */
long long julian_to_unix(double jd);

/**
 * Convert human-readable datetime to timestamp in specified format
 */
int convert_human_to_timestamp(const char *datetime_str, int use_utc, time_format_t format);

/**
 * Convert timestamp in specified format to human-readable datetime
 */
int convert_timestamp_to_human(const char *timestamp_str, int use_utc, time_format_t format);

#endif /* UTIME_CONVERSION_H */
