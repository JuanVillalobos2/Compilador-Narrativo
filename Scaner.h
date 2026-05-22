#pragma once

#include <vector>
#include <string>
#include "Token.h"

using namespace std;

class Lexer {
private:
    string input;
    int posicion;

public:
    Lexer(std::string texto);

    char obtenerActual();
    void avanzar();

    vector<Token> analizar();
};