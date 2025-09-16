## logX - Async C++ logger
logX is an asynchronous logger library for C++ that writes log messages to a file using a background worker thread.

- Log levels (DEBUG, INFO, WARN, ERROR, NONE)
- Configurable output targets (file, terminal)
- Single-producer single-consumer lock-free queue

## Building and running logX:
Run the `build_and_test.sh` script
```bash
./build_and_test.sh
```
Run the `/build/example` bin.
```bash
./build/example
```
Check the generated log file (e.g. `app.log`) to see the output.

## Configuration:
The logger can be customized using config file `logger.conf`
This file contains the log level and output hooks
```ini
[logger]
level = INFO        # Possible values: DEBUG, INFO, WARN, ERROR, NONE
queue_size = 1024   # queue size

[hooks]
file = app.log      # File path for file hook
console = true      # Print logs to console
```
`level` = minimum level to record.

## Usage:
Use the per-level helpers:
```cpp
Logger logger("path_to_conf_file");
logger.INFO("App started");
logger.ERROR("File not found");
```
Refer `example.cpp` for example usage.

## Running Tests:
logX uses [Googletest (Gtest)](https://github.com/google/googletest) for unit testing.<br> (fetched automatically using cmake)
1. Build with provided script:
```bash
./build_and_test.sh
```
This will automatically build and run the test suite for you.

2. To run tests manually from the `build/` directory:
```bash
cd build
ctest --output-on-failure
```

**Note:**  
Currently, the project is under development.<br>
Try playing around with `example.cpp` for example usage. The public logger API is under active development.
