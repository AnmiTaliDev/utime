# API Documentation

## Common Header (`include/common.h`)

This header provides shared functionality for date/time validation and leap year calculations.

### Constants

```c
#define VERSION "2.0.0"
#define AUTHOR "AnmiTaliDev"
#define AUTHOR_EMAIL "anmitali198@gmail.com"
```

### Functions

#### `is_leap_year()`

```c
static inline int is_leap_year(int year)
```

Checks if a year is a leap year according to the Gregorian calendar rules.

**Parameters:**
- `year`: The year to check

**Returns:**
- `1` if the year is a leap year
- `0` if the year is not a leap year

**Algorithm:**
- A year is a leap year if it's divisible by 4
- Exception: years divisible by 100 are NOT leap years
- Exception to the exception: years divisible by 400 ARE leap years

**Examples:**
```c
is_leap_year(2024);  // Returns 1 (leap year)
is_leap_year(2023);  // Returns 0 (not a leap year)
is_leap_year(2000);  // Returns 1 (leap year - divisible by 400)
is_leap_year(1900);  // Returns 0 (not a leap year - divisible by 100 but not 400)
```

---

#### `days_in_month()`

```c
static inline int days_in_month(int month, int year)
```

Returns the number of days in a given month, accounting for leap years.

**Parameters:**
- `month`: Month number (1-12, where 1=January, 12=December)
- `year`: Year (for leap year calculation)

**Returns:**
- Number of days in the month (28-31)
- `0` if the month is invalid (< 1 or > 12)

**Examples:**
```c
days_in_month(1, 2024);   // Returns 31 (January)
days_in_month(2, 2024);   // Returns 29 (February in leap year)
days_in_month(2, 2023);   // Returns 28 (February in non-leap year)
days_in_month(4, 2024);   // Returns 30 (April)
days_in_month(13, 2024);  // Returns 0 (invalid month)
```

---

#### `validate_date()`

```c
static inline int validate_date(int year, int month, int day)
```

Validates a date, checking that the month and day are within valid ranges.

**Parameters:**
- `year`: Year
- `month`: Month (1-12)
- `day`: Day (1-31)

**Returns:**
- `0` if the date is valid
- `-1` if the date is invalid

**Validation Rules:**
- Month must be between 1 and 12
- Day must be between 1 and the maximum number of days for that month
- Accounts for leap years when validating February dates

**Examples:**
```c
validate_date(2024, 1, 15);   // Returns 0 (valid)
validate_date(2024, 2, 29);   // Returns 0 (valid - leap year)
validate_date(2023, 2, 29);   // Returns -1 (invalid - not a leap year)
validate_date(2024, 4, 31);   // Returns -1 (invalid - April has only 30 days)
validate_date(2024, 13, 1);   // Returns -1 (invalid month)
validate_date(2024, 1, 0);    // Returns -1 (invalid day)
```

---

## Main Program (`src/utime.c`)

### Command-Line Interface

```bash
utime [OPTIONS] <input>
```

**Options:**
- `-h, --help`: Display help message and exit
- `-v, --version`: Display version information and exit
- `--utc`: Use UTC timezone instead of local time

**Input Formats:**

1. **Unix Timestamp**: Integer value representing seconds since Unix epoch
   - Can be positive (dates after 1970-01-01) or negative (dates before 1970-01-01)
   - Example: `1716556245`, `0`, `-86400`

2. **Human-Readable Date**: String in format `YYYY-MM-DD HH:MM:SS`
   - Example: `"2024-05-24 15:30:45"`

The program automatically detects which format is provided.

### Exit Codes

- `0`: Success
- `1`: Error (invalid input, conversion failure, etc.)

### Functions

#### `print_version()`

```c
void print_version()
```

Prints version information including program version, copyright, and license.

---

#### `print_usage()`

```c
void print_usage(const char *program_name)
```

Prints detailed usage information including options, input formats, and examples.

**Parameters:**
- `program_name`: The name of the program (typically `argv[0]`)

---

#### `parse_datetime()`

```c
int parse_datetime(const char *datetime_str, struct tm *tm, int use_utc)
```

Parses a datetime string and validates it.

**Parameters:**
- `datetime_str`: Input string in format "YYYY-MM-DD HH:MM:SS"
- `tm`: Pointer to `struct tm` to fill with parsed values
- `use_utc`: Flag indicating UTC mode (currently unused in parsing)

