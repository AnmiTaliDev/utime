# Building utime

This document describes how to build utime from source.

## Prerequisites

### Required

- **C Compiler**: GCC 4.9+, Clang 3.5+, or any C99-compatible compiler
- **Meson**: 0.50.0 or later
- **Ninja**: Build backend (usually installed with Meson)

### Installation

#### Arch Linux / Manjaro

```bash
sudo pacman -S meson ninja gcc
```

#### Ubuntu / Debian

```bash
sudo apt install meson ninja-build gcc
```

#### Fedora / RHEL

```bash
sudo dnf install meson ninja-build gcc
```

#### macOS

```bash
brew install meson ninja gcc
```

## Building

### Quick Build

```bash
# Clone the repository (if not already done)
git clone https://github.com/AnmiTaliDev/utime.git
cd utime

# Setup build directory
meson setup build

# Compile
meson compile -C build

# The binary is now at: build/utime
```

### Build with Tests

```bash
# Setup build with tests enabled
meson setup build -Dtests=true

# Compile
meson compile -C build

# Run tests
meson test -C build

# Or run tests directly to see detailed output
./build/test_utime
```

### Release Build

For optimized production builds:

```bash
meson setup build --buildtype=release
meson compile -C build
```

### Debug Build

For development with debug symbols:

```bash
meson setup build --buildtype=debug
meson compile -C build
```

## Installation

### System-wide Installation

```bash
# Build first
meson setup build --prefix=/usr
meson compile -C build

# Install (requires sudo)
sudo meson install -C build
```

The binary will be installed to `/usr/bin/utime`.

### User Installation

For installation in your home directory:

```bash
meson setup build --prefix=$HOME/.local
meson compile -C build
meson install -C build
```

Make sure `$HOME/.local/bin` is in your `PATH`.

### Custom Installation Path

```bash
meson setup build --prefix=/opt/utime
meson compile -C build
sudo meson install -C build
```

## Build Options

### Available Options

- `tests`: Build and enable tests (default: `false`)

### Setting Options

```bash
# During setup
meson setup build -Dtests=true

# Reconfigure existing build
meson configure build -Dtests=true
```

## Cleaning

### Clean Build Artifacts

```bash
ninja -C build clean
```

### Remove Build Directory

```bash
rm -rf build
```

## Cross-Compilation

### For 32-bit on 64-bit Linux

Create a cross-file `cross-32bit.txt`:

```ini
[binaries]
c = 'gcc'
ar = 'ar'
strip = 'strip'

[properties]

[host_machine]
system = 'linux'
cpu_family = 'x86'
cpu = 'i686'
endian = 'little'
```

Build:

```bash
meson setup build --cross-file cross-32bit.txt
meson compile -C build
```

### For ARM

Create `cross-arm.txt`:

```ini
[binaries]
c = 'arm-linux-gnueabihf-gcc'
ar = 'arm-linux-gnueabihf-ar'
strip = 'arm-linux-gnueabihf-strip'

[host_machine]
system = 'linux'
cpu_family = 'arm'
cpu = 'armv7l'
endian = 'little'
```

Build:

```bash
meson setup build --cross-file cross-arm.txt
meson compile -C build
```

## Troubleshooting

### Meson Not Found

```bash
# Install via pip (Python package manager)
pip3 install --user meson ninja
```

Add `~/.local/bin` to PATH if needed.

### Compilation Errors

If you see compilation errors, ensure:

1. You have a C99-compatible compiler
2. Standard C library headers are available
3. POSIX headers are available (Linux, macOS, *BSD)

### Test Failures

If tests fail:

```bash
# Run tests with verbose output
meson test -C build -v

# Run test binary directly
./build/test_utime
```

## Build Artifacts

After a successful build:

```
build/
├── utime              # Main executable
├── test_utime         # Test executable (if tests enabled)
├── compile_commands.json
└── meson-logs/
    ├── meson-log.txt
    └── testlog.txt    # Test results (if tests run)
```

## Static Linking

For a fully static binary (no dynamic dependencies):

```bash
# Note: This may not work on all systems
LDFLAGS="-static" meson setup build
meson compile -C build
```

Check dependencies:

```bash
ldd build/utime
# Output should show "not a dynamic executable" if fully static
```

## Performance

### Binary Size

Typical binary sizes:

- Debug build: ~60-80 KB
- Release build: ~20-30 KB
- Stripped release: ~15-20 KB

### Strip Binary

To reduce size further:

```bash
strip build/utime
```

## Platform-Specific Notes

### Linux

Works on all major distributions. Tested on:
- Arch Linux
- Ubuntu 20.04+
- Debian 11+
- Fedora 35+
- RHEL/CentOS 8+

### macOS

Requires Xcode Command Line Tools:

```bash
xcode-select --install
```

### BSD

Works on FreeBSD, OpenBSD, NetBSD with minor adjustments.

### Windows

Not currently supported. Use WSL (Windows Subsystem for Linux) instead.

## Development

### Code Style

The project uses K&R-style C code formatting:
- 4 spaces for indentation
- Opening braces on same line (except functions)
- 80-character line limit (soft)

### Static Analysis

```bash
# Using cppcheck
cppcheck --enable=all src/

# Using clang-tidy
clang-tidy src/utime.c -- -I include
```

### Compiler Warnings

Build with extra warnings:

```bash
CFLAGS="-Wall -Wextra -Wpedantic -Werror" meson setup build
meson compile -C build
```

## Continuous Integration

For CI/CD pipelines:

```bash
#!/bin/bash
set -e

# Setup and build
meson setup build -Dtests=true --werror
meson compile -C build

# Run tests
meson test -C build --print-errorlogs

# Check installation
DESTDIR=/tmp/install meson install -C build
```

## Getting Help

If you encounter build issues:

1. Check this document
2. Read error messages carefully
3. Verify prerequisites are installed
4. Open an issue at: https://github.com/AnmiTaliDev/utime/issues

---

**Last Updated**: 2026-01-27  
**Version**: 2.0.0
