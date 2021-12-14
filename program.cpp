#include <iostream>

#include "Lexer/Lexer.hpp"

int main()
{
    Lexer lexer("source.trk");

    lexer.tokenize();

    lexer.print_tokens();

    std::cin.get();
}