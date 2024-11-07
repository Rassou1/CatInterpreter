#include "Interpreter.h"
#include <iostream>

int main() {
    Interpreter i = Interpreter();
    std::string input = "1+1";
    std::vector<std::string> result = i.Tokenize(input);

    for (const auto& word : result) {
        std::cout << word << std::endl;
    }

    int b = i.Parse_MathExp();
    std::cout << b << std::endl;

    return 0;
}


