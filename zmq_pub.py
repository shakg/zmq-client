#!/usr/bin/env python3
import zmq
import time
import random

context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.bind("tcp://*:5555")

print("ZMQ Publisher running on tcp://*:5555")
print("Press Ctrl+C to stop")

try:
    while True:
        message = f"Message {random.randint(1, 1000)}: Data from publisher"
        socket.send_string(message)
        print(f"Sent: {message}")
        time.sleep(1)
except KeyboardInterrupt:
    print("\nStopping publisher...")
finally:
    socket.close()
    context.term()
