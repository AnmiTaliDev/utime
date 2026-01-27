/* This software is released into the public domain.
 *
 * Do whatever you want with it. I don't give a shit.
 * - AnmiTaliDev <anmitali198@gmail.com>, 2026
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../include/common.h"
#include "../include/conversion.h"
#include "../include/ui.h"

int main(int argc, char *argv[]) {
    // Warning for 32-bit systems
    #if LONG_MAX == 2147483647L
    static int warning_shown = 0;
    if (!warning_shown) {
        fprintf(stderr, "⚠ NOTE: Running on 32-bit system.\n");
        fprintf(stderr, "   Using software 64-bit time emulation.\n");
        fprintf(stderr, "   Dates beyond 2038 are supported, but may be slower.\n");
        fprintf(stderr, "   For best performance, consider upgrading to 64-bit.\n\n");
        warning_shown = 1;
    }
    #endif
    
    int use_utc = 0;
    time_format_t format = TIME_FORMAT_UNIX;
    const char *input = NULL;
    
    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version();
            return 0;
        } else if (strcmp(argv[i], "--utc") == 0) {
            use_utc = 1;
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--format") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr, "Error: -f/--format requires an argument\n");
                fprintf(stderr, "Run '%s --help' for usage information\n", argv[0]);
                return 1;
            }
            i++;
            if (strcmp(argv[i], "unix") == 0) {
                format = TIME_FORMAT_UNIX;
            } else if (strcmp(argv[i], "ntp") == 0) {
                format = TIME_FORMAT_NTP;
            } else if (strcmp(argv[i], "apple") == 0) {
                format = TIME_FORMAT_APPLE;
            } else if (strcmp(argv[i], "jd") == 0 || strcmp(argv[i], "julian") == 0) {
                format = TIME_FORMAT_JULIAN;
            } else {
                fprintf(stderr, "Error: Unknown format: %s\n", argv[i]);
                fprintf(stderr, "Valid formats: unix, ntp, apple, jd\n");
                return 1;
            }
        } else if (argv[i][0] == '-' && argv[i][1] != '\0' && (argv[i][1] < '0' || argv[i][1] > '9')) {
            fprintf(stderr, "Error: Unknown option: %s\n", argv[i]);
            fprintf(stderr, "Run '%s --help' for usage information\n", argv[0]);
            return 1;
        } else {
            if (input != NULL) {
                fprintf(stderr, "Error: Multiple inputs provided\n");
                fprintf(stderr, "Run '%s --help' for usage information\n", argv[0]);
                return 1;
            }
            input = argv[i];
        }
    }
    
    if (input == NULL) {
        print_usage(argv[0]);
        return 1;
    }
    
    // Determine if input is a timestamp or human-readable datetime
    int is_timestamp = 1;
    const char *p = input;
    
    if (*p == '-') {
        p++;
    }
    
    int has_dot = 0;
    while (*p) {
        if (*p == '.') {
            if (has_dot || format != TIME_FORMAT_JULIAN) {
                is_timestamp = 0;
                break;
            }
            has_dot = 1;
        } else if (*p < '0' || *p > '9') {
            is_timestamp = 0;
            break;
        }
        p++;
    }
    
    if (is_timestamp && p > input && (p > input + 1 || input[0] != '-')) {
        return convert_timestamp_to_human(input, use_utc, format);
    } else {
        return convert_human_to_timestamp(input, use_utc, format);
    }
}
