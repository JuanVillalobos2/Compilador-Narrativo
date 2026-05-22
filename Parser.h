#pragma once
#include <vector>
#include <memory>
#include "Token.h"
#include "NodoAST.h"

using namespace std;

class Parser {
private:
    vector<Token> tokens;
    size_t pos;
    int errores;

    // ── Utilidades de navegación ──────────────────────────────────────
    Token& actual();
    Token& ver(int offset = 0);
    bool   esFin() const;
    Token  consumir();
    Token  esperar(TokenType tipo, const string& contexto);
    bool   check(TokenType tipo) const;

    // ── Reglas gramaticales ───────────────────────────────────────────
    shared_ptr<Nodo> parsePrograma();
    shared_ptr<Nodo> parseOracion();
    shared_ptr<Nodo> parseClausula();
    shared_ptr<Nodo> parseComplemento();

    // ── Recuperación de errores ───────────────────────────────────────
    void sincronizar();

public:
    Parser(vector<Token> tokens);
    shared_ptr<Nodo> analizar();
    int getErrores() const { return errores; }
};
