# Architecture Documentation

## Project Overview

**utime** is a simple command-line utility for bidirectional conversion between Unix timestamps and human-readable datetime formats. The project follows a minimalist design philosophy with a focus on correctness, portability, and ease of use.

## Design Principles

1. **Simplicity**: Single binary with automatic input detection
2. **Correctness**: Proper date validation including leap years and month-specific day counts
3. **Portability**: Pure C99, POSIX-compliant, minimal dependencies
4. **User-Friendly**: Clean CLI output, helpful error messages
5. **No artificial limits**: Uses system-native timestamp range

## Project Structure

```
utime/
├── include/
│   └── common.h           # Shared definitions and date validation
├── src/
│   └── utime.c            # Main program logic
├── tests/
│   └── test_utime.c       # Unit tests for validation functions
├── docs/
│   ├── API.md             # API documentation
│   └── ARCHITECTURE.md    # This file
├── meson.build            # Build configuration
├── LICENSE                # Unlicense (public domain)
└── README.md              # User documentation
```

## Module Breakdown

### 1. Common Header (`include/common.h`)

**Purpose**: Provides shared functionality for date validation.

**Components**:
- Version and author constants
- `is_leap_year()`: Gregorian calendar leap year detection
- `days_in_month()`: Returns day count for any month/year
- `validate_date()`: Comprehensive date validation

**Design Decisions**:
- Static inline functions for zero runtime overhead
- Pure functions (no side effects) for thread safety
- Header-only implementation (no separate compilation unit needed)

**Why inline?**
- These functions are called frequently during validation
- They're small (1-5 lines of actual logic)
- Inlining eliminates function call overhead

### 2. Main Program (`src/utime.c`)

**Purpose**: Command-line interface and conversion logic.

**Architecture**:

```
┌─────────────────────────────────────┐
│         Command Line Input           │
└──────────────┬──────────────────────┘
               │
               ▼
┌──────────────────────────────────────┐
│      Argument Parser (main)          │
│  - Parse flags (--utc, --help, etc)  │
│  - Detect input type (number vs date)│
└──────────────┬───────────────────────┘
               │
       ┌───────┴────────┐
       │                │
       ▼                ▼
┌──────────────┐ ┌──────────────────┐
│ Unix → Human │ │ Human → Unix     │
│              │ │                  │
│ strtoll()    │ │ parse_datetime() │
│    ↓         │ │        ↓         │
│ localtime()  │ │  validate_date() │
│  or gmtime() │ │        ↓         │
│    ↓         │ │    mktime()      │
│ strftime()   │ │  or timegm()     │
└──────┬───────┘ └────────┬─────────┘
       │                  │
       └────────┬─────────┘
                ▼
        ┌──────────────┐
        │ Print Result │
        └──────────────┘
```

**Key Functions**:

1. **`main()`**: Entry point
   - Parses command-line arguments
   - Detects input type (timestamp vs datetime string)
   - Routes to appropriate conversion function

2. **`parse_datetime()`**: DateTime parser
   - Uses `sscanf()` for parsing (safe, with format validation)
   - Validates all components
   - Populates `struct tm`

3. **`convert_human_to_unix()`**: Forward conversion
   - Parses datetime string
   - Calls `mktime()` or `timegm()`
   - Handles edge case of -1 return value

4. **`convert_unix_to_human()`**: Reverse conversion
   - Parses timestamp with `strtoll()` (64-bit safe)
   - Calls `localtime()` or `gmtime()`
   - Formats output with `strftime()`

### 3. Test Suite (`tests/test_utime.c`)

**Purpose**: Validates date calculation and validation logic.

**Test Categories**:
1. Leap year detection (boundary cases, century years)
2. Days per month (all 12 months, leap years)
3. Date validation (valid/invalid combinations)
4. Boundary cases (month transitions, century boundaries)

**Design**:
- Custom test framework (no external dependencies)
- Clear pass/fail reporting with colors
- Returns non-zero exit code on failure (CI-friendly)

## Data Flow

### Converting Human Time to Unix Timestamp

```
"2024-05-24 15:30:45"
         │
         ▼
   parse_datetime()
         │
         ├─ sscanf() → year, month, day, hour, minute, second
         │
         ├─ validate_date(year, month, day)
         │    │
         │    ├─ Check month range (1-12)
         │    ├─ days_in_month(month, year)
         │    │    │
         │    │    └─ is_leap_year(year) if month == 2
         │    │
         │    └─ Check day range
         │
         ▼
   struct tm populated
         │
         ▼
   mktime() or timegm()
         │
         ▼
   Unix timestamp (time_t)
         │
         ▼
   Print to stdout
```

### Converting Unix Timestamp to Human Time

