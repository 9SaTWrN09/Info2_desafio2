#ifndef FILE_REPOSITORY_H
#define FILE_REPOSITORY_H

#include "listas.h"
#include "clases.h"
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

    // Función auxiliar para leer una línea
    bool leerLinea(FILE* archivo, StringBuffer& linea) {
        char c;
        while ((c = fgetc(archivo))) {
            if (c == EOF) return !linea.vacio();
            if (c == '\n') return true;
            linea.agregar(c);
        }
        return false;
    }

public:
    FileRepository(const char* rutaArchivo) {
        ruta = new char[strlen(rutaArchivo) + 1];
        strcpy(ruta, rutaArchivo);
    }

    ~FileRepository() {
        delete[] ruta;
    }

    // Guarda un objeto en el archivo
    void guardar(const T& objeto) {
        FILE* archivo = fopen(ruta, "a");
        if (!archivo) return;

        char* csv = objeto.toCSV();
        fprintf(archivo, "%s\n", csv);
        delete[] csv;

        fclose(archivo);
    }

    // Guarda todos los objetos de una lista
    void guardarTodos(const Lista<T>& objetos) {
        FILE* archivo = fopen(ruta, "w");
        if (!archivo) return;

        auto it = objetos.obtenerIterador();
        while (it.haySiguiente()) {
            char* csv = it.siguiente().toCSV();
            fprintf(archivo, "%s\n", csv);
            delete[] csv;
        }

        fclose(archivo);
    }

    // Carga todos los objetos del archivo
    Lista<T> cargar() {
        Lista<T> resultados;
        FILE* archivo = fopen(ruta, "r");
        if (!archivo) return resultados;

        StringBuffer linea;
        while (leerLinea(archivo, linea)) {
            if (!linea.vacio()) {
                try {
                    T obj = T::fromCSV(linea.obtener());
                    resultados.insertar(obj);
                } catch (...) {
                    // Ignorar líneas corruptas
                }
            }
            linea.limpiar();
        }

        fclose(archivo);
        return resultados;
    }

    // Mueve una reserva a histórico
    void moverAHistorico(T& reserva) {
        // 1. Modificar el estado directamente
        reserva.setEstado(EstadoReserva::Historica);

        // 2. Guardar en histórico (usando la versión modificada)
        FileRepository<T> repoHist("reservas_historicas_test.dat");
        repoHist.guardar(reserva);

        // 3. Eliminar de activas
        Lista<T> activas = cargar();

        // Buscar y eliminar por código (más seguro que por comparación)
        bool encontrada = false;
        Lista<T> nuevasActivas;
        auto it = activas.obtenerIterador();

        while (it.haySiguiente()) {
            T r = it.siguiente();
            if (strcmp(r.getCodigo(), reserva.getCodigo()) == 0) {
                encontrada = true;
            } else {
                nuevasActivas.insertar(r);
            }
        }

        guardarTodos(nuevasActivas);
    }

};
#endif // FILE_REPOSITORY_H
