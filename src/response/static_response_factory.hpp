#ifndef STATIC_RESPONSE_HPP_1C04914N9N891C041C04N891C0489
#define STATIC_RESPONSE_HPP_1C04914N9N891C041C04N891C0489

#include <filesystem>

using namespace std::filesystem;
struct StaticResponseFactory {

        StaticResponseFactory(path parent_dir);

        std::string make_response(const path & filename);

private:
        path parent_dir;

        std::string readfile(const path & fp);

        std::string mime_type(const path & fp);
};
#endif//STATIC_RESPONSE_HPP_1C04914N9N891C041C04N891C0489
