#ifndef CLASES_H
#define CLASES_H

#include <stdexcept>
#include <cstring>
#include "listas.h"

// ==================== Clase Fecha ====================
class Fecha {
private:
    int dia;
    int mes;
    int año;

    bool esBisiesto() const;
    int diasEnMes() const;
    void validar() const;

public:
    Fecha() : dia(1), mes(1), año(2025) {} // Constructor por defecto
    Fecha(int d, int m, int a);
    Fecha(const Fecha& otra) = default;
    Fecha& operator=(const Fecha& otra) = default;

    bool operator==(const Fecha& otra) const;
    bool operator<(const Fecha& otra) const;
    bool operator>(const Fecha& otra) const;
    bool operator<=(const Fecha& otra) const;
    bool operator>=(const Fecha& otra) const;

    Fecha sumarDias(int n) const;
    int diferenciaDias(const Fecha& otra) const;
    void mostrar() const;

    // Getters
    int getDia() const { return dia; }
    int getMes() const { return mes; }
    int getAño() const { return año; }

    // Métodos para serialización
    char* toCSV() const;
    static Fecha fromCSV(const char* linea);
};

// ==================== Clase Alojamiento ====================

class Alojamiento {
private:
    struct Rango {
        Fecha inicio{};
        Fecha fin{};
        char* codigoReserva = nullptr;

        Rango(const Fecha& i, const Fecha& f, const char* cod);
        ~Rango();

        std::string toCSV() const;
        static Rango fromCSV(const std::string& linea);
    };

    struct NodoRango {
        Rango dato;
        NodoRango* siguiente;
        NodoRango(const Rango& r, NodoRango* sig = nullptr);
    };

    char* codigo;
    char* nombre;
    char* departamento;
    char* municipio;
    char* direccion;
    double precioPorNoche;
    NodoRango* rangosReservados;

public:
    Alojamiento(const char* cod, const char* nom, const char* dep,
                const char* mun, const char* dir, double precio);
    Alojamiento(const Alojamiento& otro);
    ~Alojamiento();

    void eliminarReserva(const char* codigoReserva);
    bool disponible(const Fecha& inicio, int noches) const;
    void reservar(const Fecha& inicio, int noches, const char* codigoReserva);
    const char* getCodigo() const;
    const char* getNombre() const;
    const char* getMunicipio() const;
    double getPrecioPorNoche() const;

    std::string toCSV() const;
    static Alojamiento fromCSV(const std::string& linea);
};

// ==================== Enum Estado Reserva ====================
enum class EstadoReserva { Activa, Anulada, Historica };

// ==================== Clase Reserva ====================
class Reserva {
private:
    char* codigo;
    Fecha fechaEntrada;
    int duracionNoches;
    char* codigoAlojamiento; // ✅ Cambio: usar código en lugar de puntero
    char* documentoHuesped;
    EstadoReserva estado;
    char* metodoPago;
    Fecha fechaPago;
    double monto;
    char* anotaciones;

public:
    Reserva(const char* cod, const Fecha& entrada, int duracion,
            const char* codAlojamiento, const char* docHuesped,
            const char* metodo, const Fecha& fPago, double monto,
            const char* anot = "");
    Reserva(const Reserva& otra);
    Reserva& operator=(const Reserva& otra);
    ~Reserva();

    void anular();
    void moverAHistorico();
    bool estaActiva() const;
    bool esHistorica() const;
    bool conflictoCon(const Fecha& inicio, const Fecha& fin) const;

    // Getters
    const char* getCodigo() const;
    const char* getCodigoAlojamiento() const;
    const char* getDocumentoHuesped() const;
    EstadoReserva getEstado() const;
    Fecha getFechaEntrada() const;
    Fecha getFechaFin() const;
    int getDuracionNoches() const;
    double getMonto() const;
    const char* getAnotaciones() const;
    const char* getMetodoPago() const;
    Fecha getFechaPago() const;

    // Operadores de comparación
    bool operator==(const Reserva& otra) const;

    // Métodos para serialización
    char* toCSV() const;
    static Reserva fromCSV(const char* linea);

    // Método auxiliar para convertir estado a string
    static const char* estadoToString(EstadoReserva estado);
    static EstadoReserva stringToEstado(const char* str);
};

// ==================== Template Usuario ====================
template<typename TPolicy>
class Usuario {
private:
    char* documento;
    int antiguedadMeses;
    double puntuacion;
    Lista<char*> coleccion; // Lista de códigos (alojamientos o reservas)

    void limpiarColeccion();
    void copiarColeccion(const Lista<char*>& otra);

public:
    Usuario(const char* doc, int antiguedad, double punt);
    Usuario(const Usuario& otro);
    Usuario& operator=(const Usuario& otro);
    ~Usuario();

    // Getters
    const char* getDocumento() const;
    int getAntiguedad() const;
    double getPuntuacion() const;
    const Lista<char*>& getColeccion() const;

    // Setters
    void setPuntuacion(double nuevaPunt);
    void setAntiguedad(int nuevaAnt);

    // Métodos específicos de la colección
    void agregarItem(const char* item);
    bool eliminarItem(const char* item);
    bool tieneItem(const char* item) const;
    int cantidadItems() const;

    // Operadores de comparación
    bool operator==(const Usuario& otro) const;

