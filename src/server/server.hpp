#ifndef SOCKET_HPP_QNQC23C37CR89Q237R89C7NQ2RN23
#define SOCKET_HPP_QNQC23C37CR89Q237R89C7NQ2RN23
#include <iostream>
#include <cstdint>
#include <functional>
#include <netinet/in.h>

struct Server {

        Server(uint16_t port, std::ostream & log_os);
        ~Server() noexcept;

        void listen();

        void parse_request(std::function<void(int)> & response_func);
private:
        int server_fd;
        sockaddr_in address;
        std::ostream & log_os;
};
#endif//SOCKET_HPP_QNQC23C37CR89Q237R89C7NQ2RN23
