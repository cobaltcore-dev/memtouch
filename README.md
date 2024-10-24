<!--
# SPDX-FileCopyrightText: Copyright 2025 SAP SE or an SAP affiliate company and cobaltcore-dev contributors
#
# SPDX-License-Identifier: Apache-2.0
-->

# memtouch

## About this project

Simple but configurable memory stresstest for UNIX-like systems to benchmark live
migration.

## How to Build

### Nix Toolchain

```bash
git checkout <this repo>
nix develop . # to get a toolchain to compile this regularly, or
nix build . # to build this directly in Nix
nix run . -- <args> # to run the artifact from Nix directly, e.g.: nix run -- --help
```

### Regular Toolchain

```bash
git checkout <this repo>
git submodule update --init
meson setup build
ninja -C build
./build/memtouch --help
```

## How to use

```bash
Usage: memtouch [--help] [--version] --thread_mem VAR --num_threads VAR --rw_ratio VAR [--random] [--stat_file VAR] [--stat_ival VAR]

The measurements are given in Mebibyte per second. This means that increasing or 
decreasing the interval has no effect on the read/write measurements, but only
increase the accuracy in terms of time resolution.

Optional arguments:
  -h, --help      shows help message and exits
  -v, --version   prints version information and exits
  --thread_mem    amount of memory a thread touches in MiB [required]
  --num_threads   number of worker threads [required]
  --rw_ratio      read/write ratio where 0 means only reads and 100 only writes [required]
  --stat_file     filepath where statistics are logged
  --stat_ival     interval for statistics logging in ms
  --page_log_ival log statistics after a specific number of pages have been read/written
```

### Example

```bash
$ ./memtouch --thread_mem 256 --num_threads 4 --rw_ratio 50 --stat_file ./stats.log --stat_ival 100
Running 4 threads touching 256 MB of memory
    memory consumption : 1024 MB
    access pattern     : sequential
    r/w ratio          : 50
    statistics file    : ./stats.log
    statistics interval: 100 ms

# The measurements are given in Mebibyte per second.
$ cat stats.log
2024-10-29T21:51:03.315+0100 read_mibps:16809.49 write_mibps:17834.99
2024-10-29T21:51:03.415+0100 read_mibps:15886.61 write_mibps:17992.63
2024-10-29T21:51:03.515+0100 read_mibps:16129.03 write_mibps:17834.99
```

### Hints

Ensure that `--thread_mem` is significantly higher than your L3 cache, if you
want to test the main memory. Otherwise, you are testing the caches.

## Support, Feedback, Contributing

This project is open to feature requests/suggestions, bug reports etc. via 
[GitHub issues](https://github.com/cobaltcore-dev/memtouch/issues). Contribution
and feedback are encouraged and always welcome. For more information about how
to contribute, the project structure, as well as additional contribution
information, see our [Contribution Guidelines](CONTRIBUTING.md).

## Security / Disclosure

Please do not create GitHub issues for security-related doubts or problems.
Instead, contact the maintainers by Email.

## Code of Conduct

We as members, contributors, and leaders pledge to make participation in our
community a harassment-free experience for everyone. By participating in this
project, you agree to abide by its [Code of Conduct](https://github.com/SAP/.github/blob/main/CODE_OF_CONDUCT.md)
at all times.

## Licensing

Copyright 2025 SAP SE or an SAP affiliate company and memtouch 
contributors. Please see our [LICENSE](LICENSE) for copyright and license 
information. Detailed information including third-party components and their
licensing/copyright information is available
[via the REUSE tool](https://api.reuse.software/info/github.com/cobaltcore-dev/memtouch).
