#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <flatbuffers/flatbuffers.h>
#include "data_generated.h"

std::atomic<bool> running(true);

void string_receiver(zmq::context_t& context) {
    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5556");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    subscriber.set(zmq::sockopt::rcvtimeo, 500);

    std::cout << "String receiver connected to tcp://localhost:5556, waiting for messages..." << std::endl;

    int count = 0;
    while (running.load()) {
        zmq::message_t message;
        try {
            auto result = subscriber.recv(message, zmq::recv_flags::none);
            if (result && *result > 0) {
                count++;
                std::string msg(static_cast<const char*>(message.data()), message.size());
                std::cout << "[" << count << "] Received: " << msg << std::endl;
            }
        } catch (const zmq::error_t& e) {
            if (errno != EAGAIN && errno != ETIMEDOUT) {
                std::cerr << "ZMQ error: " << e.what() << std::endl;
            }
        }
    }
}

void flatbuffers_receiver(zmq::context_t& context) {
    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5555");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    subscriber.set(zmq::sockopt::rcvtimeo, 500);

    std::cout << "FlatBuffers receiver connected to tcp://localhost:5555, waiting for telemetry messages..." << std::endl;

    int count = 0;
    while (running.load()) {
        zmq::message_t message;
        try {
            auto result = subscriber.recv(message, zmq::recv_flags::none);
            if (result && *result > 0) {
                count++;

                const uint8_t* buffer = static_cast<const uint8_t*>(message.data());
                size_t size = message.size();

                auto verifier = flatbuffers::Verifier(buffer, size);
                if (verifier.VerifyBuffer<DataMessage::Message>()) {
                    auto msg = DataMessage::GetMessage(buffer);

                    std::cout << "[" << count << "] Received FlatBuffers telemetry message:" << std::endl;
                    std::cout << "  ID: " << msg->id() << std::endl;
                    std::cout << "  Timestamp: " << msg->timestamp()->seconds()
                              << "." << std::setw(9) << std::setfill('0') << msg->timestamp()->nanoseconds() << std::endl;
                    std::cout << "  Payload: " << msg->payload()->c_str() << std::endl;
                    std::cout << "  Priority: " << msg->priority() << std::endl;
                    std::cout << "  Size: " << size << " bytes" << std::endl;
                } else {
                    std::cerr << "[" << count << "] Invalid FlatBuffers message" << std::endl;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        } catch (const zmq::error_t& e) {
            if (errno != EAGAIN && errno != ETIMEDOUT) {
                std::cerr << "ZMQ error: " << e.what() << std::endl;
            }
        }
    }
}

int main() {
    std::cout << "ZMQ Receiver" << std::endl;
    std::cout << "============" << std::endl;

    zmq::context_t context(1);
    std::thread string_thread(string_receiver, std::ref(context));
    std::thread flatbuffers_thread(flatbuffers_receiver, std::ref(context));

    std::cout << "Press 'q' to quit" << std::endl;

    while (true) {
        char ch = std::cin.get();
        if (ch == 'q' || ch == 'Q') {
            running.store(false);
            break;
        }
    }

    string_thread.join();
    flatbuffers_thread.join();
    return 0;
}