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

The application connects to `tcp://localhost:5555` and displays received messages in a ncurses terminal UI. Press 'q' to quit.

## Publisher Example

To test, you can use the `zmq_pub.py` Python publisher:

```bash
python3 zmq_pub.py
```
