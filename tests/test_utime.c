/* This is free and unencumbered software released into the public domain.
 *
 * Copyright 2026 AnmiTaliDev <anmitali198@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/common.h"

/* Color codes for test output */
#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

int tests_passed = 0;
int tests_failed = 0;

void test_assert(const char *test_name, int condition) {
    if (condition) {
        printf(GREEN "✓" RESET " %s\n", test_name);
        tests_passed++;
    } else {
        printf(RED "✗" RESET " %s\n", test_name);
        tests_failed++;
    }
}

void test_is_leap_year() {
    printf("\n=== Testing is_leap_year ===\n");
    
    test_assert("2000 is leap year", is_leap_year(2000) == 1);
    test_assert("2004 is leap year", is_leap_year(2004) == 1);
    test_assert("2024 is leap year", is_leap_year(2024) == 1);
    test_assert("1900 is not leap year", is_leap_year(1900) == 0);
    test_assert("2100 is not leap year", is_leap_year(2100) == 0);
    test_assert("2001 is not leap year", is_leap_year(2001) == 0);
    test_assert("2023 is not leap year", is_leap_year(2023) == 0);
}

void test_days_in_month() {
    printf("\n=== Testing days_in_month ===\n");
    
    /* Regular year */
    test_assert("January has 31 days", days_in_month(1, 2023) == 31);
    test_assert("February has 28 days (non-leap)", days_in_month(2, 2023) == 28);
    test_assert("March has 31 days", days_in_month(3, 2023) == 31);
    test_assert("April has 30 days", days_in_month(4, 2023) == 30);
    test_assert("May has 31 days", days_in_month(5, 2023) == 31);
    test_assert("June has 30 days", days_in_month(6, 2023) == 30);
    test_assert("July has 31 days", days_in_month(7, 2023) == 31);
    test_assert("August has 31 days", days_in_month(8, 2023) == 31);
    test_assert("September has 30 days", days_in_month(9, 2023) == 30);
    test_assert("October has 31 days", days_in_month(10, 2023) == 31);
    test_assert("November has 30 days", days_in_month(11, 2023) == 30);
    test_assert("December has 31 days", days_in_month(12, 2023) == 31);
    
    /* Leap year */
    test_assert("February has 29 days (leap)", days_in_month(2, 2024) == 29);
    test_assert("February has 29 days (2000)", days_in_month(2, 2000) == 29);
    test_assert("February has 28 days (1900)", days_in_month(2, 1900) == 28);
    
    /* Invalid months */
    test_assert("Month 0 returns 0", days_in_month(0, 2023) == 0);
    test_assert("Month 13 returns 0", days_in_month(13, 2023) == 0);
    test_assert("Month -1 returns 0", days_in_month(-1, 2023) == 0);
}

void test_validate_date() {
    printf("\n=== Testing validate_date ===\n");
    
    /* Valid dates */
    test_assert("Valid: 2024-01-15", validate_date(2024, 1, 15) == 0);
    test_assert("Valid: 2024-12-31", validate_date(2024, 12, 31) == 0);
    test_assert("Valid: 2024-02-29 (leap)", validate_date(2024, 2, 29) == 0);
    test_assert("Valid: 2000-02-29 (leap)", validate_date(2000, 2, 29) == 0);
    test_assert("Valid: 2024-04-30", validate_date(2024, 4, 30) == 0);
    
    /* Invalid dates */
    test_assert("Invalid: 2023-02-29 (non-leap)", validate_date(2023, 2, 29) == -1);
    test_assert("Invalid: 1900-02-29 (non-leap)", validate_date(1900, 2, 29) == -1);
    test_assert("Invalid: 2024-04-31 (April has 30)", validate_date(2024, 4, 31) == -1);
    test_assert("Invalid: 2024-06-31 (June has 30)", validate_date(2024, 6, 31) == -1);
    test_assert("Invalid: 2024-11-31 (November has 30)", validate_date(2024, 11, 31) == -1);
    test_assert("Invalid: 2024-02-30", validate_date(2024, 2, 30) == -1);
    test_assert("Invalid: 2024-00-15 (month 0)", validate_date(2024, 0, 15) == -1);
    test_assert("Invalid: 2024-13-15 (month 13)", validate_date(2024, 13, 15) == -1);
    test_assert("Invalid: 2024-01-00 (day 0)", validate_date(2024, 1, 0) == -1);
    test_assert("Invalid: 2024-01-32 (day 32)", validate_date(2024, 1, 32) == -1);
    
    /* Edge cases */
    test_assert("Valid: 1970-01-01 (Unix epoch)", validate_date(1970, 1, 1) == 0);
    test_assert("Valid: 1900-01-01", validate_date(1900, 1, 1) == 0);
    test_assert("Valid: 2099-12-31", validate_date(2099, 12, 31) == 0);
    test_assert("Valid: 9999-12-31", validate_date(9999, 12, 31) == 0);
}

