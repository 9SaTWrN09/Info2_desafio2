#ifndef LISTAS_H
#define LISTAS_H

#include <stdexcept>

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

    T& obtener(int indice) {
        if (indice < 0 || indice >= longitud) throw std::out_of_range("Índice inválido");

        Nodo* actual = cabeza;
        for (int i = 0; i < indice; i++) {
            actual = actual->siguiente;
        }
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

    // Iterador
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

#endif // LISTAS_H
