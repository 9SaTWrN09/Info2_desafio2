#ifndef FILE_REPOSITORY_H
#define FILE_REPOSITORY_H

#include "listas.h"
#include <fstream>
#include <stdexcept>

// Clase auxiliar para manejo de strings sin STL
class StringBuffer {
private:
    char* buffer;
    int capacidad;
    int longitud;

    void redimensionar(int nuevaCapacidad);

public:
    StringBuffer();
    StringBuffer(const char* str);
    StringBuffer(const StringBuffer& otro);
    ~StringBuffer();

    StringBuffer& operator=(const StringBuffer& otro);
    void agregar(char c);
    void agregar(const char* str);
    void limpiar();

    const char* obtener() const { return buffer; }
    int obtenerLongitud() const { return longitud; }
    bool vacio() const { return longitud == 0; }
};

template <typename T>
class FileRepository {
private:
    char* ruta;

    // Método auxiliar para leer una línea del archivo
    bool leerLinea(std::ifstream& archivo, StringBuffer& linea);

public:
    FileRepository(const char* rutaArchivo);
    FileRepository(const FileRepository& otro);
    ~FileRepository();

    FileRepository& operator=(const FileRepository& otro);

    void guardar(const T& objeto);
    void guardarTodos(const Lista<T>& objetos);
    Lista<T> cargar();
    void limpiarArchivo();
    bool existe();
};

// ==================== Implementación StringBuffer ====================
inline StringBuffer::StringBuffer() : buffer(nullptr), capacidad(0), longitud(0) {
    redimensionar(64); // Capacidad inicial
}

inline StringBuffer::StringBuffer(const char* str) : buffer(nullptr), capacidad(0), longitud(0) {
    if (str != nullptr) {
        int len = strlen(str);
        redimensionar(len + 1);
        strcpy(buffer, str);
        longitud = len;
    } else {
        redimensionar(64);
    }
}

inline StringBuffer::StringBuffer(const StringBuffer& otro) : buffer(nullptr), capacidad(0), longitud(0) {
    redimensionar(otro.capacidad);
    strcpy(buffer, otro.buffer);
    longitud = otro.longitud;
}

inline StringBuffer::~StringBuffer() {
    delete[] buffer;
}

inline StringBuffer& StringBuffer::operator=(const StringBuffer& otro) {
    if (this != &otro) {
        delete[] buffer;
        buffer = nullptr;
        capacidad = 0;
        longitud = 0;
        redimensionar(otro.capacidad);
        strcpy(buffer, otro.buffer);
        longitud = otro.longitud;
    }
    return *this;
}

inline void StringBuffer::redimensionar(int nuevaCapacidad) {
    if (nuevaCapacidad <= capacidad) return;

    char* nuevoBuffer = new char[nuevaCapacidad];
    if (buffer != nullptr) {
        strcpy(nuevoBuffer, buffer);
        delete[] buffer;
    } else {
        nuevoBuffer[0] = '\0';
    }

    buffer = nuevoBuffer;
    capacidad = nuevaCapacidad;
}

inline void StringBuffer::agregar(char c) {
    if (longitud + 2 >= capacidad) {
        redimensionar(capacidad * 2);
    }
    buffer[longitud] = c;
    buffer[longitud + 1] = '\0';
    longitud++;
}

inline void StringBuffer::agregar(const char* str) {
    if (str == nullptr) return;
    int len = strlen(str);
    if (longitud + len + 1 >= capacidad) {
        redimensionar((longitud + len + 1) * 2);
    }
    strcat(buffer, str);
    longitud += len;
}

inline void StringBuffer::limpiar() {
    longitud = 0;
    if (buffer != nullptr) {
        buffer[0] = '\0';
    }
}

// ==================== Implementación FileRepository ====================
template <typename T>
FileRepository<T>::FileRepository(const char* rutaArchivo) {
    int len = strlen(rutaArchivo);
    ruta = new char[len + 1];
    strcpy(ruta, rutaArchivo);
}

template <typename T>
FileRepository<T>::FileRepository(const FileRepository& otro) {
    int len = strlen(otro.ruta);
    ruta = new char[len + 1];
    strcpy(ruta, otro.ruta);
}

template <typename T>
FileRepository<T>::~FileRepository() {
    delete[] ruta;
}

template <typename T>
FileRepository<T>& FileRepository<T>::operator=(const FileRepository& otro) {
    if (this != &otro) {
        delete[] ruta;
        int len = strlen(otro.ruta);
        ruta = new char[len + 1];
        strcpy(ruta, otro.ruta);
    }
    return *this;
}

template <typename T>
void FileRepository<T>::guardar(const T& objeto) {
    std::ofstream archivo(ruta, std::ios::app);
    if (!archivo.is_open()) {
        throw std::runtime_error("Error al abrir el archivo para escritura");
    }

    char* csvData = objeto.toCSV();
    archivo << csvData << "\n";
    delete[] csvData;
    archivo.close();
}

template <typename T>
void FileRepository<T>::guardarTodos(const Lista<T>& objetos) {
    std::ofstream archivo(ruta, std::ios::trunc); // Sobrescribe el archivo
    if (!archivo.is_open()) {
        throw std::runtime_error("Error al abrir el archivo para escritura");
    }

    Lista<T>::Iterador it = objetos.obtenerIterador();
    while (it.haySiguiente()) {
        const T& obj = it.siguiente();
        char* csvData = obj.toCSV();
        archivo << csvData << "\n";
        delete[] csvData;
    }
    archivo.close();
}

template <typename T>
Lista<T> FileRepository<T>::cargar() {
    Lista<T> objetos;
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        // Si el archivo no existe, retornamos lista vacía
        return objetos;
    }

    StringBuffer linea;
    while (leerLinea(archivo, linea)) {
        if (!linea.vacio()) {
            try {
                T objeto = T::fromCSV(linea.obtener());
                objetos.insertar(objeto);
            } catch (const std::exception& e) {
                // Ignorar líneas malformadas y continuar
                continue;
            }
        }
        linea.limpiar();
    }

    archivo.close();
    return objetos;
}

template <typename T>
bool FileRepository<T>::leerLinea(std::ifstream& archivo, StringBuffer& linea) {
    char c;
    bool lineaLeida = false;

    while (archivo.get(c)) {
        lineaLeida = true;
        if (c == '\n') {
            break;
        }
        if (c != '\r') { // Ignorar \r para compatibilidad Windows
            linea.agregar(c);
        }
    }

    return lineaLeida;
}

template <typename T>
void FileRepository<T>::limpiarArchivo() {
    std::ofstream archivo(ruta, std::ios::trunc);
    archivo.close();
}

template <typename T>
bool FileRepository<T>::existe() {
    std::ifstream archivo(ruta);
    bool existe = archivo.is_open();
    archivo.close();
    return existe;
}

#endif // FILE_REPOSITORY_H
