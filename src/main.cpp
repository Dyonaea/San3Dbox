#include <iostream>
#include <thread>
#include <string>
#include "San3Dbox/server/Server.hpp"
#include "San3Dbox/client/Client.hpp"
#include "San3Dbox/interface/Window.hpp"
#include "San3Dbox/interface/WorldRender.hpp"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Usage:\n"
                  << "  " << argv[0] << " server <port>\n"
                  << "  " << argv[0] << " client <ip> <port>\n";
        return 1;
    }

    std::string mode = argv[1];

    try {
        if (mode == "server") {
            int port = (argc >= 3) ? std::stoi(argv[2]) : 5555;

            Server server(port);
            std::cout << "[INFO] Starting server on port " << port << "...\n";
            server.start();
            while (true) {
                std::string cmd;
                std::getline(std::cin, cmd);
                if (cmd == "quit" || cmd == "exit") break;
            }
            server.stop();
        }
        else if (mode == "client") {
            if (argc < 4) {
                std::cerr << "Usage: " << argv[0] << " client <ip> <port>\n";
                return 1;
            }

            std::string ip = argv[2];
            int port = std::stoi(argv[3]);

            Client client(ip, port);
            client.start();
            std::string message;
            std::cout << "Type a message (or 'exit' to quit):\n";
            client.send("Hello\n");
            while (true) {
                std::getline(std::cin, message);
                if (message == "exit") break;
                client.send(message);
            }

        }
        else {
            std::cerr << "Unknown mode: " << mode << "\n";
            return 1;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
    }

    return 0;
}
