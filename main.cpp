#include <iostream>
#include "Lexer.h"

using namespace std;

int main() {

    string texto = "pepito cumple un anio";

    Lexer lexer(texto);

    vector<Token> tokens = lexer.analizar();

    for (Token t : tokens) {

        cout << "<"<< t.getTypeAsString()<< ", "<< t.getLexema()<< ">"<< endl;
    }

    return 0;
}