#include "request.hpp"

Request::Request(const std::string & raw) {
        if(raw.size() < MIN_HEAD_LEN + MIN_BODY_LEN + TAIL_LEN) {
                method = Method::UNKNOWN;
                return;
        }

        size_t tail_sign_loc = raw.find(TAIL_SIGN);
        if(tail_sign_loc == std::string::npos) {
                method = Method::UNKNOWN;
                return;
        }

        // TODO manage other methods
        if(raw.substr(0, 3) == "GET") {
                method = Method::GET;
        }
        else if(raw.substr(0, 3) == "PUT") {
                method = Method::PUT;
        }
        else if(raw.substr(0, 4) == "POST") {
                method = Method::POST;
        }
        else {
                method = Method::UNKNOWN;
        }

        size_t target_idx_bgn = 5; // strlen(GET /)
        size_t target_idx_cnt = tail_sign_loc - target_idx_bgn - 1;
        size_t query_idx_bgn = 0;
        size_t query_idx_cnt = 0;

        size_t question_loc = raw.find('?');
        if(question_loc != std::string::npos && question_loc < tail_sign_loc) {
                target_idx_cnt = question_loc - target_idx_bgn;
                query_idx_bgn = question_loc + 1;
                query_idx_cnt = tail_sign_loc - query_idx_bgn - 1;
        }

        target = raw.substr(target_idx_bgn, target_idx_cnt);
        query = raw.substr(query_idx_bgn, query_idx_cnt);
}

std::ostream & operator<<(std::ostream & os, Request req) {
        os << "Request(";
        switch(req.method) {
                case Request::Method::GET:
                        os << "GET ";
                        break;
                case Request::Method::PUT:
                        os << "PUT ";
                        break;
                case Request::Method::POST:
                        os << "POST ";
                        break;
                case Request::Method::UNKNOWN:
                        os << "UNKNOWN ";
                        break;
        }
        return os  << "target=\""<< req.target << "\" query=\"" << req.query << "\")";
}
