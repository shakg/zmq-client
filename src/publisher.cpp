#include <zmq.hpp>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <random>

int main() {
    zmq::context_t context(1);
    zmq::socket_t publisher(context, zmq::socket_type::pub);
    publisher.set(zmq::sockopt::sndhwm, 0);
    publisher.bind("tcp://*:5555");

    std::cout << "ZMQ Publisher running on tcp://*:5555" << std::endl;
    std::cout << "Press Ctrl+C to stop" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 1000);

    int count = 0;
    while (true) {
        std::string message = "Message " + std::to_string(count++) + ": Data from C++ publisher";
        zmq::message_t msg(message.data(), message.size());
        publisher.send(msg, zmq::send_flags::none);

        std::cout << "Sent: " << message << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
