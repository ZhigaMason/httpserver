#include <iostream>
#include <atomic>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>

#include "server/server.hpp"
#include "request/request.hpp"
#include "response/static_response_factory.hpp"

#include "argparse.cpp"


std::atomic<bool>& running_flag() {
    static std::atomic<bool> flag{true};
    return flag;
}

void handle_signal(int) {
    running_flag().store(false);
}

int main(int argc, char ** argv) {
        ArgData argdata = input(argc, argv);

        if(argdata.arparse_exit_status == EXIT_HELP)
                return EXIT_SUCCESS;

        signal(SIGPIPE, SIG_IGN);
        signal(SIGTERM, handle_signal);


        StaticResponseFactory factory(argdata.static_dir);


        std::ostream * log_os = nullptr;
        if(argdata.log_file.empty()) log_os = &std::cout;
        else {
                std::ofstream * fos = new std::ofstream(argdata.log_file, std::ios_base::app);
                log_os = fos;
                if(!fos->is_open())
                        throw std::domain_error("Could not open file: " + argdata.log_file);
        }
        Server server(argdata.port, *log_os);
        server.listen();

        std::function<void(int)> response_func = [&factory](int socket_fd) -> void {

                char buffer[30000] = {0};
                read(socket_fd, buffer, 30000);
                Request req(buffer);

                if(req.target.empty())
                        req.target = "index.html";

                factory.make_response(req.target, socket_fd);
        };

        while (running_flag().load()) {
                server.parse_request(response_func);
        }

        if(!argdata.static_dir.empty())
                delete log_os;

        return EXIT_SUCCESS;
}
