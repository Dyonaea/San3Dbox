#include "San3Dbox/server/Server.hpp"
#include <chrono>

Server::Server(unsigned short port)
    : socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
      running(false) {
        world = new World();
      }

Server::~Server() {
    stop();
    delete world;
}

void Server::start() {
    running = true;
    doReceive();

    worldThread = std::thread([this]() {
        using namespace std::chrono_literals;
        while(1){
            auto start = std::chrono::steady_clock::now();
            world->run();
            auto players = world->players;
            updateClients(world->players);
            std::this_thread::sleep_until(start + 16ms);
        }
    });

    unsigned int n_threads = std::thread::hardware_concurrency();
    if (n_threads == 0) n_threads = 2;

    for (unsigned int i = 0; i < n_threads; ++i) {
        threads.emplace_back([this]() { io_context.run(); });
    }

    std::cout << "[Server] Running on port " << socket.local_endpoint().port() << std::endl;
}

void Server::stop() {
    running = false;
    io_context.stop();

    for (auto& t : threads)
        if (t.joinable())
            t.join();
    if (worldThread.joinable())
        worldThread.join();

    std::cout << "[Server] Stopped." << std::endl;
}

void Server::doReceive() {
    socket.async_receive_from(
        asio::buffer(buffer), remote_endpoint,
        [this](std::error_code ec, std::size_t bytes) {
            if (!ec && running) {
                std::string msg(buffer.data(), bytes);
                std::cout << "[Client] " << remote_endpoint.address().to_string()
                          << ":" << remote_endpoint.port() << " -> " << msg << std::endl;

                {
                    std::lock_guard<std::mutex> lock(clientsMutex);
                    auto it = std::find(clients.begin(), clients.end(), remote_endpoint);
                    if (it == clients.end())
                        clients.push_back(remote_endpoint);
                }

                sendToAll(msg);
                doReceive();
            }
        });
}

void Server::sendToAll(const std::string& message) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto& client : clients) {
        socket.async_send_to(asio::buffer(message), client,
            [](std::error_code ec, std::size_t) {
                if (ec)
                    std::cerr << "[Server] Send error: " << ec.message() << std::endl;
            });
    }
}

void Server::updateClients(const std::vector<Player*>& players) {
    std::vector<Player> data;
    data.reserve(players.size());

    for (auto* p : players)
        data.push_back(*p); 


    for (auto& client : clients) {
        socket.async_send_to(
            asio::buffer(data.data(), data.size() * sizeof(Player)),
            client,
            [](std::error_code ec, std::size_t) {
                if (ec)
                    std::cerr << "[Server] Send error: " << ec.message() << std::endl;
            }
        );
    }
}

