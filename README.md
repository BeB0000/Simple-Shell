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
  - `cat`
  - `mkdir`
  - `touch`
  - `rm`
  - `cp`
- Directory navigation and environment variable usage
- System information via `uname`, `sysinfo`, and `sysconf`
- Custom ASCII art branding

---

## Build

Requires a Unix-like system (Linux recommended) and GCC.

```sh
gcc shell.c -o shell -lreadline 
```
### or

You can make it with Makefile to run "shell" without './' like "./shell"

```sh
sudo Makefile install
```
---

## Run

```sh
./shell
```
### or

if you ran "sudo Makefile install"

```sh
shell
```
---

## Notes

This project is intentionally minimal and educational.
It is not intended to replace a full-featured shell.

---

## License

This projects is under MIT license
