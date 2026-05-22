#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

// ── Tipos de nodos del árbol ─────────────────────────────────────────
enum class NodoTipo {
    PROGRAMA,
    ORACION,
    CLAUSULA,
    SUJETO,
    MODIFICADOR,
    VERBO,
    COMPLEMENTO,
    NUMERO,
    UNIDAD,
    CONECTOR,
    ERROR_SINTACTICO
};

static string nodoTipoStr(NodoTipo t) {
    switch(t) {
        case NodoTipo::PROGRAMA:          return "PROGRAMA";
        case NodoTipo::ORACION:           return "ORACION";
        case NodoTipo::CLAUSULA:          return "CLAUSULA";
        case NodoTipo::SUJETO:            return "SUJETO";
        case NodoTipo::MODIFICADOR:       return "MODIFICADOR";
        case NodoTipo::VERBO:             return "VERBO";
        case NodoTipo::COMPLEMENTO:       return "COMPLEMENTO";
        case NodoTipo::NUMERO:            return "NUMERO";
        case NodoTipo::UNIDAD:            return "UNIDAD";
        case NodoTipo::CONECTOR:          return "CONECTOR";
        case NodoTipo::ERROR_SINTACTICO:  return "ERROR_SINT";
        default:                          return "?";
    }
}

// ── Nodo del AST ──────────────────────────────────────────────────────
struct Nodo {
    NodoTipo tipo;
    string valor;                       // lexema asociado (puede estar vacío)
    int linea;
    vector<shared_ptr<Nodo>> hijos;

    Nodo(NodoTipo tipo, string valor = "", int linea = 0)
        : tipo(tipo), valor(valor), linea(linea) {}

    void agregarHijo(shared_ptr<Nodo> hijo) {
        hijos.push_back(hijo);
    }

    // Imprime el árbol con sangría en cualquier ostream
    void imprimir(ostream& out, const string& prefijo = "", bool esUltimo = true) const {
        string rama    = esUltimo ? "└── " : "├── ";
        string etiqueta = nodoTipoStr(tipo);
        if (!valor.empty()) etiqueta += " [" + valor + "]";
        if (linea > 0)      etiqueta += " (L" + to_string(linea) + ")";

        out << prefijo << rama << etiqueta << "\n";

        string nuevoPrefijo = prefijo + (esUltimo ? "    " : "│   ");
        for (size_t i = 0; i < hijos.size(); i++) {
            hijos[i]->imprimir(out, nuevoPrefijo, i == hijos.size() - 1);
        }
    }
};
