#pragma once
#include <asio.hpp>
#include <thread>
#include <atomic>
#include <iostream>
#include <vector>
#include <mutex>
#include "San3Dbox/server/World.hpp"

struct SharedState {
    std::mutex mtx;
    std::vector<Player> players;
    bool updated = false;
};


class Server {
public:
    explicit Server(unsigned short port);
    ~Server();

    void start();
    void stop();

    void sendToAll(const std::string& message);

private:
    void doReceive();
    asio::io_context io_context;
    asio::ip::udp::socket socket;
    std::vector<asio::ip::udp::endpoint> clients;
    std::vector<std::thread> threads;
    std::atomic<bool> running;
    std::mutex clientsMutex;

    std::array<char, 1024> buffer;
    asio::ip::udp::endpoint remote_endpoint;

    World *world;
    std::thread worldThread;
    void updateClients(const std::vector<Player*>& players);
};
