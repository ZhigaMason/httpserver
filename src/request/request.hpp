#ifndef REQUEST_HPP_8149M812D4801288D04192ND104N
#define REQUEST_HPP_8149M812D4801288D04192ND104N

#include <cstring>
#include <string>
#include <cstdint>
#include <iostream>

struct Request {
        enum class Method : uint8_t {
                GET,
                POST,
                PUT,
                UNKNOWN=0xFF
        };

        Request(const std::string &);

        friend std::ostream & operator<<(std::ostream & os, Request req);

        Method method;
        std::string target = "", query = "";

        static constexpr size_t TAIL_LEN = 8;      //strlen("HTTP/1.1");
        static constexpr size_t MIN_HEAD_LEN = 3;  //strlen("GET");
        static constexpr size_t MIN_BODY_LEN = 3;  //strlen(" / ");
        static constexpr std::string TAIL_SIGN = "HTTP/1.1";

};
#endif//REQUEST_HPP_8149M812D4801288D04192ND104N
