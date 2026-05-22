#include "Scaner.h"
#include "keywords.h"
#include <sstream>
#include <cctype>
#include <iostream>

using namespace std;

Lexer::Lexer(string texto) {
    input = texto;
}

// Elimina puntuación pegada al inicio y al final de una palabra.
// Ejemplo: "anios," → "anios" | "¡Hola!" → "Hola"
static string limpiarPuntuacion(const string& palabra) {
    int inicio = 0;
    int fin = (int)palabra.size() - 1;

    while (inicio <= fin && ispunct((unsigned char)palabra[inicio])) inicio++;
    while (fin >= inicio && ispunct((unsigned char)palabra[fin]))   fin--;

    if (inicio > fin) return "";
    return palabra.substr(inicio, fin - inicio + 1);
}

// Decide si una palabra es un identificador válido (nombre propio):
// debe empezar con mayúscula y contener solo letras.
static bool esIdentificador(const string& palabra) {
    if (palabra.empty()) return false;
    if (!isupper((unsigned char)palabra[0])) return false;
    for (char c : palabra) {
        if (!isalpha((unsigned char)c)) return false;
    }
    return true;
}

// Verifica si un string es un número entero puro.
static bool esNumeroLiteral(const string& palabra) {
    if (palabra.empty()) return false;
    for (char c : palabra) {
        if (!isdigit((unsigned char)c)) return false;
    }
    return true;
}

vector<Token> Lexer::analizar() {

    vector<Token> tokens;
    istringstream ss(input);
    string lineaStr;
    int numLinea = 1;

    while (getline(ss, lineaStr)) {

        istringstream lineaStream(lineaStr);
        string palabraBruta;
        int columna = 1;

        while (lineaStream >> palabraBruta) {

            // 1. Limpiar puntuación
            string palabraLimpia = limpiarPuntuacion(palabraBruta);
            if (palabraLimpia.empty()) {
                columna += (int)palabraBruta.size() + 1;
                continue;
            }

            // 2. Normalizar (sin tildes, minúsculas) para buscar en la tabla
            string palabraNorm = normalizar(palabraLimpia);

            // 3. Buscar en tabla de palabras reservadas
            auto it = PALABRAS_RESERVADAS.find(palabraNorm);

            if (it != PALABRAS_RESERVADAS.end()) {
                TokenType tipo = it->second;
                string lexema = palabraLimpia;

                // Para números-palabra, guardar el valor numérico canónico
                if (tipo == TokenType::NUMERO) {
                    auto itN = NUMERO_CANONICO.find(palabraNorm);
                    if (itN != NUMERO_CANONICO.end()) {
                        lexema = itN->second;
                    }
                }

                tokens.push_back(Token(tipo, lexema, numLinea, columna));
            }

            // 4. Número literal
            else if (esNumeroLiteral(palabraLimpia)) {
                tokens.push_back(Token(TokenType::NUMERO, palabraLimpia, numLinea, columna));
            }

            // 5. Identificador válido (empieza con mayúscula, solo letras)
            else if (esIdentificador(palabraLimpia)) {
                tokens.push_back(Token(TokenType::IDENTIFICADOR, palabraLimpia, numLinea, columna));
            }

            // 6. Error léxico: no encaja en ninguna categoría
            else {
                tokens.push_back(Token(TokenType::ERROR_TOKEN, palabraLimpia, numLinea, columna));            }

            columna += (int)palabraBruta.size() + 1;
        }

        tokens.push_back(Token(TokenType::NUEVA_LINEA, "\\n", numLinea, 0));
        numLinea++;
    }

    tokens.push_back(Token(TokenType::FIN_ARCHIVO, "fin del archivo", numLinea, 0));

    return tokens;
}