**Returns:**
- `0` on success
- `-1` on parsing or validation failure

**Validation:**
- Hour: 0-23
- Minute: 0-59
- Second: 0-60 (60 for leap seconds)
- Date validation via `validate_date()` function

---

#### `convert_human_to_unix()`

```c
int convert_human_to_unix(const char *datetime_str, int use_utc)
```

Converts human-readable datetime to Unix timestamp and prints the result.

**Parameters:**
- `datetime_str`: Input datetime string
- `use_utc`: If non-zero, interpret input as UTC; otherwise use local time

**Returns:**
- `0` on success
- `1` on error

**Output:**
- Prints Unix timestamp (as long integer) to stdout

**Notes:**
- Uses `mktime()` for local time conversion
- Uses `timegm()` for UTC conversion (if available)
- Falls back to TZ environment variable manipulation if `timegm()` is not available

---

#### `convert_unix_to_human()`

```c
int convert_unix_to_human(const char *timestamp_str, int use_utc)
```

Converts Unix timestamp to human-readable format and prints the result.

**Parameters:**
- `timestamp_str`: String representation of Unix timestamp
- `use_utc`: If non-zero, output UTC time; otherwise use local time

**Returns:**
- `0` on success
- `1` on error

**Output:**
- Prints datetime in format "YYYY-MM-DD HH:MM:SS" to stdout
- Appends " UTC" to output if `use_utc` is true

**Notes:**
- Uses `strtoll()` for parsing (supports large timestamps on 64-bit systems)
- Uses `gmtime()` for UTC conversion
- Uses `localtime()` for local time conversion
- Checks if timestamp fits within `time_t` range

---

### Data Structures

The program uses standard POSIX `struct tm` for time representation:

```c
struct tm {
    int tm_sec;    /* Seconds (0-60) */
    int tm_min;    /* Minutes (0-59) */
    int tm_hour;   /* Hours (0-23) */
    int tm_mday;   /* Day of month (1-31) */
    int tm_mon;    /* Month (0-11) */
    int tm_year;   /* Years since 1900 */
    int tm_wday;   /* Day of week (0-6, Sunday = 0) */
    int tm_yday;   /* Day in year (0-365) */
    int tm_isdst;  /* Daylight saving time flag */
};
```

### Error Handling

The program provides descriptive error messages for common failure cases:

- Invalid datetime format
- Invalid Unix timestamp
- Timestamp out of range for the system
- Conversion failures

All error messages are written to `stderr`.

---

## Platform-Specific Notes

### Timestamp Range

The valid range of Unix timestamps depends on the size of `time_t`:

- **32-bit `time_t`**: -2147483648 to 2147483647 (1901-12-13 to 2038-01-19)
- **64-bit `time_t`**: Much larger range (approximately ±292 billion years)

Modern Linux systems (since kernel 5.6, glibc 2.34) use 64-bit `time_t` even on 32-bit architectures.

### Timezone Handling

- Local time conversions respect the system's `TZ` environment variable
- DST (Daylight Saving Time) is handled automatically via `tm_isdst = -1`
- UTC conversions are timezone-independent

### Leap Seconds

Standard POSIX time does not account for leap seconds. The program accepts seconds value of 60 in input (for leap second compatibility), but the underlying system functions may normalize it.

---

## Thread Safety

The common header functions (`is_leap_year`, `days_in_month`, `validate_date`) are thread-safe as they are pure functions with no shared state.

The main program functions use `gmtime()` and `localtime()`, which may not be thread-safe. For thread-safe versions, use `gmtime_r()` and `localtime_r()` instead (requires modifying the source code).

---

## Compilation

The program requires:
- C99 compiler
- POSIX-compatible system
- Standard C library with time.h support

Compile flags used:
```
-std=c99 -Wall -Wextra
```

---

## Examples

### Basic Usage

```bash
# Convert current time to Unix timestamp
$ utime "$(date '+%Y-%m-%d %H:%M:%S')"

# Convert Unix timestamp back
$ utime 1716556245

# Use UTC for both operations
$ utime --utc "2024-01-01 00:00:00"
$ utime --utc 1704067200
```

### Edge Cases

```bash
# Unix epoch
$ utime 0
1970-01-01 00:00:00

# Day before Unix epoch
$ utime -86400
1969-12-31 00:00:00

# Leap year date
$ utime "2024-02-29 12:00:00"
1709211600

# Century boundary
$ utime "2000-01-01 00:00:00"
946684800
```
