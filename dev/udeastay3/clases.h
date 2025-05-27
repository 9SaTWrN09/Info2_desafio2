#ifndef CLASES_H
#define CLASES_H

#include <stdexcept>
#include <cstring>
#include "listas.h"
#include <string>
#include <sstream>

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
    Fecha() : dia(1), mes(1), año(2025) {}
    Fecha(int d, int m, int a);
    Fecha(const Fecha& otra) = default;

    bool operator==(const Fecha& otra) const;
    bool operator<(const Fecha& otra) const;
    bool operator>(const Fecha& otra) const;
    Fecha sumarDias(int n) const;
    int diferenciaDias(const Fecha& otra) const;

    // Getters
    int getDia() const { return dia; }
    int getMes() const { return mes; }
    int getAño() const { return año; }

    // Serialización
    char* toCSV() const;
    static Fecha fromCSV(const char* linea);
};

// ==================== Clase Alojamiento ====================
class Alojamiento {
private:
    struct Rango {
        Fecha inicio;
        Fecha fin;
        char* codigoReserva;

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

    // Getters
    const char* getCodigo() const { return codigo; }
    const char* getNombre() const { return nombre; }
    const char* getMunicipio() const { return municipio; }
    double getPrecioPorNoche() const { return precioPorNoche; }

    // Serialización
    char* toCSV() const;
    static Alojamiento fromCSV(const char* linea);
};

// ==================== Enum Estado Reserva ====================
enum class EstadoReserva { Activa, Anulada, Historica };

// ==================== Clase Reserva ====================
class Reserva {
private:
    char* codigo;
    Fecha fechaEntrada;
    int duracionNoches;
    char* codigoAlojamiento;  // Usar código en lugar de puntero
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
    bool estaActiva() const { return estado == EstadoReserva::Activa; }

    // Getters
    const char* getCodigo() const { return codigo; }
    const char* getCodigoAlojamiento() const { return codigoAlojamiento; }
    const char* getDocumentoHuesped() const { return documentoHuesped; }
    EstadoReserva getEstado() const { return estado; }
    Fecha getFechaEntrada() const { return fechaEntrada; }
    Fecha getFechaFin() const { return fechaEntrada.sumarDias(duracionNoches); }
    int getDuracionNoches() const { return duracionNoches; }
    double getMonto() const { return monto; }
    Fecha getFechaPago() const { return fechaPago;}

    // Serialización
    char* toCSV() const;
    static Reserva fromCSV(const char* linea);
};

// ==================== Template Usuario ====================
template<typename TPolicy>
class Usuario {
private:
    char* documento;
    char* contraseña;
    int antiguedadMeses;
    double puntuacion;
    Lista<const char*> coleccion;

    void limpiarColeccion() {
        while (!coleccion.vacia()) {
            delete[] coleccion.obtener(0);
            coleccion.eliminar(0);
        }
    }

    void copiarColeccion(Lista<char*>& otra) {
        for (int i = 0; i < otra.tamano(); ++i) {
            char* item = new char[strlen(otra.obtener(i)) + 1];
            strcpy(item, otra.obtener(i));
            coleccion.insertar(item);
        }
    }

public:
    // Constructor
    Usuario(const char* doc, const char* pass, int antiguedad, double punt)
        : documento(new char[strlen(doc) + 1]),
        contraseña(new char[strlen(pass) + 1]),
        antiguedadMeses(antiguedad),
        puntuacion(punt) {
        strcpy(documento, doc);
        strcpy(contraseña, pass);
    }

    // Constructor de copia
    Usuario(const Usuario& otro)
        : documento(new char[strlen(otro.documento) + 1]),
        contraseña(new char[strlen(otro.contraseña) + 1]),
        antiguedadMeses(otro.antiguedadMeses),
        puntuacion(otro.puntuacion) {
        strcpy(documento, otro.documento);
        strcpy(contraseña, otro.contraseña);
        copiarColeccion(otro.coleccion);
    }

    // Operador de asignación
    Usuario& operator=(const Usuario& otro) {
        if (this != &otro) {
            delete[] documento;
            delete[] contraseña;
            limpiarColeccion();

            documento = new char[strlen(otro.documento) + 1];
            strcpy(documento, otro.documento);
            contraseña = new char[strlen(otro.contraseña) + 1];
            strcpy(contraseña, otro.contraseña);
            antiguedadMeses = otro.antiguedadMeses;
            puntuacion = otro.puntuacion;
            copiarColeccion(otro.coleccion);
        }
        return *this;
    }

    // Destructor
    ~Usuario() {
        delete[] documento;
        delete[] contraseña;
        limpiarColeccion();
    }

    // Getters
    const char* getDocumento() const { return documento; }
    const char* getContraseña() const { return contraseña; }
    int getAntiguedad() const { return antiguedadMeses; }
    double getPuntuacion() const { return puntuacion; }
    const Lista<const char*>& getColeccion() const { return coleccion; }

    void agregarItem(const char* item) {
        coleccion.insertar(item);  // La lista maneja la copia
    }

    bool eliminarItem(const char* item) {
        for (int i = 0; i < coleccion.tamano(); ++i) {
            if (strcmp(coleccion.obtener(i), item) == 0) {
                char* ptr = coleccion.obtener(i);  // guardar puntero
                delete[] ptr;                      // liberar memoria
                coleccion.eliminar(ptr);           // eliminar por valor, no por índice
                return true;
            }
        }
        return false;
    }


    // Serialización
    char* toCSV() const {
        std::ostringstream oss;
        oss << documento << ","
            << contraseña << ","
            << antiguedadMeses << ","
            << puntuacion << ","
            << TPolicy::getNombreColeccion();

        char* resultado = new char[oss.str().size() + 1];
        strcpy(resultado, oss.str().c_str());
        return resultado;
    }

    static Usuario fromCSV(const char* linea) {
        std::istringstream iss(linea);
        std::string doc, pass, coleccion;
        int antiguedad;
        double punt;

        std::getline(iss, doc, ',');
        std::getline(iss, pass, ',');
        iss >> antiguedad;
        iss.ignore();
        iss >> punt;
        iss.ignore();
        std::getline(iss, coleccion);

        Usuario user(doc.c_str(), pass.c_str(), antiguedad, punt);
        return user;
    }
};

// Políticas para Usuario
struct HostPolicy {
    static const char* getTipo() { return "HOST"; }
    static const char* getNombreColeccion() { return "alojamientos"; }
};

struct GuestPolicy {
    static const char* getTipo() { return "GUEST"; }
    static const char* getNombreColeccion() { return "reservas"; }
};

using Anfitrion = Usuario<HostPolicy>;
using Huesped = Usuario<GuestPolicy>;

#endif // CLASES_H
