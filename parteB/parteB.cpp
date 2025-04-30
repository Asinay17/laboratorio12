#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

// Estructura para guardar la base de conocimiento
map<string, string> baseConocimiento;

// Función para convertir texto a minúsculas
string aMinusculas(const string& texto) {
    string resultado = texto;
    transform(resultado.begin(), resultado.end(), resultado.begin(),
        [](unsigned char c) { return tolower(c); });
    return resultado;
}

// Cargar conocimiento desde archivo
void cargarBaseConocimiento(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    string linea;
    while (getline(archivo, linea)) {
        size_t separador = linea.find('|');
        if (separador != string::npos) {
            string pregunta = linea.substr(0, separador);
            string respuesta = linea.substr(separador + 1);
            baseConocimiento[aMinusculas(pregunta)] = respuesta;
        }
    }
    archivo.close();
}

// Búsqueda por palabras clave
string buscarPorPalabrasClave(const string& preguntaUsuario) {
    vector<string> palabrasUsuario;
    istringstream iss(preguntaUsuario);
    string palabra;
    while (iss >> palabra) {
        palabrasUsuario.push_back(aMinusculas(palabra));
    }

    for (const auto& par : baseConocimiento) {
        for (const string& palabra : palabrasUsuario) {
            if (par.first.find(palabra) != string::npos) {
                return par.second;
            }
        }
    }

    return "";
}

// Función principal
int main() {
    cargarBaseConocimiento("conocimiento.txt");

    cout << "=== ChatBot C++ ===" << endl;
    cout << "Escribe 'salir' para terminar." << endl;

    string preguntaUsuario;
    while (true) {
        cout << "\nTú: ";
        getline(cin, preguntaUsuario);

        if (aMinusculas(preguntaUsuario) == "salir")
            break;

        string preguntaNormalizada = aMinusculas(preguntaUsuario);

        // Búsqueda exacta
        if (baseConocimiento.count(preguntaNormalizada)) {
            cout << "Bot: " << baseConocimiento[preguntaNormalizada] << endl;
        }
        else {
            // Búsqueda por palabras clave
            string respuesta = buscarPorPalabrasClave(preguntaUsuario);
            if (!respuesta.empty()) {
                cout << "Bot: " << respuesta << endl;
            }
            else {
                cout << "Bot: Lo siento, no tengo una respuesta para eso." << endl;
            }
        }
    }

    cout << "Chat finalizado. ¡Hasta luego!" << endl;
    return 0;
}
