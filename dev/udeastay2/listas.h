#ifndef LISTAS_H
#define LISTAS_H

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
    ~Lista();

    void insertar(const T& dato);
    bool vacia() const { return cabeza == nullptr; }
    int tamano() const { return longitud; }

    // Iterador para recorrer la lista
    class Iterador {
        Nodo* actual;
    public:
        Iterador(Nodo* nodo) : actual(nodo) {}
        bool haySiguiente() const { return actual != nullptr; }
        T& siguiente() {
            T& dato = actual->dato;
            actual = actual->siguiente;
            return dato;
        }
    };

    Iterador obtenerIterador() { return Iterador(cabeza); }
};

// Implementación de métodos (incluida en el .h por ser plantilla)
template <typename T>
Lista<T>::~Lista() {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
}

template <typename T>
void Lista<T>::insertar(const T& dato) {
    Nodo* nuevoNodo = new Nodo(dato);
    nuevoNodo->siguiente = cabeza;
    cabeza = nuevoNodo;
    longitud++;
}

#endif // LISTAS_H
