#include "server.hpp"
#include <unistd.h>
#include <stdexcept>

Server::Server(uint16_t port, std::ostream & log_os) : log_os(log_os) {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == 0) {
                throw std::domain_error("socket creation failed");
        }

        int opt = 1;
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
                throw std::domain_error("address bind failed");
        }

        log_os << "Server successfully created on port " << port << " fd=" << server_fd << std::endl;
}

Server::~Server() noexcept {
        close(server_fd);
        log_os << "Server successfully closed fd=" << server_fd << std::endl;
}

void Server::listen() {
        ::listen(server_fd, 3);
        log_os << "Server listening. fd=" << server_fd << std::endl;
}

void Server::parse_request(std::function<void(int)> & response_func) {
        int addrlen = sizeof(addrlen);
        int new_socket = accept(server_fd, (sockaddr *)&address,
                            (socklen_t*)&addrlen);
        if (new_socket < 0) {
                throw std::domain_error("Accepting socket failed.");
        }
        response_func(new_socket);
        close(new_socket);
        log_os << "Parsed request. fd=" << server_fd << std::endl;
}
