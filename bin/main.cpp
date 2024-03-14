#include "parser.h"

int main(int argc,char* argv[]) {
    std::vector<std::string> arguments;
    for (int i = 1; i < argc; i++) {
        arguments.emplace_back(argv[i]);
    }
    parse(arguments);

    return 0;
}
