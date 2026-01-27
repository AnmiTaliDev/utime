/* This software is released into the public domain.
 *
 * Do whatever you want with it. I don't give a shit.
 * - AnmiTaliDev <anmitali198@gmail.com>, 2026
 */

#ifndef UTIME_PARSER_H
#define UTIME_PARSER_H

#include <time.h>

/**
 * Parse datetime string in format "YYYY-MM-DD HH:MM:SS"
 * @param datetime_str Input string
 * @param tm Output struct tm
 * @param use_utc Whether to use UTC (currently unused in parsing)
 * @return 0 on success, -1 on error
 */
int parse_datetime(const char *datetime_str, struct tm *tm, int use_utc);

#endif /* UTIME_PARSER_H */
