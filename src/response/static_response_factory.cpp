#include "static_response_factory.hpp"

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/sendfile.h>

StaticResponseFactory::StaticResponseFactory(path parent_dir) : parent_dir(std::move(parent_dir)) {}

void StaticResponseFactory::make_response(const path & filename, int socket) {
        static const std::string response404 = "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n\r\n"
                "404 Not Found";
        path fp = parent_dir / filename;
        FILE * fptr = fopen(fp.string().c_str(), "rb");
        if(!fptr) {
                write(socket, response404.c_str(), response404.size());
                return;
        }

        int fd = fileno(fptr);
        size_t filesize = file_size(fp);

        std::string header =  "HTTP/1.1 200 OK\r\n" +
                std::string("Content-Type: ") + mime_type(filename) + "\r\n" +
                "Content-Length: " + std::to_string(filesize) + "\r\n\r\n";

        write(socket, header.c_str(), header.size());
        off_t offset = 0;
        ssize_t sent = 0;
        while (sent < filesize) {
                ssize_t n = sendfile(socket, fd, &offset, filesize - sent);
                if (n <= 0) {
                        if (errno == EPIPE || errno == ECONNRESET)
                                break;
                        fclose(fptr);
                        throw std::domain_error("sendfile");
                }
                sent += n;
        }
        fclose(fptr);
}

std::string StaticResponseFactory::readfile(const path & fp) {
        std::ifstream file(fp, std::ios::binary);
        if (!file) return ""; // File not found
        std::ostringstream ss;
        ss << file.rdbuf();
        return ss.str();
}

std::string StaticResponseFactory::mime_type(const path & fp) {
        std::string extension = fp.extension();
        if (extension == ".html") return "text/html";
        if (extension == ".css")  return "text/css";
        if (extension == ".js")   return "application/javascript";
        if (extension == ".png")  return "image/png";
        if (extension == ".jpg")  return "image/jpeg";
        return "application/octet-stream";
}
