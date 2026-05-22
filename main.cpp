#include <iostream>
#include <fstream>
#include <sstream>

#include "Scaner.h"

using namespace std;

int main() {

    ifstream archivo("entrada.txt");

    if (!archivo.is_open()) {
        cout << "No existe entrada" << endl;
        return 0;
    }

    stringstream buffer;
    buffer << archivo.rdbuf();
    string texto = buffer.str();

    Lexer lexer(texto);

    vector<Token> tokens = lexer.analizar();

    for (Token t : tokens) {

        cout << "<"<< t.getTypeAsString()<< ", "<< t.getLexema()<< ">"<< endl;
    }

    archivo.close();

    return 0;
}
