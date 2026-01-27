# utime - Universal Time Converter

[![License: Unlicense](https://img.shields.io/badge/license-Unlicense-blue.svg)](http://unlicense.org/)
[![Version](https://img.shields.io/badge/version-2.1.0-blue.svg)](https://github.com/AnmiTaliDev/utime)

A simple, efficient command-line utility for converting between different time formats including Unix, NTP, Apple CFAbsoluteTime, Julian Day, and human-readable datetime.

## Features

- **Multiple time formats**:
  - **Unix** - Seconds since 1970-01-01 00:00:00 UTC
  - **NTP** - Seconds since 1900-01-01 00:00:00 UTC
  - **Apple CFAbsoluteTime** - Seconds since 2001-01-01 00:00:00 UTC
  - **Julian Day** - Days since noon, January 1, 4713 BCE
- **Bidirectional conversion**: Automatically detects input type and converts appropriately
- **UTC and local time support**: Use `--utc` flag for UTC timezone
- **Negative timestamp support**: Handle dates before epoch
- **Month-aware date validation**: Correctly validates dates including leap years
- **System-native range**: Uses maximum available range for your architecture
- **Clean CLI output**: No pseudographics, just clean text output
- **Zero dependencies**: Pure C99, no external libraries required

## Installation

### Building from source

```bash
# Clone the repository
git clone https://github.com/AnmiTaliDev/utime.git
cd utime

# Build with Meson
meson setup build
meson compile -C build

# Install (optional)
meson install -C build
```

### Requirements

- C99-compatible compiler (GCC, Clang, etc.)
- Meson build system (0.50.0 or later)

## Usage

```bash
utime [OPTIONS] <input>
```

### Options

- `-h, --help` - Show help message
- `-v, --version` - Show version information
- `--utc` - Use UTC instead of local time
- `-f, --format FORMAT` - Specify time format (unix, ntp, apple, jd)

### Input Formats

1. **Timestamp**: Number (integer or decimal for Julian Day)
2. **Human-readable**: `YYYY-MM-DD HH:MM:SS`

The program automatically detects which format you're using.

## Examples

### Unix Timestamp (default)

```bash
# Convert human time to Unix timestamp
$ utime "2024-05-24 15:30:45"
1716546645

# Convert Unix timestamp to human time
$ utime 1716546645
2024-05-24 15:30:45

# Unix epoch
$ utime 0
1970-01-01 00:00:00
```

### NTP Timestamp

```bash
# Convert to NTP timestamp
$ utime -f ntp "2024-05-24 15:30:45"
3925535445

# Convert NTP timestamp to human time
$ utime -f ntp 3925535445
2024-05-24 15:30:45

# NTP epoch (1900-01-01 00:00:00)
$ utime -f ntp 0
1900-01-01 00:00:00
```

### Apple CFAbsoluteTime

```bash
# Convert to Apple timestamp
$ utime -f apple "2024-05-24 15:30:45"
738239445

# Convert Apple timestamp to human time
$ utime -f apple 738239445
2024-05-24 15:30:45

# Apple epoch (2001-01-01 00:00:00)
$ utime -f apple 0
2001-01-01 00:00:00
```

### Julian Day

```bash
# Convert to Julian Day
$ utime -f jd "2024-05-24 15:30:45"
2460455.438021

# Convert Julian Day to human time
$ utime -f jd 2460455.438021
2024-05-24 15:30:45

# Y2K at noon
$ utime -f jd "2000-01-01 12:00:00"
2451545.250000
```

### UTC Mode

```bash
# Use UTC for all conversions
$ utime --utc 0
1970-01-01 00:00:00 UTC

$ utime --utc "2024-05-24 15:30:45"
1716556245
```

## Technical Details

### Timestamp Range

The valid timestamp range depends on your system architecture:

- **32-bit systems**: Typically `-2147483648` to `2147483647` (1901-12-13 to 2038-01-19)
- **64-bit systems**: Much larger range, effectively unlimited for practical purposes

### Negative Timestamps

Negative Unix timestamps represent dates before the Unix epoch (1970-01-01 00:00:00 UTC). For example:
- `-86400` = 1969-12-31 00:00:00 UTC
- `-31536000` = 1969-01-01 00:00:00 UTC

### Date Validation

The program includes month-aware date validation:
- Correctly handles February in leap years (29 days)
- Validates day ranges for each month (e.g., April has 30 days, not 31)
- Supports leap year calculation for any year

### Timezone Behavior

- **Without `--utc`**: Input/output uses local timezone
- **With `--utc`**: Input/output uses UTC timezone
- The program respects your system's timezone settings

## Testing

```bash
# Build and run tests
meson setup build -Dtests=true
meson test -C build
```

## License

**CopyDoWhateverYouWant License**

This software is released into the public domain. Do whatever you want with it. I don't give a shit.

By AnmiTaliDev <<anmitali198@gmail.com>>, 2026

See [LICENSE](LICENSE) file for the full (satirical) license text.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## Author

**AnmiTaliDev**
- Email: anmitali198@gmail.com
- GitHub: [@AnmiTaliDev](https://github.com/AnmiTaliDev)
