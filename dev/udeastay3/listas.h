#ifndef LISTAS_H
#define LISTAS_H

#include <stdexcept>
#include <cstring>

// ================================================================
// Versión genérica para tipos que NO sean const char*
// ================================================================
template <typename T>
class Lista {
private:
    struct Nodo {
        T dato;
        Nodo* siguiente;
        Nodo(const T& dato) : dato(dato), siguiente(nullptr) {}
    };

    Nodo* cabeza;
    int longitud;

public:
    Lista() : cabeza(nullptr), longitud(0) {}

    // Constructor de copia
    Lista(const Lista& otra) : cabeza(nullptr), longitud(0) {
        Nodo* actual = otra.cabeza;
        while (actual != nullptr) {
            insertar(actual->dato);
            actual = actual->siguiente;
        }
    }

    // Operador de asignación
    Lista& operator=(const Lista& otra) {
        if (this != &otra) {
            limpiar();
            Nodo* actual = otra.cabeza;
            while (actual != nullptr) {
                insertar(actual->dato);
                actual = actual->siguiente;
            }
        }
        return *this;
    }

    ~Lista() { limpiar(); }

    void insertar(const T& dato) {
        Nodo* nuevoNodo = new Nodo(dato);
        nuevoNodo->siguiente = cabeza;
        cabeza = nuevoNodo;
        longitud++;
    }

    bool eliminar(const T& dato) {
        if (cabeza == nullptr) return false;

        if (cabeza->dato == dato) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            longitud--;
            return true;
        }

        Nodo* actual = cabeza;
        while (actual->siguiente != nullptr && !(actual->siguiente->dato == dato)) {
            actual = actual->siguiente;
        }

        if (actual->siguiente != nullptr) {
            Nodo* temp = actual->siguiente;
            actual->siguiente = temp->siguiente;
            delete temp;
            longitud--;
            return true;
        }
        return false;
    }

    // Métodos const y no-const para obtener()
    T& obtener(int indice) {
        if (indice < 0 || indice >= longitud) throw std::out_of_range("Índice inválido");
        Nodo* actual = cabeza;
        for (int i = 0; i < indice; i++) actual = actual->siguiente;
        return actual->dato;
    }

    const T& obtener(int indice) const {
        if (indice < 0 || indice >= longitud) throw std::out_of_range("Índice inválido");
        Nodo* actual = cabeza;
        for (int i = 0; i < indice; i++) actual = actual->siguiente;
        return actual->dato;
    }

    int tamano() const { return longitud; }
    bool vacia() const { return cabeza == nullptr; }

    void limpiar() {
        while (cabeza != nullptr) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
        longitud = 0;
    }

    class Iterador {
        Nodo* actual;
    public:
        Iterador(Nodo* nodo) : actual(nodo) {}
        bool haySiguiente() const { return actual != nullptr; }
        T& siguiente() {
            if (!haySiguiente()) throw std::runtime_error("No hay más elementos");
            T& dato = actual->dato;
            actual = actual->siguiente;
            return dato;
        }
    };

    Iterador obtenerIterador() { return Iterador(cabeza); }
};

// ================================================================
// Especialización para const char* (manejo seguro de cadenas)
// ================================================================
template <>
class Lista<const char*> {
private:
    struct Nodo {
        char* dato;
        Nodo* siguiente;

        Nodo(const char* dato) : siguiente(nullptr) {
            this->dato = new char[strlen(dato) + 1];
            strcpy(this->dato, dato);
        }

        ~Nodo() {
            delete[] dato;
        }
    };

    Nodo* cabeza;
    int longitud;

public:
    Lista() : cabeza(nullptr), longitud(0) {}

    // Constructor de copia
    Lista(const Lista& otra) : cabeza(nullptr), longitud(0) {
        Nodo* actual = otra.cabeza;
        while (actual != nullptr) {
            insertar(actual->dato);
            actual = actual->siguiente;
        }
    }

    // Operador de asignación
    Lista& operator=(const Lista& otra) {
        if (this != &otra) {
            limpiar();
            Nodo* actual = otra.cabeza;
            while (actual != nullptr) {
                insertar(actual->dato);
                actual = actual->siguiente;
            }
        }
        return *this;
    }

    ~Lista() { limpiar(); }

    void insertar(const char* dato) {
        Nodo* nuevoNodo = new Nodo(dato);
        nuevoNodo->siguiente = cabeza;
        cabeza = nuevoNodo;
        longitud++;
    }

    bool eliminar(const char* dato) {
        if (cabeza == nullptr) return false;

        if (strcmp(cabeza->dato, dato) == 0) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            longitud--;
            return true;
        }

        Nodo* actual = cabeza;
        while (actual->siguiente != nullptr &&
               strcmp(actual->siguiente->dato, dato) != 0) {
            actual = actual->siguiente;
        }

        if (actual->siguiente != nullptr) {
            Nodo* temp = actual->siguiente;
            actual->siguiente = temp->siguiente;
            delete temp;
            longitud--;
            return true;
        }
        return false;
    }

    // Método const corregido
    const char* obtener(int indice) const {
        if (indice < 0 || indice >= longitud) throw std::out_of_range("Índice inválido");
        Nodo* actual = cabeza;
        for (int i = 0; i < indice; i++) actual = actual->siguiente;
        return actual->dato;
    }

    int tamano() const { return longitud; }
    bool vacia() const { return cabeza == nullptr; }

    void limpiar() {
        while (cabeza != nullptr) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
        longitud = 0;
    }

    class Iterador {
        Nodo* actual;
    public:
        Iterador(Nodo* nodo) : actual(nodo) {}
        bool haySiguiente() const { return actual != nullptr; }
        const char* siguiente() {
            if (!haySiguiente()) throw std::runtime_error("No hay más elementos");
            const char* dato = actual->dato;
            actual = actual->siguiente;
            return dato;
        }
    };

    Iterador obtenerIterador() { return Iterador(cabeza); }
};

#endif // LISTAS_H
