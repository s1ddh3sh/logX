## logX - Async C++ logger
logX is an asynchronous logger library for C++ that writes log messages to a file using a background worker thread.

- Log levels (DEBUG, INFO, WARN, ERROR, NONE)
- Configurable output targets (file, terminal)
- Single-producer single-consumer lock-free queue

### Building and running logX
Run the `build.sh` script
```bash
chmod +x build.sh
./build.sh 
# or just bash build.sh
```
Run the `/build/logger` bin.
```bash
./build/logger
```
Check the generated log file (e.g. `app.log`) to see the output.

**Note:**  
Currently, the project is in testing mode.<br>
At present, only test messages are logged from `logX.cpp`. The logger API and core functionality are under active development.
