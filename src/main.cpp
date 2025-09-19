#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>

#include "server/server.hpp"


int main() {
        Server server(8080, std::cout);
        server.listen();

        std::function<void(int)> response_func = [](int socket_fd) -> void {
                // Read request
                char buffer[30000] = {0};
                read(socket_fd, buffer, 30000);
                std::cout << "Request:\n" << buffer << "\n";

                // Send response
                std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Connection: close\r\n\r\n"
                "<html><body><h1>Hello, World!</h1></body></html>";
                write(socket_fd, response.c_str(), response.size());
        };

        while (true) {
                server.parse_request(response_func);
        }

        return EXIT_SUCCESS;
}
