#pragma once

#include <string>
#include <unordered_map>
#include "Token.h"

using namespace std;

// Normaliza tildes y variantes ortográficas a su forma canónica sin tilde.
// Esto permite que el texto fuente use tildes libremente sin romper el lexer.
static string normalizar(const string& palabra) {
    string resultado;
    resultado.reserve(palabra.size());

    size_t i = 0;
    while (i < palabra.size()) {
        unsigned char c = palabra[i];

        // Secuencias UTF-8 de 2 bytes para vocales con tilde (minúsculas y mayúsculas)
        if (c == 0xC3 && i + 1 < palabra.size()) {
            unsigned char c2 = palabra[i + 1];
            switch (c2) {
                // á / Á
                case 0xA1: resultado += 'a'; i += 2; continue;
                case 0x81: resultado += 'A'; i += 2; continue;
                // é / É
                case 0xA9: resultado += 'e'; i += 2; continue;
                case 0x89: resultado += 'E'; i += 2; continue;
                // í / Í
                case 0xAD: resultado += 'i'; i += 2; continue;
                case 0x8D: resultado += 'I'; i += 2; continue;
                // ó / Ó
                case 0xB3: resultado += 'o'; i += 2; continue;
                case 0x93: resultado += 'O'; i += 2; continue;
                // ú / Ú
                case 0xBA: resultado += 'u'; i += 2; continue;
                case 0x9A: resultado += 'U'; i += 2; continue;
                // ü / Ü
                case 0xBC: resultado += 'u'; i += 2; continue;
                case 0x9C: resultado += 'U'; i += 2; continue;
                // ñ / Ñ
                case 0xB1: resultado += "nyo"; i += 2; continue;  // ñ → "nyo" interno
                case 0x91: resultado += "NYO"; i += 2; continue;
                default: break;
            }
        }

        resultado += (char)c;
        i++;
    }

    // Convertir a minúsculas para comparación uniforme
    for (char& ch : resultado) ch = tolower((unsigned char)ch);

    return resultado;
}

// Tabla de palabras reservadas: forma normalizada → TokenType
// Para agregar vocabulario nuevo, basta con añadir una entrada aquí.
static const unordered_map<string, TokenType> PALABRAS_RESERVADAS = {

    // Verbos
    {"cumple",   TokenType::VERBO},
    {"cumplir",  TokenType::VERBO},
    {"camina",   TokenType::VERBO},
    {"caminar",  TokenType::VERBO},
    {"sabe",     TokenType::VERBO},
    {"saber",    TokenType::VERBO},
    {"tiene",    TokenType::VERBO},
    {"tener",    TokenType::VERBO},
    {"es",       TokenType::VERBO},
    {"ser",      TokenType::VERBO},
    {"corre",    TokenType::VERBO},
    {"correr",   TokenType::VERBO},
    {"habla",    TokenType::VERBO},
    {"hablar",   TokenType::VERBO},

    // Números como palabra (el lexema se sobreescribe en el lexer)
    {"un",       TokenType::NUMERO},
    {"uno",      TokenType::NUMERO},
    {"una",      TokenType::NUMERO},
    {"dos",      TokenType::NUMERO},
    {"tres",     TokenType::NUMERO},
    {"cuatro",   TokenType::NUMERO},
    {"cinco",    TokenType::NUMERO},

    // Unidades de tiempo
    {"anio",     TokenType::UNIDAD},
    {"anios",    TokenType::UNIDAD},
    {"mes",      TokenType::UNIDAD},
    {"meses",    TokenType::UNIDAD},
    {"dia",      TokenType::UNIDAD},
    {"dias",     TokenType::UNIDAD},
    {"semana",   TokenType::UNIDAD},
    {"semanas",  TokenType::UNIDAD},

    // Adverbios negativos
    {"no",       TokenType::ADVERBIO_NEGA},
    {"nunca",    TokenType::ADVERBIO_NEGA},
    {"jamas",    TokenType::ADVERBIO_NEGA},   // normalizado desde "jamás"
    {"tampoco",  TokenType::ADVERBIO_NEGA},

    // Adverbios positivos
    {"ya",       TokenType::ADVERBIO_POS},
    {"siempre",  TokenType::ADVERBIO_POS},
    {"tambien",  TokenType::ADVERBIO_POS},    // normalizado desde "también"
    {"aun",      TokenType::ADVERBIO_POS},    // normalizado desde "aún"

    // Conectores
    {"y",        TokenType::CONECTOR},
    {"o",        TokenType::CONECTOR},
    {"pero",     TokenType::CONECTOR},
    {"sino",     TokenType::CONECTOR},
    {"ni",       TokenType::CONECTOR},
    {"mas",      TokenType::CONECTOR},        // normalizado desde "más"

    // Irrelevantes (artículos, preposiciones, pronombres de función)
    {"solo",     TokenType::IRRELEVANTE},
    {"el",       TokenType::IRRELEVANTE},
    {"la",       TokenType::IRRELEVANTE},
    {"los",      TokenType::IRRELEVANTE},
    {"las",      TokenType::IRRELEVANTE},
    {"de",       TokenType::IRRELEVANTE},
    {"del",      TokenType::IRRELEVANTE},
    {"en",       TokenType::IRRELEVANTE},
    {"con",      TokenType::IRRELEVANTE},
    {"por",      TokenType::IRRELEVANTE},
    {"para",     TokenType::IRRELEVANTE},
    {"su",       TokenType::IRRELEVANTE},
    {"sus",      TokenType::IRRELEVANTE},
    {"al",       TokenType::IRRELEVANTE},
    {"a",        TokenType::IRRELEVANTE},
};

// Lexemas canónicos para números-palabra (lo que se guarda como lexema final)
static const unordered_map<string, string> NUMERO_CANONICO = {
    {"un",     "1"},
    {"uno",    "1"},
    {"una",    "1"},
    {"dos",    "2"},
    {"tres",   "3"},
    {"cuatro", "4"},
    {"cinco",  "5"},
};

