# ZMQ TUI Client

A C++ TUI application using ncurses to display data from a ZMQ publisher.

## Dependencies

Install the required development libraries:

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y libncurses5-dev libzmq3-dev cmake g++

# macOS
brew install ncurses zeromq cmake
```

## FlatBuffers Schema

The transmitter uses FlatBuffers for efficient binary serialization. The schema is defined in `schema/data.fbs`:

- **Message**: Contains sequence number, timestamp, payload string, and priority
- **Timestamp**: Seconds and nanoseconds since epoch

To compile the schema:
```bash
flatc --cpp schema/data.fbs
```

This generates `data_generated.h` which is included by the transmitter.

## Build

```bash
mkdir build
cd build
cmake ..
make
```

## Run

```bash
./zmq_tui_app
```

The TUI application connects to `tcp://localhost:5555` and displays received messages in a ncurses terminal UI. Press 'q' to quit.

## Transmitter

The FlatBuffers transmitter runs on port 5556:

```bash
./transmitter
```

## Publisher Example

To test, you can use the `zmq_pub.py` Python publisher:

```bash
python3 zmq_pub.py
```