```
"1716556245"
      │
      ▼
strtoll() with error checking
      │
      ├─ errno check
      ├─ endptr validation
      └─ Range check
      │
      ▼
Cast to time_t
      │
      ▼
localtime() or gmtime()
      │
      ├─ NULL check (out of range)
      │
      ▼
struct tm populated by system
      │
      ▼
strftime() with format string
      │
      ▼
Print to stdout
```

## Error Handling Strategy

### Validation Errors
- Detected early (during parsing)
- Clear error messages pointing to the problem
- Examples provided for correct format

### Conversion Errors
- `mktime()` return value checked (but -1 is ambiguous)
- `localtime()`/`gmtime()` NULL check (out of range)
- `strtoll()` errno check (overflow, invalid input)

### Error Reporting
- All errors go to `stderr`
- Exit code 1 on any error
- Exit code 0 only on success

## Portability Considerations

### POSIX Compliance
- Uses standard `time.h` functions
- Relies on POSIX `struct tm` layout
- Optional `timegm()` with fallback

### Platform Differences

**32-bit vs 64-bit**:
- Uses `strtoll()` to parse large timestamps
- Checks if parsed value fits in `time_t`
- Range depends on system's `time_t` size

**timegm() Availability**:
- Used for UTC conversion (GNU extension)
- Fallback: temporarily set `TZ=""` and use `mktime()`
- Defined by feature test macros

**Timezone Handling**:
- Respects system `TZ` environment variable
- DST handled via `tm_isdst = -1`
- UTC conversions are portable

### Compiler Compatibility
- C99 standard (widely supported)
- No GNU-specific extensions in core logic
- Works with GCC, Clang, MSVC (with C99 mode)

## Build System

**Meson** is used for the following reasons:

1. **Modern**: Better than Autotools, simpler than CMake
2. **Fast**: Ninja backend for parallel builds
3. **Clean**: Declarative syntax
4. **Cross-platform**: Works on Linux, BSD, macOS, Windows

**Build Configuration**:
```meson
project('utime', 'c', version: '2.0.0')
executable('utime', 'src/utime.c', include_directories: 'include')
```

**Warning Flags**:
- `-Wall -Wextra`: Catch common issues
- `-Werror` not used (too strict for portable code)
- Level 3 warnings in Meson

## Testing Strategy

### Unit Tests
- Focus on date validation logic
- Test boundary cases (leap years, month transitions)
- Automated via `meson test`

### Integration Tests
Not currently implemented, but could include:
- Roundtrip tests (human→unix→human)
- Timezone tests (local vs UTC)
- Edge cases (epoch, negative timestamps, year 2038)

### Manual Testing
- Different architectures (32-bit, 64-bit)
- Different timezones
- Different locales

## Performance Characteristics

### Time Complexity
- `parse_datetime()`: O(1) - fixed number of operations
- `validate_date()`: O(1) - simple arithmetic
- `mktime()`/`localtime()`: O(1) - system call
- Overall: O(1) per conversion

### Space Complexity
- Stack usage: < 1KB (local variables, buffers)
- No heap allocations
- No static buffers (except string literals)

### Benchmarks
Not critical for this application (single conversions), but typical performance:
- Human→Unix: < 1 microsecond
- Unix→Human: < 1 microsecond

## Future Considerations

### Possible Enhancements

1. **Additional Formats**:
   - ISO 8601 (2024-05-24T15:30:45Z)
   - RFC 2822 (Fri, 24 May 2024 15:30:45 +0000)
   - Relative times ("+1 day", "next week")

2. **Batch Processing**:
   - Read from stdin (one per line)
   - Process multiple files
   - CSV support

3. **Millisecond Precision**:
   - Use `struct timespec` instead of `time_t`
   - Support fractional seconds in input/output

4. **Timezone Specification**:
   - `--timezone=America/New_York`
   - Auto-detect from input string
   - TZ database integration

### Constraints
- Must remain dependency-free
- Must stay lightweight (< 50KB binary)
- Must compile with C99
- Must not require network access

## Security Considerations

### Input Validation
- All user input is validated before use
- No buffer overflows (sized buffers, bounds checking)
- No format string vulnerabilities (no user-controlled formats)

### Memory Safety
- No dynamic allocation (no malloc/free)
- Fixed-size buffers with overflow protection
- No pointer arithmetic on user data

### Attack Surface
- Minimal: only processes command-line arguments
- No network, no file I/O, no privileged operations
- Safe to run with untrusted input

## Maintenance

### Code Quality
- Clear function names
- Documented edge cases
- Comments explain "why", not "what"
- Consistent style (K&R-ish)

### Testing
- Run tests after every change
- Test on multiple platforms
- Use static analysis tools (cppcheck, clang-tidy)

### Version Management
- Semantic versioning (MAJOR.MINOR.PATCH)
- Update VERSION constant in common.h
- Document changes in README.md changelog

---

**Last Updated**: 2026-01-27  
**Version**: 2.0.0  
**Author**: AnmiTaliDev <anmitali198@gmail.com>
