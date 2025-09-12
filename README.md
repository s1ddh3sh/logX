## logX - Async C++ logger
logX is an asynchronous logger library for C++ that writes log messages to a file using a background worker thread.

- Log levels (DEBUG, INFO, WARN, ERROR, NONE)
- Single-producer single-consumer lock-free queue

### Building and running logX
```bash
make
./bin/logger
```
**Note:**  
Currently, the project is in testing mode.<br>
At present, only test messages are logged from `logX.cpp`. The logger API and core functionality are under active development.
