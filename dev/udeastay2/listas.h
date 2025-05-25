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

    // Constructor de copia
    Lista(const Lista& otra) : cabeza(nullptr), longitud(0) {
        Nodo* actualOtra = otra.cabeza;
        Nodo** actualThis = &cabeza;
        while (actualOtra != nullptr) {
            *actualThis = new Nodo(actualOtra->dato);
            actualThis = &(*actualThis)->siguiente;
            actualOtra = actualOtra->siguiente;
            longitud++;
        }
    }

    // Operador de asignación
    Lista& operator=(const Lista& otra) {
        if (this != &otra) {
            limpiar();
            Nodo* actualOtra = otra.cabeza;
            Nodo** actualThis = &cabeza;
            while (actualOtra != nullptr) {
                *actualThis = new Nodo(actualOtra->dato);
                actualThis = &(*actualThis)->siguiente;
                actualOtra = actualOtra->siguiente;
                longitud++;
            }
        }
        return *this;
    }

    ~Lista() { limpiar(); }

    void insertar(const T& dato);
    void insertarOrdenado(const T& dato); // Para inserción ordenada
    bool eliminar(const T& dato);
    bool eliminarPorIndice(int indice);
    T* buscar(const T& dato);
    T& obtener(int indice);
    const T& obtener(int indice) const;
    bool vacia() const { return cabeza == nullptr; }
    int tamano() const { return longitud; }
    void limpiar();

    // Iterador para recorrer la lista
    class Iterador {
        Nodo* actual;
    public:
        Iterador(Nodo* nodo) : actual(nodo) {}
        bool haySiguiente() const { return actual != nullptr; }
        T& siguiente() {
            if (actual == nullptr) throw std::runtime_error("No hay más elementos");
            T& dato = actual->dato;
            actual = actual->siguiente;
            return dato;
        }
        T& actual_dato() {
            if (actual == nullptr) throw std::runtime_error("Iterador inválido");
            return actual->dato;
        }
    };

    Iterador obtenerIterador() { return Iterador(cabeza); }
    const Iterador obtenerIterador() const { return Iterador(cabeza); }
};

// ==================== Implementación de métodos ====================
template <typename T>
void Lista<T>::limpiar() {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
    cabeza = nullptr;
    longitud = 0;
}

template <typename T>
void Lista<T>::insertar(const T& dato) {
    Nodo* nuevoNodo = new Nodo(dato);
    nuevoNodo->siguiente = cabeza;
    cabeza = nuevoNodo;
    longitud++;
}

template <typename T>
void Lista<T>::insertarOrdenado(const T& dato) {
    Nodo* nuevoNodo = new Nodo(dato);

    // Si la lista está vacía o el nuevo elemento es menor que el primero
    if (cabeza == nullptr || dato < cabeza->dato) {
        nuevoNodo->siguiente = cabeza;
        cabeza = nuevoNodo;
        longitud++;
        return;
    }

    // Buscar la posición correcta
    Nodo* actual = cabeza;
    while (actual->siguiente != nullptr && actual->siguiente->dato < dato) {
        actual = actual->siguiente;
    }

    nuevoNodo->siguiente = actual->siguiente;
    actual->siguiente = nuevoNodo;
    longitud++;
}

template <typename T>
bool Lista<T>::eliminar(const T& dato) {
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

template <typename T>
bool Lista<T>::eliminarPorIndice(int indice) {
    if (indice < 0 || indice >= longitud) return false;

    if (indice == 0) {
        Nodo* temp = cabeza;
        cabeza = cabeza->siguiente;
        delete temp;
        longitud--;
        return true;
    }

    Nodo* actual = cabeza;
    for (int i = 0; i < indice - 1; i++) {
        actual = actual->siguiente;
    }

    Nodo* temp = actual->siguiente;
    actual->siguiente = temp->siguiente;
    delete temp;
    longitud--;
    return true;
}

template <typename T>
T* Lista<T>::buscar(const T& dato) {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        if (actual->dato == dato) {
            return &(actual->dato);
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

template <typename T>
T& Lista<T>::obtener(int indice) {
    if (indice < 0 || indice >= longitud) {
        throw std::out_of_range("Índice fuera de rango");
    }

    Nodo* actual = cabeza;
    for (int i = 0; i < indice; i++) {
        actual = actual->siguiente;
    }
    return actual->dato;
}

template <typename T>
const T& Lista<T>::obtener(int indice) const {
    if (indice < 0 || indice >= longitud) {
        throw std::out_of_range("Índice fuera de rango");
    }

    Nodo* actual = cabeza;
    for (int i = 0; i < indice; i++) {
        actual = actual->siguiente;
    }
    return actual->dato;
}

#endif // LISTAS_H
