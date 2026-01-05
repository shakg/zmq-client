#!/bin/bash
set -e

echo "Installing dependencies..."
sudo apt-get update
sudo apt-get install -y libncurses5-dev libzmq3-dev cmake g++ libflatbuffers-dev flatbuffers-compiler

echo "Building ZMQ TUI app..."
mkdir -p build
cd build
cmake ..
make

echo "Build complete! Run with: ./build/zmq_tui_app and ./build/publisher"
