# User Stories

## Overview

This document outlines the user stories for the ZMQ TUI Client and Publisher project - a real-time messaging system using ZMQ for transport and FlatBuffers for efficient binary serialization.

---

## User Story 1: Receive String Messages

**As a** developer
**I want to** receive and display plain string messages from a ZMQ publisher
**So that I can** monitor text-based data streams in a terminal UI

**Acceptance Criteria:**
- Application connects to `tcp://localhost:5555`
- Displays received messages with sequence numbering
- Shows timestamp for each message
- Clean ncurses-based terminal interface
- Press 'q' to quit gracefully

---

## User Story 2: Receive FlatBuffers Messages

**As a** developer
**I want to** receive and display structured FlatBuffers binary messages
**So that I can** handle high-performance, type-safe data streams

**Acceptance Criteria:**
- Application connects to `tcp://localhost:5556`
- Parses FlatBuffers-encoded messages
- Displays: ID, timestamp (seconds.nanoseconds), payload, priority, and message size
- Validates FlatBuffers buffer integrity
- Handles malformed messages gracefully

---

## User Story 3: Publish FlatBuffers Messages

**As a** developer
**I want to** publish structured data using FlatBuffers serialization
**So that I can** send efficiently serialized binary data over ZMQ

**Acceptance Criteria:**
- Publisher binds to `tcp://*:5556`
- Serializes data according to `schema/data.fbs` schema
- Sends messages with: sequence ID, timestamp, payload, and priority
- Automatic incrementing sequence numbers
- Configurable priority (1-10)
- Continuous publishing at configurable intervals

---

## User Story 4: Schema Compilation

**As a** developer
**I want to** compile FlatBuffers schema to C++ headers
**So that I can** use type-safe generated code in my publisher

**Acceptance Criteria:**
- Schema located at `schema/data.fbs`
- Compile with: `flatc --cpp schema/data.fbs`
- Generates `data_generated.h` in project root
- Generated code includes:
  - `Message` struct with id, timestamp, payload, priority fields
  - `Timestamp` struct with seconds and nanoseconds fields
  - Builder functions for message creation

---

## User Story 5: Build System

**As a** developer
**I want to** build all project executables with a single command
**So that I can** quickly compile and test the application

**Acceptance Criteria:**
- Single `cmake` and `make` build process
- Builds both `zmq_tui_app` (receiver) and `publisher`
- Automatic FlatBuffers header detection
- Links against ZMQ, ncurses, and pthread
- Build script (`build.sh`) installs all dependencies

---

## User Story 6: Dual Port Listening

**As a** user
**I want to** see messages from both string and FlatBuffers sources in one UI
**So that I can** monitor multiple data streams simultaneously

**Acceptance Criteria:**
- Single receiver application listens on two ports:
  - Port 5555: String messages (legacy/publisher)
  - Port 5556: FlatBuffers messages (new publisher)
- Threaded receiving for concurrent message handling
- Clear visual separation of message types
- Consistent message formatting

---

## User Story 7: Terminal UI Experience

**As a** user
**I want to** interact with a clean, responsive terminal UI
**So that I can** comfortably monitor messages over extended periods

**Acceptance Criteria:**
- ncurses-based interface
- Auto-refreshing display
- Clear message formatting with sequence numbers
- Smooth scrolling
- Responsive to keyboard input ('q' to quit)
- No flickering or display artifacts

---

## User Story 8: Cross-Platform Support

**As a** developer
**I want to** run this application on both Linux and macOS
**So that I can** use my preferred development platform

**Acceptance Criteria:**
- Ubuntu/Debian: `apt-get install libncurses5-dev libzmq3-dev cmake g++ libflatbuffers-dev flatbuffers-compiler`
- macOS: `brew install ncurses zeromq cmake flatbuffers`
- Same build process on both platforms
- Consistent behavior across platforms

---

## User Story 9: Error Handling

**As a** developer
**I want to** see meaningful error messages when something goes wrong
**So that I can** quickly diagnose and fix issues

**Acceptance Criteria:**
- ZMQ errors logged with descriptive messages
- Invalid FlatBuffers messages identified and reported
- Connection timeouts handled gracefully (no crashes)
- Clean shutdown on Ctrl+C or 'q' key
- No memory leaks on exit

---

## User Story 10: Performance Monitoring

**As a** developer
**I want to** see message statistics and throughput information
**So that I can** monitor the performance of the messaging system

**Acceptance Criteria:**
- Message counter for each received message
- Message size displayed for FlatBuffers messages
- Real-time display updates
- Optional: throughput (messages/second) calculation

---

## Future Enhancements

These user stories are planned for future iterations:

- **Topic-based filtering**: Subscribe to specific message types/topics
- **Message replay**: Replay historical messages from log
- **Remote publisher control**: Start/stop publisher remotely
- **Web interface**: Browser-based monitoring alternative
- **Message aggregation**: Aggregate multiple messages into batch displays
- **Configuration file**: JSON/YAML config instead of hardcoded values
- **Authentication**: Secure ZMQ with authentication mechanisms
- **Message compression**: Optional compression for large payloads

---

## Related Documents

- [Schema Definition](../schema/data.fbs)
- [Build Instructions](../README.md)
- [API Documentation](../docs/api.md) - *To be created*