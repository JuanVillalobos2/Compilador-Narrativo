#pragma once
#include <string>

using namespace std;

// Tipos de tokens para el compilador narrativo
// Organizados por categoría gramatical
enum class TokenType {

    // --- Entidades ---
    IDENTIFICADOR,      // Nombres propios: Luis, Mateo, etc.

    // --- Verbos ---
    VERBO,              // Acción principal: cumple, camina, sabe, etc.

    // --- Cuantificadores ---
    NUMERO,             // Literal numérico o su equivalente léxico: 2, "un" → 1

    // --- Unidades de medida narrativa ---
    UNIDAD,             // anio, anios, mes, meses, dia, dias...

    // --- Modificadores ---
    ADVERBIO_NEGA,      // Negación: no, nunca, jamás
    ADVERBIO_POS,       // Afirmación/intensificación: ya, siempre, también

    // --- Conectores / Estructura ---
    CONECTOR,           // y, o, pero, sino (unen cláusulas)
    IRRELEVANTE,        // Palabras descartadas: solo, etc.

    // --- Control de flujo léxico ---
    NUEVA_LINEA,        // Separador lógico de oraciones (para el parser)
    FIN_ARCHIVO,        // EOF

    // --- Error ---
    ERROR_TOKEN
};

class Token {

private:
    TokenType type;
    string lexema;
    int linea;      // Línea donde aparece (útil para errores en el parser)
    int columna;    // Columna donde aparece

public:
    // Constructor completo (con posición)
    Token(TokenType type, string lexema, int linea = 0, int columna = 0);

    TokenType getType() const;
    string getLexema() const;
    int getLinea() const;
    int getColumna() const;

    string getTypeAsString() const;

    // Utilidad: saber si el token es de un tipo específico
    bool es(TokenType t) const;
    bool esUno(initializer_list<TokenType> tipos) const;
};
