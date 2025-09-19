#include "static_response_factory.hpp"

#include <iostream>
#include <fstream>

StaticResponseFactory::StaticResponseFactory(path parent_dir) : parent_dir(std::move(parent_dir)) {}

std::string StaticResponseFactory::make_response(const path & filename) {
        path fp = parent_dir / filename;
        std::string body = readfile(fp);
        if(body.empty()) {
                return  "HTTP/1.1 404 Not Found\r\n"
                        "Content-Type: text/plain\r\n\r\n"
                        "404 Not Found";
        }

        return  "HTTP/1.1 200 OK\r\n" +
                std::string("Content-Type: ") + mime_type(filename) + "\r\n" +
                "Content-Length: " + std::to_string(body.size()) + "\r\n\r\n" +
                body;
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
