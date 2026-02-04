# BeboShell

BeboShell is a minimal Unix-like shell written in C, built to explore low-level
systems programming, POSIX APIs, and terminal interaction.

This project is designed as a learning-focused shell implementation, gradually
expanding toward real shell behavior while remaining simple and readable.

---

## Features

- Interactive command prompt with ANSI colors
- Built-in commands:
  - `cd` / `goto`
  - `ls` / `peek`
  - `pwd` / `path`
  - `echo`
  - `whoami`
  - `clear` / `wipe`
  - `fetch` (system information)
  - `logo`
- Directory navigation and environment variable usage
- System information via `uname`, `sysinfo`, and `sysconf`
- Custom ASCII art branding

---

## Build

Requires a Unix-like system (Linux recommended) and GCC.

---

## License

This projects is under MIT license

```sh
gcc src/main.c -o bebo