    // Métodos para serialización
    char* toCSV() const;
    static Usuario fromCSV(const char* linea);
};

// ==================== Policies ====================
struct HostPolicy {
    static const char* getTipo() { return "HOST"; }
    static const char* getNombreColeccion() { return "alojamientos"; }
};

struct GuestPolicy {
    static const char* getTipo() { return "GUEST"; }
    static const char* getNombreColeccion() { return "reservas"; }
};

// Alias para facilitar el uso
using Anfitrion = Usuario<HostPolicy>;
using Huesped = Usuario<GuestPolicy>;

// ==================== Implementación Template Usuario ====================
template<typename TPolicy>
Usuario<TPolicy>::Usuario(const char* doc, int antiguedad, double punt)
    : documento(nullptr), antiguedadMeses(antiguedad), puntuacion(punt) {
    if (doc == nullptr) throw std::invalid_argument("Documento no puede ser nulo");
    if (antiguedad < 0) throw std::invalid_argument("Antigüedad no puede ser negativa");
    if (punt < 0.0 || punt > 5.0) throw std::invalid_argument("Puntuación debe estar entre 0 y 5");

    documento = new char[strlen(doc) + 1];
    strcpy(documento, doc);
}

template<typename TPolicy>
Usuario<TPolicy>::Usuario(const Usuario& otro)
    : documento(nullptr), antiguedadMeses(otro.antiguedadMeses), puntuacion(otro.puntuacion) {
    documento = new char[strlen(otro.documento) + 1];
    strcpy(documento, otro.documento);
    copiarColeccion(otro.coleccion);
}

template<typename TPolicy>
Usuario<TPolicy>& Usuario<TPolicy>::operator=(const Usuario& otro) {
    if (this != &otro) {
        delete[] documento;
        limpiarColeccion();

        documento = new char[strlen(otro.documento) + 1];
        strcpy(documento, otro.documento);
        antiguedadMeses = otro.antiguedadMeses;
        puntuacion = otro.puntuacion;
        copiarColeccion(otro.coleccion);
    }
    return *this;
}

template<typename TPolicy>
Usuario<TPolicy>::~Usuario() {
    delete[] documento;
    limpiarColeccion();
}

template<typename TPolicy>
void Usuario<TPolicy>::limpiarColeccion() {
    Lista<char*>::Iterador it = coleccion.obtenerIterador();
    while (it.haySiguiente()) {
        char* item = it.siguiente();
        delete[] item;
    }
    coleccion = Lista<char*>(); // Limpiar la lista
}

template<typename TPolicy>
void Usuario<TPolicy>::copiarColeccion(const Lista<char*>& otra) {
    Lista<char*>::Iterador it = otra.obtenerIterador();
    while (it.haySiguiente()) {
        const char* item = it.siguiente();
        char* copia = new char[strlen(item) + 1];
        strcpy(copia, item);
        coleccion.insertar(copia);
    }
}

template<typename TPolicy>
const char* Usuario<TPolicy>::getDocumento() const { return documento; }

template<typename TPolicy>
int Usuario<TPolicy>::getAntiguedad() const { return antiguedadMeses; }

template<typename TPolicy>
double Usuario<TPolicy>::getPuntuacion() const { return puntuacion; }

template<typename TPolicy>
const Lista<char*>& Usuario<TPolicy>::getColeccion() const { return coleccion; }

template<typename TPolicy>
void Usuario<TPolicy>::setPuntuacion(double nuevaPunt) {
    if (nuevaPunt < 0.0 || nuevaPunt > 5.0)
        throw std::invalid_argument("Puntuación debe estar entre 0 y 5");
    puntuacion = nuevaPunt;
}

template<typename TPolicy>
void Usuario<TPolicy>::setAntiguedad(int nuevaAnt) {
    if (nuevaAnt < 0) throw std::invalid_argument("Antigüedad no puede ser negativa");
    antiguedadMeses = nuevaAnt;
}

template<typename TPolicy>
void Usuario<TPolicy>::agregarItem(const char* item) {
    if (item == nullptr) return;
    if (!tieneItem(item)) {
        char* copia = new char[strlen(item) + 1];
        strcpy(copia, item);
        coleccion.insertar(copia);
    }
}

template<typename TPolicy>
bool Usuario<TPolicy>::eliminarItem(const char* item) {
    if (item == nullptr) return false;

    Lista<char*>::Iterador it = coleccion.obtenerIterador();
    int indice = 0;
    while (it.haySiguiente()) {
        char* actual = it.siguiente();
        if (strcmp(actual, item) == 0) {
            delete[] actual;
            return coleccion.eliminarPorIndice(indice);
        }
        indice++;
    }
    return false;
}

template<typename TPolicy>
bool Usuario<TPolicy>::tieneItem(const char* item) const {
    if (item == nullptr) return false;

    Lista<char*>::Iterador it = coleccion.obtenerIterador();
    while (it.haySiguiente()) {
        const char* actual = it.siguiente();
        if (strcmp(actual, item) == 0) {
            return true;
        }
    }
    return false;
}

template<typename TPolicy>
int Usuario<TPolicy>::cantidadItems() const {
    return coleccion.tamano();
}

template<typename TPolicy>
bool Usuario<TPolicy>::operator==(const Usuario& otro) const {
    return strcmp(documento, otro.documento) == 0;
}

#endif // CLASES_H
