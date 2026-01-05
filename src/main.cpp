#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

std::atomic<bool> running(true);

void message_receiver(zmq::context_t& context) {
    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5555");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    subscriber.set(zmq::sockopt::rcvtimeo, 500);

    std::cout << "Subscriber connected, waiting for messages..." << std::endl;

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

int main() {
    zmq::context_t context(1);
    std::thread receiver_thread(message_receiver, std::ref(context));

    std::cout << "Press 'q' to quit" << std::endl;

    while (true) {
        char ch = std::cin.get();
        if (ch == 'q' || ch == 'Q') {
            running.store(false);
            break;
        }
    }

    receiver_thread.join();
    return 0;
}
