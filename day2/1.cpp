#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::string getInput() {
    const std::string file = "input.txt";
    if (std::ifstream is{file, std::ios::ate}) {
        auto size = is.tellg();
        std::string str(size, '\0');
        is.seekg(0);
        is.read(&str[0], size);
        return str;
    }
    printf("Cannot open file: %s\n", file.c_str());
    return {};
}

uint64_t solution(const std::string& input) {
    uint64_t ret{};
    return ret;
}

int main(int argc, char* argv[]) {
    printf("Answer: %lu\n", solution(getInput()));
    return 0;
}
