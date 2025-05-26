#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ANSI Color Codes
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"

// Colors
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

// Bright colors
#define BRIGHT_RED     "\033[91m"
#define BRIGHT_GREEN   "\033[92m"
#define BRIGHT_YELLOW  "\033[93m"
#define BRIGHT_BLUE    "\033[94m"
#define BRIGHT_CYAN    "\033[96m"

void print_header() {
    printf("\n");
    printf(BRIGHT_CYAN "┌──────────────────────────────────────────┐\n" RESET);
    printf(BRIGHT_CYAN "│" BRIGHT_YELLOW "     Human Time to Unix Timestamp       " BRIGHT_CYAN "│\n" RESET);
    printf(BRIGHT_CYAN "│" DIM "           by AnmiTaliDev               " BRIGHT_CYAN "│\n" RESET);
    printf(BRIGHT_CYAN "└──────────────────────────────────────────┘\n" RESET);
    printf("\n");
}

void print_usage(const char *program_name) {
    print_header();
    
    printf(BOLD BLUE "USAGE:\n" RESET);
    printf("  %s " YELLOW "\"YYYY-MM-DD HH:MM:SS\"\n\n" RESET, program_name);
    
    printf(BOLD BLUE "EXAMPLES:\n" RESET);
    printf("  %s " CYAN "\"2024-05-24 15:30:45\"\n" RESET, program_name);
    printf("  %s " CYAN "\"2025-12-31 23:59:59\"\n" RESET, program_name);
    printf("  %s " CYAN "\"1970-01-01 00:00:00\"\n\n" RESET, program_name);
    
    printf(BOLD BLUE "FORMAT:\n" RESET);
    printf("  " YELLOW "YYYY-MM-DD HH:MM:SS\n" RESET);
    printf(DIM "  Where:\n" RESET);
    printf(DIM "    YYYY = Year (1900-2099)\n" RESET);
    printf(DIM "    MM   = Month (01-12)\n" RESET);
    printf(DIM "    DD   = Day (01-31)\n" RESET);
    printf(DIM "    HH   = Hour (00-23)\n" RESET);
    printf(DIM "    MM   = Minute (00-59)\n" RESET);
    printf(DIM "    SS   = Second (00-59)\n\n" RESET);
}

void print_error(const char *message) {
    printf(BRIGHT_RED "Error: " RESET "%s\n\n", message);
}

void print_result(const char *input_time, time_t unix_time) {
    printf("\n");
    printf(BRIGHT_GREEN "┌─ Conversion Result ─────────────────────┐\n" RESET);
    printf(BRIGHT_GREEN "│\n" RESET);
    printf(BRIGHT_GREEN "│ " RESET BOLD "Input:      " RESET CYAN "%s\n" RESET, input_time);
    printf(BRIGHT_GREEN "│ " RESET BOLD "Unix Time:  " RESET BRIGHT_YELLOW "%ld\n" RESET, unix_time);
    
    // Show UTC equivalent
    struct tm *utc_tm = gmtime(&unix_time);
    if (utc_tm != NULL) {
        char utc_str[64];
        strftime(utc_str, sizeof(utc_str), "%Y-%m-%d %H:%M:%S UTC", utc_tm);
        printf(BRIGHT_GREEN "│ " RESET BOLD "UTC:        " RESET MAGENTA "%s\n" RESET, utc_str);
    }
    
    printf(BRIGHT_GREEN "│\n" RESET);
    printf(BRIGHT_GREEN "└─────────────────────────────────────────┘\n" RESET);
    printf("\n");
}

int parse_datetime(const char *datetime_str, struct tm *tm) {
    memset(tm, 0, sizeof(struct tm));
    
    int year, month, day, hour, minute, second;
    int parsed = sscanf(datetime_str, "%d-%d-%d %d:%d:%d", 
                       &year, &month, &day, &hour, &minute, &second);
    
    if (parsed != 6) {
        return -1;
    }
    
    // Validate ranges
    if (year < 1900 || year > 2099 || 
        month < 1 || month > 12 || 
        day < 1 || day > 31 || 
        hour < 0 || hour > 23 ||
        minute < 0 || minute > 59 || 
        second < 0 || second > 59) {
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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    // Check for help flags
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_usage(argv[0]);
        return 0;
    }
    
    struct tm tm;
    if (parse_datetime(argv[1], &tm) != 0) {
        print_error("Invalid datetime format. Use: YYYY-MM-DD HH:MM:SS");
        printf(DIM "Example: \"2024-05-24 15:30:45\"\n\n" RESET);
        return 1;
    }
    
    time_t unix_time = mktime(&tm);
    if (unix_time == -1) {
        print_error("Failed to convert to Unix time. Check your date/time values.");
        return 1;
    }
    
    print_result(argv[1], unix_time);
    return 0;
}