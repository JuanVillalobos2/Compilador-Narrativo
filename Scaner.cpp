#include "Lexer.h"
#include <sstream>

using namespace std;

Lexer::Lexer(std::string texto) {
    input = texto;
    posicion = 0;
}

vector<Token> Lexer::analizar() {

    vector<Token> tokens;

    stringstream ss(input);

    string palabra;

    while (ss >> palabra) {

        if (palabra == "cumple") {
            tokens.push_back(Token(TokenType::VERBO, palabra));
        }

        else if (palabra == "anio") {
            tokens.push_back(Token(TokenType::UNIDAD, palabra));
        }

        else if (palabra == "un") {
            tokens.push_back(Token(TokenType::NUMERO, "1"));
        }

        else {
            tokens.push_back(Token(TokenType::IDENTIFICADOR, palabra));
        }
    }

    tokens.push_back(Token(TokenType::FIN_ARCHIVO, "fin del archivo"));

    return tokens;
}