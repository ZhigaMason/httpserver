# HTTP Server

A multithreaded static HTTP server implemented in modern C++
Designed for **performance** and **simplicity**, it demonstrates how low-level networking and concurrency can be combined to serve files efficiently.

---

## Features
- **Multithreading** with a producer–consumer model (configurable worker pool).
- **Static file serving** from a user-specified directory.
- **Graceful shutdown** via `SIGTERM`.
- **Logging** to file or `STDOUT`.
- Lightweight, dependency-free (just POSIX APIs + C++ standard library).

---

## Example

```bash
# Serve files from ./static on port 8080 with 4 worker threads
./httpserver -d static -p 8080 -w 4
```

---

## Usage

```
Usage:
        httpserver [-d/--dir] <file directory> [-l/--log] <log file> [-p/--port] <port> [-w/--workers] <number of workers>

Options:
        -h/--help    - display this help menu

        -d/--dir     - file directory, where all of the served files could be found.
                                default: static

        -l/--log     - file where all logs will be dumped. If none is provided STDOUT is used.
                                default: logs to STDOUT

        -p/--port    - TCP port to create connection with.
                                default: 8080

        -w/--workers - number of worker threads serving files
                                default: 1
```

---

## Project Structure

```
.
├── cli
│   ├── argparse.cpp    # CLI options parsing
│   └── main.cpp        # CLI entry point
├── src
│   ├── request         # HTTP request parsing
│   ├── response        # HTTP response handling
│   ├── server          # server implementation
│   └── ...
├── ...
├── Makefile
└── README.md

11 directories, 11 files

```

---

## Build

### Requirments
- `clang++ >= 18` (or any C++20-compliant compiler)
- `make`
- POSIX system headers: `unistd.h`, `netinet/in.h`, `sys/socket.h`

```bash
make httpserver
```
