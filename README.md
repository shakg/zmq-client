# ZMQ TUI Client and Publisher

A C++ TUI application using ncurses to display data from a ZMQ publisher, with a FlatBuffers-enabled publisher.

## Dependencies

Install the required development libraries:

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y libncurses5-dev libzmq3-dev cmake g++ libflatbuffers-dev flatbuffers-compiler

# macOS
brew install ncurses zeromq cmake flatbuffers
```

## FlatBuffers Schema

The publisher uses FlatBuffers for efficient binary serialization. The schema is defined in `schema/data.fbs`:

- **Message**: Contains sequence number, timestamp, payload string, and priority
- **Timestamp**: Seconds and nanoseconds since epoch

To compile the schema:
```bash
flatc --cpp schema/data.fbs
```

This generates `data_generated.h` which is included by the publisher.

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

## Publisher

The C++ FlatBuffers publisher runs on port 5556:

```bash
./publisher
```

The receiver (zmq_tui_app) listens on both:
- Port 5555: String messages (original)
- Port 5556: FlatBuffers messages (new)
