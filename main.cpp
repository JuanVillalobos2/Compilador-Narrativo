#include <iostream>
#include <fstream>
#include <sstream>

#include "Scaner.h"
#include "Parser.h"

using namespace std;

static void imprimir(ostream& salida, const string& linea) {
    cout   << linea << "\n";
    salida << linea << "\n";
}

int main() {

    ifstream archivo("entrada.txt");
    if (!archivo.is_open()) {
        cerr << "[Error] No se pudo abrir 'entrada.txt'\n";
        return 1;
    }
    stringstream buffer;
    buffer << archivo.rdbuf();
    archivo.close();
    string texto = buffer.str();

    ofstream salida("tokens.txt");
    if (!salida.is_open()) {
        cerr << "[Error] No se pudo crear 'tokens.txt'\n";
        return 1;
    }

    // ── Fase 1: Análisis léxico ───────────────────────────────────────
    Lexer lexer(texto);
    vector<Token> tokens = lexer.analizar();

    imprimir(salida, "=== TOKENS ===");
    int errLex = 0;

    for (const Token& t : tokens) {
        if (t.es(TokenType::NUEVA_LINEA)) { imprimir(salida, "---"); continue; }

        string linea = "<" + t.getTypeAsString() + ", " + t.getLexema() + "> "
                     + "(L" + to_string(t.getLinea()) + ":C" + to_string(t.getColumna()) + ")";
        imprimir(salida, linea);

        if (t.es(TokenType::ERROR_TOKEN)) {
            string err = "[Error lexico] L" + to_string(t.getLinea())
                       + ":C" + to_string(t.getColumna())
                       + " -> token no reconocido: \"" + t.getLexema() + "\"";
            cerr   << err << "\n";
            salida << err << "\n";
            errLex++;
        }
    }

    imprimir(salida, "");
    if (errLex > 0)
        imprimir(salida, "Analisis lexico: " + to_string(errLex) + " error(es).");
    else
        imprimir(salida, "Analisis lexico: sin errores.");

    // ── Fase 2: Análisis sintáctico ───────────────────────────────────
    imprimir(salida, "\n=== ARBOL SINTACTICO ===");

    Parser parser(tokens);
    shared_ptr<Nodo> arbol = parser.analizar();

    // Imprimir árbol en consola con sangría
    arbol->imprimir(cout);
    // También al archivo tokens.txt
    arbol->imprimir(salida);

    imprimir(salida, "");
    int errSint = parser.getErrores();
    if (errSint > 0)
        imprimir(salida, "Analisis sintactico: " + to_string(errSint) + " error(es).");
    else
        imprimir(salida, "Analisis sintactico: sin errores.");

    salida.close();
    cout << "\n[OK] Resultado guardado en 'tokens.txt'\n";

    return (errLex + errSint > 0) ? 1 : 0;
}
