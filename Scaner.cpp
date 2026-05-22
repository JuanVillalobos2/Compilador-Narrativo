#include "Scaner.h"
#include <sstream>

using namespace std;

Lexer::Lexer(string texto) {
    input = texto;
    //posicion = 0;
}

bool esNumero(string palabra){
        for(char c : palabra) {
            if(!isdigit(c)) {
                return false;
            }
        }   
        return true;
    }

vector<Token> Lexer::analizar() {

    vector<Token> tokens;

    stringstream ss(input);

    string palabra;

    while (ss >> palabra) {

        if (palabra == "cumple" || palabra =="camina" || palabra =="sabe"|| palabra =="caminar") {
            tokens.push_back(
                Token(TokenType::VERBO, palabra)
            );
        }

        else if (palabra == "un") {
            tokens.push_back(
                Token(TokenType::NUMERO, "1")
            );
        }

        else if (esNumero(palabra)) {
            tokens.push_back(
                Token(TokenType::NUMERO, palabra)
            );
        }

        else if (palabra == "anio" || palabra == "anios") {
            tokens.push_back(
                Token(TokenType::UNIDAD, palabra)
            );
        }

        else if (palabra == "y" || palabra == "ya" || palabra == "solo"|| palabra =="pero") {
            tokens.push_back(
                Token(TokenType::IRRELEVANTE, palabra)
            );
        }
        
        else if (palabra == "no"|| palabra =="nunca"|| palabra =="jamas") {
            tokens.push_back(
                Token(TokenType::ADVERBIO_NEGA, palabra)
            );
        }

        else {

            tokens.push_back(
                Token(TokenType::IDENTIFICADOR, palabra)
            );
        }
    }

    tokens.push_back(
        Token(TokenType::FIN_ARCHIVO, "fin del archivo")
    );

    return tokens;
}
