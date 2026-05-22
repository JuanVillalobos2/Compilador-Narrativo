#pragma once

#include <vector>
#include <string>
#include "Token.h"

using namespace std;

class Lexer {
private:
    string input;

public:
    Lexer(string texto);
    vector<Token> analizar();
};
