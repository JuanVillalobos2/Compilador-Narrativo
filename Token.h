#pragma once
#include <string>

using namespace std;

enum class TokenType {

    IDENTIFICADOR,
    VERBO,
    NUMERO,
    UNIDAD,
    IRRELEVANTE,
    ADVERBIO_NEGA,
    FIN_ARCHIVO,
    ERROR_TOKEN
};

class Token {

private:

    TokenType type;
    string lexema;

public:

    Token(TokenType type, string lexema);

    TokenType getType() const;

    string getLexema() const;

    string getTypeAsString() const;
};
