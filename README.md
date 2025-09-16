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

### Configuration
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


**Note:**  
Currently, the project is in testing mode.<br>
At present, only test messages are logged from `logX.cpp`. The logger API is under active development.
