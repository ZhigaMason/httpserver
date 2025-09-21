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

        log("Server successfully created on port " + std::to_string(port) + " fd=" + std::to_string(server_fd));
}

Server::~Server() noexcept {
        close(server_fd);
        for(auto & thr : thrs)
                thr.join();
        log("Server successfully closed fd=" + std::to_string(server_fd));
}

Server & Server::listen() {
        ::listen(server_fd, 3);
        log("Server listening. fd=" + std::to_string(server_fd));
        return *this;
}

Server & Server::setup_workers(size_t n_thrs, std::function<void(int)> & response_func) {
        for(size_t i = 0; i < n_thrs; ++i) {
                thrs.emplace_back([&] () {
                        std::optional<int> socket_fd{};
                        while((socket_fd = request_queue.pop())) {
                                response_func(*socket_fd);
                                close(*socket_fd);

                                this->log("Parsed request. fd=" + std::to_string(server_fd) + " socket=" + std::to_string(*socket_fd));
                        }
                });
        }
        return *this;
}

void Server::run(std::function<bool()> & stop_predicate) {
        while(!stop_predicate()) {
                int addrlen = sizeof(addrlen);
                int new_socket = accept(server_fd, (sockaddr *)&address,
                                (socklen_t*)&addrlen);

                if (new_socket < 0) {
                        throw std::domain_error("Accepting socket failed.");
                }

                request_queue.push(new_socket);
        }

        request_queue.finish_pops(thrs.size());
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
}

void Server::log(const std::string & txt) {
        std::lock_guard lock(log_mtx);
        log_os << txt << std::endl;
}
