#ifndef FILE_REPOSITORY_H
#define FILE_REPOSITORY_H

#include "listas.h"
#include <fstream>
#include <stdexcept>

template <typename T>
class FileRepository {
private:
    const char* ruta;

public:
    FileRepository(const char* rutaArchivo) : ruta(rutaArchivo) {}

    void guardar(const T& objeto) {
        std::ofstream archivo(ruta, std::ios::app);
        if (!archivo.is_open()) {
            throw std::runtime_error("Error al abrir el archivo para escritura");
        }
        archivo << objeto.toCSV() << "\n";
    }

    Lista<T> cargar() {
        Lista<T> objetos;
        std::ifstream archivo(ruta);
        if (!archivo.is_open()) {
            throw std::runtime_error("Error al abrir el archivo para lectura");
        }

        std::string linea;
        while (std::getline(archivo, linea)) {
            objetos.insertar(T::fromCSV(linea));
        }

        return objetos;
    }
};

#endif // FILE_REPOSITORY_H
