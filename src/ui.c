/* This software is released into the public domain.
 *
 * Do whatever you want with it. I don't give a shit.
 * - AnmiTaliDev <anmitali198@gmail.com>, 2026
 */

#include <stdio.h>
#include "../include/ui.h"
#include "../include/common.h"

/* ANSI color codes - subtle colors only */
#define COLOR_RESET   "\033[0m"
#define COLOR_DIM     "\033[2m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"

void print_version(void) {
    printf(COLOR_CYAN "utime" COLOR_RESET " (Universal Time Converter) version " COLOR_GREEN "%s" COLOR_RESET "\n", VERSION);
    printf("By %s <%s>, 2026\n", AUTHOR, AUTHOR_EMAIL);
    printf(COLOR_DIM "Do whatever you want with it. I don't give a shit." COLOR_RESET "\n");
    printf(COLOR_DIM "Released into the public domain." COLOR_RESET "\n");
}

void print_usage(const char *program_name) {
    printf("Usage: " COLOR_CYAN "%s" COLOR_RESET " [OPTIONS] <input>\n\n", program_name);
    
    printf("Universal Time Converter - Convert between different time formats.\n\n");
    
    printf(COLOR_YELLOW "OPTIONS:" COLOR_RESET "\n");
    printf("  -h, --help          Show this help message\n");
    printf("  -v, --version       Show version information\n");
    printf("  --utc               Use UTC instead of local time\n");
    printf("  -f, --format FORMAT Specify input/output format:\n");
    printf("                        " COLOR_GREEN "unix" COLOR_RESET "   - Unix timestamp (default, epoch 1970-01-01)\n");
    printf("                        " COLOR_GREEN "ntp" COLOR_RESET "    - NTP timestamp (epoch 1900-01-01)\n");
    printf("                        " COLOR_GREEN "apple" COLOR_RESET "  - Apple CFAbsoluteTime (epoch 2001-01-01)\n");
    printf("                        " COLOR_GREEN "jd" COLOR_RESET "     - Julian Day Number (epoch 4713 BCE)\n\n");
    
    printf(COLOR_YELLOW "INPUT FORMATS:" COLOR_RESET "\n");
    printf("  Timestamp:          Number (integer or decimal for Julian Day)\n");
    printf("  Human-readable:     YYYY-MM-DD HH:MM:SS\n\n");
    
    printf(COLOR_YELLOW "EXAMPLES:" COLOR_RESET "\n");
    printf("  # Convert human time to Unix timestamp\n");
    printf("  %s \"2024-05-24 15:30:45\"\n\n", program_name);
    
    printf("  # Convert Unix timestamp to human time\n");
    printf("  %s 1716556245\n\n", program_name);
    
    printf("  # Convert NTP timestamp to human time\n");
    printf("  %s -f ntp 3925544445\n\n", program_name);
    
    printf("  # Convert to Apple CFAbsoluteTime\n");
    printf("  %s -f apple \"2024-05-24 15:30:45\"\n\n", program_name);
    
    printf("  # Convert Julian Day to human time\n");
    printf("  %s -f jd 2460000.5\n\n", program_name);
    
    printf("  # Convert to Julian Day\n");
    printf("  %s -f jd \"2024-01-01 12:00:00\"\n\n", program_name);
    
    printf(COLOR_YELLOW "TIME FORMATS:" COLOR_RESET "\n");
    printf("  Unix:   Seconds since 1970-01-01 00:00:00 UTC\n");
    printf("  NTP:    Seconds since 1900-01-01 00:00:00 UTC\n");
    printf("  Apple:  Seconds since 2001-01-01 00:00:00 UTC\n");
    printf("  Julian: Days since noon, January 1, 4713 BCE (proleptic Julian calendar)\n\n");
    
    printf(COLOR_YELLOW "NOTE:" COLOR_RESET "\n");
    printf("  - Negative timestamps are supported (dates before epoch)\n");
    printf("  - Julian Day uses decimal notation (e.g., 2460000.5 = midnight)\n");
    printf("  - Full 64-bit range supported on all systems (including 32-bit)\n");
    printf("  - Supported range: year -292277022657 to +292277026596\n");
}
