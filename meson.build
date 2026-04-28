project(
  'memtouch',
  'cpp',
  # Version line must be a dedicated line so that it can be parsed from
  # flake.nix.
  version: '0.1.0',
  default_options : [
    'cpp_std=c++20',
    'warning_level=3', # roughly: -Wall -Wextra -Wpedantic
    'b_pie=true', # Enables Position Independent Executable (PIE)
    'b_lto=true', # Enables Link Time Optimization (LTO)
    'optimization=3'
  ]
)

cpp_flags = [
  '-Wall', '-Wextra', # Common warnings
  '-Wpedantic', '-Wshadow', # Extra strictness
  '-Wconversion', '-Wsign-conversion', # Handle implicit conversions
  '-Wformat=2', '-Wformat-security', # Format string security
  '-Wimplicit-fallthrough',
  '-Wcast-align',
  '-Wcast-qual',
  '-Wdisabled-optimization',
  '-Wduplicated-branches', '-Wduplicated-cond',
  '-Wlogical-op',
  '-Wnull-dereference',
  '-Wpointer-arith',
  '-Wshadow',
  '-Weffc++',
  '-Wswitch-enum',
  '-Wvla',
  '-Wuseless-cast',
  '-ffat-lto-objects', # Does not compile without this: Get error plugin required to handle lto
  '-fcf-protection=return',
  '-fstack-protector-strong', # Stack smashing protection
  '-fstack-clash-protection', # Prevent stack clash attacks
  '-U_FORTIFY_SOURCE', # Hardening some calls into GNU C lib
  '-D_FORTIFY_SOURCE=3', # Buffer overflow detection and more (gcc >=12 required)
  '-D_GLIBCXX_ASSERTIONS', # Run-time bounds checking for C++ strings and containers
  '-fvisibility=hidden', # Hide non-API symbols
  '-ffunction-sections', '-fdata-sections', # Allow garbage collection of unused code/data
  '-march=native', # Tune for host machine architecture
  '-DPROJECT_VERSION="' + meson.project_version() + '"'
]

# One of our dependencies needs the avx512f flag, but we don't want everything to
# be compiled with this flag
write_pages_avx512_lib_flags = cpp_flags
avx512_foundations = '-mavx512f'
if target_machine.cpu_family() == 'x86_64'
  write_pages_avx512_lib_flags += avx512_foundations
endif

# Linker flags
linker_flags = [
  '-Wl,-z,nodlopen', # Prevent dlopen()
  '-Wl,-z,noexecstack', # Harden stack
  '-Wl,-z,relro', # Harden relocations (read-only relocations)
  '-Wl,-z,now', # General harden
  '-Wl,--gc-sections', # Enable linker garbage collection
  '-Wl,--no-copy-dt-needed-entries',
]

# Shared constants
consts_lib = static_library(
  'consts',
  sources: ['consts.hpp'],
  cpp_args: cpp_flags,
  link_args: linker_flags
)

consts_lib_dep = declare_dependency(link_with: consts_lib)

# AVX512 write_pages implementation
write_pages_avx512_lib = static_library(
  'write_pages_avx512',
  sources : ['write_pages_avx512.cpp', 'write_pages_avx512.hpp'],
  cpp_args: write_pages_avx512_lib_flags,
  link_args: linker_flags,
  dependencies: [consts_lib_dep]
)

write_pages_avx512_lib_dep = declare_dependency(link_with: write_pages_avx512_lib)
# Add include directory as a system directory, so that lints/warnings do not
# run on them.
include_dir = include_directories('include', is_system: true)

executable(
  'memtouch',
  'memtouch.cpp',
  include_directories : include_dir,
  install : true,
  dependencies: [consts_lib_dep, write_pages_avx512_lib_dep],
  cpp_args : cpp_flags,
  link_args : linker_flags
)
