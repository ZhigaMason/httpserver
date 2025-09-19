#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>


int main() {
    int server_fd, new_socket;
    sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Reuse address/port
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind to port 8080
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening
    listen(server_fd, 3);

    std::cout << "Server listening on http://localhost:8080\n";

    while (true) {
        // Accept client
        new_socket = accept(server_fd, (sockaddr *)&address,
                            (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Read request
        char buffer[30000] = {0};
        read(new_socket, buffer, 30000);
        std::cout << "Request:\n" << buffer << "\n";

        // Send response
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n\r\n"
            "<html><body><h1>Hello, World!</h1></body></html>";
        write(new_socket, response.c_str(), response.size());

        close(new_socket);
    }

    return EXIT_SUCCESS;
}
