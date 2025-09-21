#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>

#include "server/server.hpp"
#include "request/request.hpp"
#include "response/static_response_factory.hpp"


int main() {

        signal(SIGPIPE, SIG_IGN);

        StaticResponseFactory factory("static");

        Server server(8080, std::cout);
        server.listen();

        std::function<void(int)> response_func = [&factory](int socket_fd) -> void {

                char buffer[30000] = {0};
                read(socket_fd, buffer, 30000);
                Request req(buffer);

                if(req.target.empty())
                        req.target = "index.html";

                factory.make_response(req.target, socket_fd);
        };

        while (true) {
                server.parse_request(response_func);
        }

        return EXIT_SUCCESS;
}
