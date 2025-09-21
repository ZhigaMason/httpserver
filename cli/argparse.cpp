#include <getopt.h>
#include <cstdio>
#include <string>
#include <cstdint>

constexpr int32_t EXIT_HELP = 100;
struct ArgData {
        std::string static_dir = "static";
        std::string log_file = "";
        uint16_t port = 8080;
        size_t n_threads = 1;
        int32_t arparse_exit_status = EXIT_SUCCESS;
};

void print_help() {
        printf(R"###(
Usage:
        httpserver [-d/--dir] <file directory> [-l/--log] <log file> [-p/--port] <port> [-w/--workers] <number of workers>

Options:
        -h/--help    - display this help menu

        -d/--dir     - file directory, where all of the served files could be found.
                                default: static

        -l/--log     - file where all logs will be dumped. If none is provided STDOUT is used.
                                default: logs to STDOUT

        -p/--port    - TCP port to create connection with.
                                default: 8080

        -w/--workers - number of worker threads serving files
                                default: 1


)###");
}

ArgData input(int argc, char ** argv) {
        enum EOptions : int32_t {
                FLAGGED      = 0,
                UNKNOWN      = '?',
                NOARGS       = -1,

                OPTARG       = ':',

                HELP         = 'h',
                DIRECTORY    = 'd',
                LOGS         = 'l',
                PORT         = 'p',
                WORKERS      = 'w'
        };

        static struct option LONG_OPTIONS[] = {
                {"help",    no_argument, nullptr, HELP},
                {"dir",     required_argument, nullptr, DIRECTORY},
                {"logs",    required_argument, nullptr, LOGS},
                {"port",    required_argument, nullptr, PORT},
                {"workers", required_argument, nullptr, WORKERS},
                {0, 0, 0, 0}
        };

        static constexpr char OPT_STR[] = "hd:l:p:w:";

        int32_t c = -1;
        ArgData ret;

        while((c = getopt_long(argc, argv, OPT_STR, LONG_OPTIONS, nullptr)) != NOARGS) {
                switch(c) {
                        case FLAGGED:
                                break;
                        case HELP:
                                print_help();
                                ret.arparse_exit_status = EXIT_HELP;
                                return ret;
                        case DIRECTORY:
                                ret.static_dir = optarg;
                                break;
                        case LOGS:
                                ret.log_file = optarg;
                                break;
                        case PORT:
                                ret.port = std::stoul(optarg);
                                break;
                        case WORKERS:
                                ret.n_threads = std::stoul(optarg);
                                break;
                        case UNKNOWN:
                        default:
                                break;
                };
        }

        return ret;
}
