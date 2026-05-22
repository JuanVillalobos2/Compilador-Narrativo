#include "Token.h"

using namespace std;

Token::Token(TokenType type, string lexema) {
    this->type = type;
    this->lexema = lexema;
}

TokenType Token::getType() const {
    return type;
}

std::string Token::getLexema() const {
    return lexema;
}
string Token::getTypeAsString() const {

    switch(type) {
        case TokenType::IDENTIFICADOR:
            return "IDENTIFICADOR";
        case TokenType::VERBO:
            return "VERBO";
        case TokenType::NUMERO:
            return "NUMERO";
        case TokenType::UNIDAD:
            return "UNIDAD";
        case TokenType::FIN_ARCHIVO:
            return "FIN_ARCHIVO";
        case TokenType::IRRELEVANTE:
            return "IRRELEVANTE";   
        case TokenType::ADVERBIO_NEGA:
            return "ADVERBIO_NEGA";      
        case TokenType::ERROR_TOKEN:
            return "ERROR";
        default:
            return "DESCONOCIDO";
    }
}
