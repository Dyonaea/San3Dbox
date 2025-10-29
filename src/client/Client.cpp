#include "San3Dbox/client/Client.hpp"
#include "San3Dbox/server/World.hpp"

Client::Client(const std::string& host, unsigned short port)
    : socket(io_context, asio::ip::udp::endpoint(asio::ip::udp::v4(), 0)),
      running(false)
{
    asio::ip::udp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(asio::ip::udp::v4(), host, std::to_string(port));
    server_endpoint = *endpoints.begin();
    sharedState = new SharedState();
}

Client::~Client() {
    stop();
}

void Client::start() {
    running = true;
    doReceive();
    ioThread = std::thread([this]() { io_context.run(); });
    interfaceThread = std::thread([this](){
            window = new Window(500, 500, "San3Dbox", sharedState);
            window->run();
            this->stop();
        });
}

void Client::stop() {
    running = false;
    io_context.stop();
    if (ioThread.joinable())
        ioThread.join();
    std::cout << "[Client] Stopped.\n";
}

void Client::send(const std::string& message) {
    socket.async_send_to(asio::buffer(message), server_endpoint,
        [](std::error_code ec, std::size_t) {
            if (ec)
                std::cerr << "[Client] Send error: " << ec.message() << std::endl;
        });
}

void Client::doReceive() {
    socket.async_receive_from(asio::buffer(buffer),server_endpoint,
        [this](std::error_code ec, std::size_t bytes) {
            if (!ec) {
                size_t count = bytes / sizeof(Player);
                Player* players = reinterpret_cast<Player*>(buffer.data());
                {
                    std::lock_guard<std::mutex> lock(sharedState->mtx);
                    sharedState->players.assign(players, players + count);
                    sharedState->updated = true;
                }

                // for (size_t i = 0; i < count; ++i) {
                //     std::cout << "Player " << players[i].id
                //               << " pos = " << players[i].pos.x << ","
                //               << players[i].pos.y << "," << players[i].pos.z << "\n";
                // }

                doReceive();
            }
        }
    );
}
