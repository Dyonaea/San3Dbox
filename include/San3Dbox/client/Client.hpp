#pragma once
#include <asio.hpp>
#include <thread>
#include <atomic>
#include <iostream>
#include <array>
#include <string>
#include "San3Dbox/interface/Window.hpp"
#include "San3Dbox/server/Server.hpp"

class Client {
public:
    Client(const std::string& host, unsigned short port);
    ~Client();

    void start();
    void stop();
    void send(const std::string& message);

private:
    void doReceive();

    asio::io_context io_context;
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint server_endpoint;
    std::thread ioThread;
    std::atomic<bool> running;
    std::array<char, 1024> buffer;

    std::thread interfaceThread;
    Window *window;

    SharedState *sharedState;

};
