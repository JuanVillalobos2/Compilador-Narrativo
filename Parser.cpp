#include "Parser.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Parser::Parser(vector<Token> tokens) : tokens(tokens), pos(0), errores(0) {}

// ── Utilidades ────────────────────────────────────────────────────────

Token& Parser::actual() {
    return tokens[pos];
}

bool Parser::esFin() const {
    return tokens[pos].es(TokenType::FIN_ARCHIVO);
}

bool Parser::check(TokenType tipo) const {
    return tokens[pos].es(tipo);
}

Token Parser::consumir() {
    Token t = tokens[pos];
    if (!esFin()) pos++;
    return t;
}

// Consume el token esperado; si no coincide, reporta error y devuelve token ficticio
Token Parser::esperar(TokenType tipo, const string& contexto) {
    if (check(tipo)) return consumir();

    cerr << "[Error sintactico] L" << actual().getLinea()
         << ":C" << actual().getColumna()
         << " -> se esperaba " << contexto
         << " pero se encontro \"" << actual().getLexema() << "\"\n";
    errores++;

    // Devolver token vacío sin avanzar (no consumimos el token incorrecto)
    return Token(TokenType::ERROR_TOKEN, "", actual().getLinea(), actual().getColumna());
}

// Avanza hasta el siguiente NUEVA_LINEA o FIN_ARCHIVO para recuperarse
void Parser::sincronizar() {
    while (!esFin() && !check(TokenType::NUEVA_LINEA))
        consumir();
    if (check(TokenType::NUEVA_LINEA)) consumir();
}

// ── Gramática ─────────────────────────────────────────────────────────

// programa → oracion* EOF
shared_ptr<Nodo> Parser::parsePrograma() {
    auto raiz = make_shared<Nodo>(NodoTipo::PROGRAMA, "raiz");

    while (!esFin()) {
        // Saltar líneas vacías
        if (check(TokenType::NUEVA_LINEA)) {
            consumir();
            continue;
        }

        try {
            auto oracion = parseOracion();
            if (oracion) raiz->agregarHijo(oracion);
        } catch (const runtime_error& e) {
            cerr << "[Error sintactico] " << e.what() << "\n";
            errores++;
            sincronizar();
        }
    }

    return raiz;
}

// oracion → clausula (CONECTOR clausula)* NUEVA_LINEA
shared_ptr<Nodo> Parser::parseOracion() {
    int lineaInicio = actual().getLinea();
    auto nOracion = make_shared<Nodo>(NodoTipo::ORACION, "", lineaInicio);

    // Primera cláusula obligatoria
    nOracion->agregarHijo(parseClausula());

    // Saltear IRRELEVANTE sueltos entre cláusulas (ej: "solo" antes de "pero")
    while (check(TokenType::IRRELEVANTE)) consumir();

    // Cláusulas adicionales unidas por conector
    while (check(TokenType::CONECTOR)) {
        Token conector = consumir();
        auto nConector = make_shared<Nodo>(NodoTipo::CONECTOR, conector.getLexema(), conector.getLinea());
        nOracion->agregarHijo(nConector);
        nOracion->agregarHijo(parseClausula());
    }

    // Consumir el fin de línea
    if (check(TokenType::NUEVA_LINEA)) consumir();

    return nOracion;
}

// clausula → IDENTIFICADOR modificadores* VERBO complemento?
shared_ptr<Nodo> Parser::parseClausula() {
    auto nClausula = make_shared<Nodo>(NodoTipo::CLAUSULA, "", actual().getLinea());

    // Sujeto (nombre propio obligatorio)
    if (!check(TokenType::IDENTIFICADOR)) {
        throw runtime_error("se esperaba un nombre propio en L"
                            + to_string(actual().getLinea()));
    }
    Token sujeto = consumir();
    nClausula->agregarHijo(make_shared<Nodo>(NodoTipo::SUJETO, sujeto.getLexema(), sujeto.getLinea()));

    // Modificadores opcionales: adverbios antes del verbo
    while (actual().esUno({TokenType::ADVERBIO_POS, TokenType::ADVERBIO_NEGA, TokenType::IRRELEVANTE})) {
        Token mod = consumir();
        // Ignorar IRRELEVANTE (artículos, "solo", etc.) en el árbol
        if (!mod.es(TokenType::IRRELEVANTE)) {
            nClausula->agregarHijo(make_shared<Nodo>(NodoTipo::MODIFICADOR, mod.getLexema(), mod.getLinea()));
        }
    }

    // Verbo obligatorio
    if (!check(TokenType::VERBO)) {
        throw runtime_error("se esperaba un verbo despues del sujeto \"" 
                            + (nClausula->hijos.empty() ? "?" : nClausula->hijos[0]->valor)
                            + "\" en L" + to_string(actual().getLinea()));
    }
    Token verbo = consumir();
    nClausula->agregarHijo(make_shared<Nodo>(NodoTipo::VERBO, verbo.getLexema(), verbo.getLinea()));

    // Complemento opcional
    if (actual().esUno({TokenType::NUMERO, TokenType::UNIDAD,
                        TokenType::ADVERBIO_NEGA, TokenType::VERBO})) {
        nClausula->agregarHijo(parseComplemento());
    }

    return nClausula;
}

// complemento → NUMERO? UNIDAD
//             | ADVERBIO_NEGA? VERBO
shared_ptr<Nodo> Parser::parseComplemento() {
    auto nComp = make_shared<Nodo>(NodoTipo::COMPLEMENTO, "", actual().getLinea());

    // Caso 1: número + unidad  (ej: "2 anios", "tres meses")
    if (check(TokenType::NUMERO)) {
        Token num = consumir();
        nComp->agregarHijo(make_shared<Nodo>(NodoTipo::NUMERO, num.getLexema(), num.getLinea()));

        if (check(TokenType::UNIDAD)) {
            Token unidad = consumir();
            nComp->agregarHijo(make_shared<Nodo>(NodoTipo::UNIDAD, unidad.getLexema(), unidad.getLinea()));
        }
    }
    // Caso 2: solo unidad (raro, pero posible)
    else if (check(TokenType::UNIDAD)) {
        Token unidad = consumir();
        nComp->agregarHijo(make_shared<Nodo>(NodoTipo::UNIDAD, unidad.getLexema(), unidad.getLinea()));
    }
    // Caso 3: adverbio negativo + verbo(s)  (ej: "no sabe caminar")
    else if (check(TokenType::ADVERBIO_NEGA)) {
        Token adv = consumir();
        nComp->agregarHijo(make_shared<Nodo>(NodoTipo::MODIFICADOR, adv.getLexema(), adv.getLinea()));

        // Consumir todos los verbos encadenados (sabe caminar, puede correr, etc.)
        while (check(TokenType::VERBO)) {
            Token v = consumir();
            nComp->agregarHijo(make_shared<Nodo>(NodoTipo::VERBO, v.getLexema(), v.getLinea()));
        }
    }
    // Caso 4: verbo(s) directos en complemento (ej: "sabe caminar")
    else if (check(TokenType::VERBO)) {
        while (check(TokenType::VERBO)) {
            Token v = consumir();
            nComp->agregarHijo(make_shared<Nodo>(NodoTipo::VERBO, v.getLexema(), v.getLinea()));
        }
    }

    return nComp;
}

// ── Punto de entrada ──────────────────────────────────────────────────
shared_ptr<Nodo> Parser::analizar() {
    return parsePrograma();
}
