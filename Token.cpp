#include "Token.h"
#include <initializer_list>

using namespace std;

Token::Token(TokenType type, string lexema, int linea, int columna) {
    this->type    = type;
    this->lexema  = lexema;
    this->linea   = linea;
    this->columna = columna;
}

TokenType Token::getType() const {
    return type;
}

string Token::getLexema() const {
    return lexema;
}

int Token::getLinea() const {
    return linea;
}

int Token::getColumna() const {
    return columna;
}

bool Token::es(TokenType t) const {
    return type == t;
}

bool Token::esUno(initializer_list<TokenType> tipos) const {
    for (TokenType t : tipos) {
        if (type == t) return true;
    }
    return false;
}

string Token::getTypeAsString() const {
    switch(type) {
        case TokenType::IDENTIFICADOR:   return "IDENTIFICADOR";
        case TokenType::VERBO:           return "VERBO";
        case TokenType::NUMERO:          return "NUMERO";
        case TokenType::UNIDAD:          return "UNIDAD";
        case TokenType::ADVERBIO_NEGA:   return "ADVERBIO_NEGA";
        case TokenType::ADVERBIO_POS:    return "ADVERBIO_POS";
        case TokenType::CONECTOR:        return "CONECTOR";
        case TokenType::IRRELEVANTE:     return "IRRELEVANTE";
        case TokenType::NUEVA_LINEA:     return "NUEVA_LINEA";
        case TokenType::FIN_ARCHIVO:     return "FIN_ARCHIVO";
        case TokenType::ERROR_TOKEN:     return "ERROR";
        default:                         return "DESCONOCIDO";
    }
}