void test_version_info() {
    printf("\n=== Testing version info ===\n");
    
    test_assert("VERSION defined", VERSION != NULL);
    test_assert("AUTHOR defined", AUTHOR != NULL);
    test_assert("AUTHOR_EMAIL defined", AUTHOR_EMAIL != NULL);
    test_assert("VERSION is 2.0.0", strcmp(VERSION, "2.0.0") == 0);
    test_assert("AUTHOR is AnmiTaliDev", strcmp(AUTHOR, "AnmiTaliDev") == 0);
}

void test_boundary_cases() {
    printf("\n=== Testing boundary cases ===\n");
    
    /* Test all months for day 31 */
    test_assert("Jan 31 valid", validate_date(2024, 1, 31) == 0);
    test_assert("Feb 31 invalid", validate_date(2024, 2, 31) == -1);
    test_assert("Mar 31 valid", validate_date(2024, 3, 31) == 0);
    test_assert("Apr 31 invalid", validate_date(2024, 4, 31) == -1);
    test_assert("May 31 valid", validate_date(2024, 5, 31) == 0);
    test_assert("Jun 31 invalid", validate_date(2024, 6, 31) == -1);
    test_assert("Jul 31 valid", validate_date(2024, 7, 31) == 0);
    test_assert("Aug 31 valid", validate_date(2024, 8, 31) == 0);
    test_assert("Sep 31 invalid", validate_date(2024, 9, 31) == -1);
    test_assert("Oct 31 valid", validate_date(2024, 10, 31) == 0);
    test_assert("Nov 31 invalid", validate_date(2024, 11, 31) == -1);
    test_assert("Dec 31 valid", validate_date(2024, 12, 31) == 0);
    
    /* Century leap years */
    test_assert("1600-02-29 valid (leap)", validate_date(1600, 2, 29) == 0);
    test_assert("1700-02-29 invalid (not leap)", validate_date(1700, 2, 29) == -1);
    test_assert("1800-02-29 invalid (not leap)", validate_date(1800, 2, 29) == -1);
    test_assert("1900-02-29 invalid (not leap)", validate_date(1900, 2, 29) == -1);
    test_assert("2000-02-29 valid (leap)", validate_date(2000, 2, 29) == 0);
}

int main() {
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║      utime Test Suite v%s         ║\n", VERSION);
    printf("╚════════════════════════════════════════╝\n");
    
    test_is_leap_year();
    test_days_in_month();
    test_validate_date();
    test_version_info();
    test_boundary_cases();
    
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║           Test Results                 ║\n");
    printf("╠════════════════════════════════════════╣\n");
    printf("║ " GREEN "Passed:" RESET " %-5d                        ║\n", tests_passed);
    printf("║ " RED "Failed:" RESET " %-5d                        ║\n", tests_failed);
    printf("║ Total:  %-5d                        ║\n", tests_passed + tests_failed);
    printf("╚════════════════════════════════════════╝\n");
    printf("\n");
    
    if (tests_failed > 0) {
        printf(RED "Some tests failed!\n" RESET);
        return 1;
    } else {
        printf(GREEN "All tests passed!\n" RESET);
        return 0;
    }
}
