#include <ncurses.h>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstring>

std::atomic<bool> running(true);
std::string latest_message = "Waiting for messages...";

void message_receiver(zmq::context_t& context) {
    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5555");
    subscriber.set(zmq::sockopt::subscribe, "");

    subscriber.set(zmq::sockopt::rcvtimeo, 100);

    while (running.load()) {
        zmq::message_t message;
        try {
            auto result = subscriber.recv(message, zmq::recv_flags::none);
            if (result && *result > 0) {
                latest_message = std::string(
                    static_cast<const char*>(message.data()),
                    static_cast<size_t>(message.size())
                );
            }
        } catch (const zmq::error_t& e) {
            if (errno != EAGAIN && errno != ETIMEDOUT) {
                std::cerr << "ZMQ error: " << e.what() << std::endl;
            }
        }
    }
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    scrollok(stdscr, TRUE);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    zmq::context_t context(1);
    std::thread receiver_thread(message_receiver, std::ref(context));

    int y = 0;
    while (true) {
        clear();
        mvprintw(y++, 0, "ZMQ TUI Data Display");
        mvprintw(y++, 0, "========================");
        mvprintw(y++, 0, "Connected to: tcp://localhost:5555");
        mvprintw(y++, 0, "");
        mvprintw(y++, 0, "Latest Message:");
        mvprintw(y++, 0, latest_message.c_str());
        mvprintw(y++, 0, "");
        mvprintw(y++, 0, "Press 'q' to quit");

        refresh();

        int ch = getch();
        if (ch == 'q' || ch == 'Q') {
            running.store(false);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    receiver_thread.join();
    endwin();

    return 0;
}
