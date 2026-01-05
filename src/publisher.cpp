#include <zmq.hpp>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <cstdint>
#include <flatbuffers/flatbuffers.h>
#include "data_generated.h"

int main() {
    zmq::context_t context(1);
    zmq::socket_t publisher(context, zmq::socket_type::pub);
    publisher.set(zmq::sockopt::sndhwm, 0);
    publisher.bind("tcp://*:5555");

    std::cout << "ZMQ Publisher (FlatBuffers) running on tcp://*:5556" << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 1000);

    auto now = std::chrono::system_clock::now();
    auto now_sec = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    auto now_nanosec = static_cast<int32_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count() % 1000000000);

    int count = 0;
    while (true) {
        flatbuffers::FlatBufferBuilder builder;

        DataMessage::Timestamp timestamp(now_sec + count, now_nanosec);

        std::string payload = "FlatBuffers data packet #" + std::to_string(count);

        auto message = DataMessage::CreateMessageDirect(
            builder,
            count,
            &timestamp,
            payload.c_str(),
            (count % 10) + 1
        );

        builder.Finish(message);

        auto buffer = builder.GetBufferPointer();
        auto size = builder.GetSize();

        zmq::message_t zmq_msg(buffer, size);
        publisher.send(zmq_msg, zmq::send_flags::none);

        std::cout << "[" << count << "] Sent FlatBuffers message ("
                  << size << " bytes): " << payload
                  << " [priority: " << ((count % 10) + 1) << "]" << std::endl;

        count++;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
