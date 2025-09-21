#ifndef SOCKET_HPP_QNQC23C37CR89Q237R89C7NQ2RN23
#define SOCKET_HPP_QNQC23C37CR89Q237R89C7NQ2RN23
#include <iostream>
#include <cstdint>
#include <functional>
#include <netinet/in.h>
#include <thread>

#include "thread_safe_queue/thread_safe_queue.hpp"

struct Server {

        Server(uint16_t port, std::ostream & log_os);
        ~Server() noexcept;

        Server & listen();
        Server & setup_workers(size_t n_thrs, std::function<void(int)> & response_func);

        void parse_request(std::function<void(int)> & response_func);

        void run(std::function<bool()> & stop_predicate);

        void log(const std::string &);

private:
        int server_fd;
        sockaddr_in address;

        std::vector<std::thread> thrs;
        ThreadSafeQueue<int> request_queue;

        std::ostream & log_os;
        std::mutex log_mtx;
};
#endif//SOCKET_HPP_QNQC23C37CR89Q237R89C7NQ2RN23
