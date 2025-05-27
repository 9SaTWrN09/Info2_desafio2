#ifndef FILE_REPOSITORY_H
#define FILE_REPOSITORY_H

#include "listas.h"
#include <fstream>
#include <stdexcept>
#include <cstring>

// Clase auxiliar para manejo de strings sin STL
class StringBuffer {
private:
    char* buffer;
    int capacidad;
    int longitud;

    void redimensionar(int nuevaCapacidad) {
        if (nuevaCapacidad <= capacidad) return;

        char* nuevoBuffer = new char[nuevaCapacidad];
        if (buffer != nullptr) {
            strcpy(nuevoBuffer, buffer);
            delete[] buffer;
        }
        buffer = nuevoBuffer;
        capacidad = nuevaCapacidad;
    }

public:
    StringBuffer() : buffer(nullptr), capacidad(0), longitud(0) {
        redimensionar(64);
    }

    StringBuffer(const char* str) : buffer(nullptr), capacidad(0), longitud(0) {
        if (str != nullptr) {
            redimensionar(strlen(str) + 1);
            strcpy(buffer, str);
            longitud = strlen(str);
        }
    }

    StringBuffer(const StringBuffer& otro) : buffer(nullptr), capacidad(0), longitud(0) {
        redimensionar(otro.capacidad);
        strcpy(buffer, otro.buffer);
        longitud = otro.longitud;
    }

    ~StringBuffer() {
        delete[] buffer;
    }

    StringBuffer& operator=(const StringBuffer& otro) {
        if (this != &otro) {
            delete[] buffer;
            buffer = nullptr;
            redimensionar(otro.capacidad);
            strcpy(buffer, otro.buffer);
            longitud = otro.longitud;
        }
        return *this;
    }

    void agregar(char c) {
        if (longitud + 2 >= capacidad) redimensionar(capacidad * 2);
        buffer[longitud++] = c;
        buffer[longitud] = '\0';
    }

    void agregar(const char* str) {
        if (!str) return;
        int len = strlen(str);
        if (longitud + len + 1 >= capacidad) redimensionar((longitud + len + 1) * 2);
        strcat(buffer, str);
        longitud += len;
    }

    void limpiar() {
        longitud = 0;
        if (buffer) buffer[0] = '\0';
    }

    const char* obtener() const { return buffer; }
    int obtenerLongitud() const { return longitud; }
    bool vacio() const { return longitud == 0; }
};

template <typename T>
class FileRepository {
private:
    char* ruta;

    bool leerLinea(std::ifstream& archivo, StringBuffer& linea) {
        char c;
        bool leido = false;
        while (archivo.get(c)) {
            if (c == '\n') break;
            if (c != '\r') {
                linea.agregar(c);
                leido = true;
            }
        }
        return leido || !linea.vacio();
    }

public:
    FileRepository(const char* rutaArchivo) {
        ruta = new char[strlen(rutaArchivo) + 1];
        strcpy(ruta, rutaArchivo);
    }

    FileRepository(const FileRepository& otro) {
        ruta = new char[strlen(otro.ruta) + 1];
        strcpy(ruta, otro.ruta);
    }

    ~FileRepository() {
        delete[] ruta;
    }

    FileRepository& operator=(const FileRepository& otro) {
        if (this != &otro) {
            delete[] ruta;
            ruta = new char[strlen(otro.ruta) + 1];
            strcpy(ruta, otro.ruta);
        }
        return *this;
    }

    void guardar(const T& objeto) {
        std::ofstream archivo(ruta, std::ios::app);
        if (!archivo.is_open()) throw std::runtime_error("Error al abrir archivo");

        char* csv = objeto.toCSV();
        archivo << csv << "\n";
        delete[] csv;
        archivo.close();
    }

    void guardarTodos(const Lista<T>& objetos) {
        std::ofstream archivo(ruta, std::ios::trunc);
        if (!archivo.is_open()) throw std::runtime_error("Error al abrir archivo");

        typename Lista<T>::Iterador it = objetos.obtenerIterador();
        while (it.haySiguiente()) {
            T& obj = it.siguiente();
            char* csv = obj.toCSV();
            archivo << csv << "\n";
            delete[] csv;
        }
        archivo.close();
    }

    Lista<T> cargar() {
        Lista<T> resultados;
        std::ifstream archivo(ruta);
        if (!archivo.is_open()) return resultados;

        StringBuffer linea;
        while (leerLinea(archivo, linea)) {
            if (!linea.vacio()) {
                try {
                    T obj = T::fromCSV(linea.obtener());
                    resultados.insertar(obj);
                } catch (const std::exception& e) {
                    // Ignorar línea corrupta
                }
            }
            linea.limpiar();
        }
        archivo.close();
        return resultados;
    }

    void limpiarArchivo() {
        std::ofstream archivo(ruta, std::ios::trunc);
        archivo.close();
    }

    bool existe() {
        std::ifstream archivo(ruta);
        bool existe = archivo.is_open();
        archivo.close();
        return existe;
    }
};

#endif // FILE_REPOSITORY_H
