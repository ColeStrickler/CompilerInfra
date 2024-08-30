



#include <fstream>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;


int main(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    if (!file) {
        std::cout << "could not open\n";
        return -1;
    }


    std::stringstream buf;
    buf << file.rdbuf();
    std::string content = buf.str();

    std::cout << content << std::endl;



    auto j = json::parse(content);
    std::cout << j["test"]["yolo"] << "\n";
    file.close();
}