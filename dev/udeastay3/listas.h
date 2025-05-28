#ifndef LISTAS_H
#define LISTAS_H

#include <stdexcept>
#include <cstring>

// Versión genérica para tipos que no sean cadenas
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

    void copiar(const Lista& otra) {
        Nodo* actual = otra.cabeza;
        while (actual != nullptr) {
            insertar(actual->dato);
            actual = actual->siguiente;
        }
    }

public:
    Lista() : cabeza(nullptr), longitud(0) {}

    Lista(const Lista& otra) : cabeza(nullptr), longitud(0) {
        copiar(otra);
    }

    Lista& operator=(const Lista& otra) {
        if (this != &otra) {
            limpiar();
            copiar(otra);
        }
        return *this;
    }

    ~Lista() { limpiar(); }

    void insertar(const T& dato) {
        Nodo* nuevo = new Nodo(dato);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        longitud++;
    }

    bool eliminar(const T& dato) {
        if (!cabeza) return false;

        if (cabeza->dato == dato) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            longitud--;
            return true;
        }

        Nodo* actual = cabeza;
        while (actual->siguiente && actual->siguiente->dato != dato) {
            actual = actual->siguiente;
        }

        if (actual->siguiente) {
            Nodo* temp = actual->siguiente;
            actual->siguiente = temp->siguiente;
            delete temp;
            longitud--;
            return true;
        }
        return false;
    }

    bool eliminarEnIndice(int index) {
        if (index < 0 || index >= longitud) {
            return false;
        }

        if (index == 0) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            longitud--;
            return true;
        }

        Nodo* actual = cabeza;
        for (int i = 0; i < index - 1; i++) {
            actual = actual->siguiente;
        }

        Nodo* temp = actual->siguiente;
        actual->siguiente = temp->siguiente;
        delete temp;
        longitud--;
        return true;
    }

    void limpiar() {
        while (cabeza) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
        longitud = 0;
    }

    int tamano() const { return longitud; }
    bool vacia() const { return cabeza == nullptr; }

    T& obtener(int index) {
        if (index < 0 || index >= longitud) {
            throw std::out_of_range("Índice fuera de rango");
        }

        Nodo* actual = cabeza;
        for (int i = 0; i < index; i++) {
            actual = actual->siguiente;
        }
        return actual->dato;
    }

    const T& obtener(int index) const {
        if (index < 0 || index >= longitud) {
            throw std::out_of_range("Índice fuera de rango");
        }

        Nodo* actual = cabeza;
        for (int i = 0; i < index; i++) {
            actual = actual->siguiente;
        }
        return actual->dato;
    }


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

    class ConstIterador {
        const Nodo* actual;
    public:
        ConstIterador(const Nodo* nodo) : actual(nodo) {}
        bool haySiguiente() const { return actual != nullptr; }
        const T& siguiente() {
            const T& dato = actual->dato;
            actual = actual->siguiente;
            return dato;
        }
    };

    ConstIterador obtenerIterador() const { return ConstIterador(cabeza); }
};

// Especialización para manejo seguro de char*
template <>
class Lista<char*> {
private:
    struct Nodo {
        char* dato;
        Nodo* siguiente;

        Nodo(const char* dato) {
            this->dato = new char[strlen(dato) + 1];
            strcpy(this->dato, dato);
        }

        ~Nodo() { delete[] dato; }
    };

    Nodo* cabeza;
    int longitud;

    void copiar(const Lista& otra) {
        Nodo* actual = otra.cabeza;
        while (actual != nullptr) {
            insertar(actual->dato);
            actual = actual->siguiente;
        }
    }

public:
    Lista() : cabeza(nullptr), longitud(0) {}

    Lista(const Lista& otra) : cabeza(nullptr), longitud(0) {
        copiar(otra);
    }

    Lista& operator=(const Lista& otra) {
        if (this != &otra) {
            limpiar();
            copiar(otra);
        }
        return *this;
    }

    ~Lista() { limpiar(); }

    void insertar(const char* dato) {
        Nodo* nuevo = new Nodo(dato);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
        longitud++;
    }

    bool eliminar(const char* dato) {
        if (!cabeza) return false;

        if (strcmp(cabeza->dato, dato) == 0) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            longitud--;
            return true;
        }

        Nodo* actual = cabeza;
        while (actual->siguiente && strcmp(actual->siguiente->dato, dato) != 0) {
            actual = actual->siguiente;
        }

        if (actual->siguiente) {
            Nodo* temp = actual->siguiente;
            actual->siguiente = temp->siguiente;
            delete temp;
            longitud--;
            return true;
        }
        return false;
    }

    bool eliminarEnIndice(int index) {
        if (index < 0 || index >= longitud) {
            return false;
        }

        if (index == 0) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            longitud--;
            return true;
        }

        Nodo* actual = cabeza;
        for (int i = 0; i < index - 1; i++) {
            actual = actual->siguiente;
        }

        Nodo* temp = actual->siguiente;
        actual->siguiente = temp->siguiente;
        delete temp;
        longitud--;
        return true;
    }

    void limpiar() {
        while (cabeza) {
            Nodo* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
        }
        longitud = 0;
    }

    int tamano() const { return longitud; }
    bool vacia() const { return cabeza == nullptr; }

    char* obtener(int index) {
        if (index < 0 || index >= longitud) {
            throw std::out_of_range("Índice fuera de rango");
        }

        Nodo* actual = cabeza;
        for (int i = 0; i < index; i++) {
            actual = actual->siguiente;
        }
        return actual->dato;
    }

    const char* obtener(int index) const {
        if (index < 0 || index >= longitud) {
            throw std::out_of_range("Índice fuera de rango");
        }

        Nodo* actual = cabeza;
        for (int i = 0; i < index; i++) {
            actual = actual->siguiente;
        }
        return actual->dato;
    }

    class Iterador {
        Nodo* actual;
    public:
        Iterador(Nodo* nodo) : actual(nodo) {}
        bool haySiguiente() const { return actual != nullptr; }
        char* siguiente() {
            char* dato = actual->dato;
            actual = actual->siguiente;
            return dato;
        }
    };

    Iterador obtenerIterador() { return Iterador(cabeza); }

    class ConstIterador {
        const Nodo* actual;
    public:
        ConstIterador(const Nodo* nodo) : actual(nodo) {}
        bool haySiguiente() const { return actual != nullptr; }
        const char* siguiente() {
            const char* dato = actual->dato;
            actual = actual->siguiente;
            return dato;
        }
    };

    ConstIterador obtenerIterador() const { return ConstIterador(cabeza); }
};

#endif // LISTAS_H
