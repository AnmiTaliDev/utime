project('utime', 'c',
  version: '2.1.0',
  license: 'Public Domain',
  default_options: [
    'warning_level=3',
    'c_std=c99'
  ]
)

# Include directories
inc = include_directories('include')

# Source files
sources = files(
  'src/main.c',
  'src/conversion.c',
  'src/parser.c',
  'src/ui.c',
  'src/time64.c'
)

# Main executable
executable('utime',
  sources,
  include_directories: inc,
  install: true
)

# Tests
if get_option('tests')
  test_exe = executable('test_utime',
    'tests/test_utime.c',
    include_directories: inc,
    build_by_default: false
  )

  test('basic_tests', test_exe)
endif
