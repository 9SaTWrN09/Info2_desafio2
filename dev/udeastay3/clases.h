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

    bool operator==(const Reserva& otra) const {
        return strcmp(codigo, otra.codigo) == 0;
    }

    bool operator!=(const Reserva& otra) const {
        return !(*this == otra);
    }

    void setEstado(EstadoReserva nuevoEstado) {
        estado = nuevoEstado;
    }

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
    Lista<char*> coleccion;

    void copiarDesde(const Usuario& otro) {
        documento = new char[strlen(otro.documento) + 1];
        strcpy(documento, otro.documento);

        contraseña = new char[strlen(otro.contraseña) + 1];
        strcpy(contraseña, otro.contraseña);

        antiguedadMeses = otro.antiguedadMeses;
        puntuacion = otro.puntuacion;

        typename Lista<char*>::ConstIterador it = otro.coleccion.obtenerIterador();
        while (it.haySiguiente()) {
            const char* item = it.siguiente();
            coleccion.insertar(item);
        }
    }

public:
    Usuario(const char* doc, const char* pass, int antiguedad, double punt)
        : documento(new char[strlen(doc) + 1]),
        contraseña(new char[strlen(pass) + 1]),
        antiguedadMeses(antiguedad),
        puntuacion(punt) {
        strcpy(documento, doc);
        strcpy(contraseña, pass);
    }

    Usuario(const Usuario& otro) {
        copiarDesde(otro);
    }

    Usuario& operator=(const Usuario& otro) {
        if (this != &otro) {
            delete[] documento;
            delete[] contraseña;
            coleccion.limpiar();
            copiarDesde(otro);
        }
        return *this;
    }

    ~Usuario() {
        delete[] documento;
        delete[] contraseña;
    }

    void agregarItem(const char* item) {
        coleccion.insertar(item);
    }

    bool eliminarItem(const char* item) {
        return coleccion.eliminar(item);
    }

    const char* getDocumento() const { return documento; }
    const char* getContraseña() const { return contraseña; }
    int getAntiguedad() const { return antiguedadMeses; }
    double getPuntuacion() const { return puntuacion; }
    const Lista<char*>& getColeccion() const { return coleccion; }

    char* toCSV() const {
        std::ostringstream oss;
        oss << documento << ","
            << contraseña << ","
            << antiguedadMeses << ","
            << puntuacion << ","
            << TPolicy::getNombreColeccion();

        typename Lista<char*>::ConstIterador it = coleccion.obtenerIterador();
        while (it.haySiguiente()) {
            oss << "," << it.siguiente();
        }

        char* resultado = new char[oss.str().size() + 1];
        strcpy(resultado, oss.str().c_str());
        return resultado;
    }

    static Usuario fromCSV(const char* linea) {
        // Implementación segura similar a versiones anteriores
        // ...
    }
};

// Políticas
struct GuestPolicy {
    static const char* getNombreColeccion() { return "reservas"; }
};

using Huesped = Usuario<GuestPolicy>;

// ... [Código existente de Huesped] ...

// ==================== Política para Anfitrión ====================
struct HostPolicy {
    static const char* getNombreColeccion() { return "alojamientos"; }
};

// ==================== Especialización para Anfitrión ====================
template<>
class Usuario<HostPolicy> {
private:
    char* documento;
    char* contraseña;
    int antiguedadMeses;
    double puntuacion;
    Lista<Alojamiento> alojamientos;

    void copiarDesde(const Usuario<HostPolicy>& otro) {
        documento = new char[strlen(otro.documento) + 1];
        strcpy(documento, otro.documento);
        contraseña = new char[strlen(otro.contraseña) + 1];
        strcpy(contraseña, otro.contraseña);
        antiguedadMeses = otro.antiguedadMeses;
        puntuacion = otro.puntuacion;

        typename Lista<Alojamiento>::ConstIterador it = otro.alojamientos.obtenerIterador();
        while (it.haySiguiente()) {
            alojamientos.insertar(it.siguiente());
        }
    }

public:
    Usuario(const char* doc, const char* pass, int antiguedad, double punt)
        : documento(new char[strlen(doc) + 1]),
        contraseña(new char[strlen(pass) + 1]),
        antiguedadMeses(antiguedad),
        puntuacion(punt) {
        strcpy(documento, doc);
        strcpy(contraseña, pass);
    }

    Usuario(const Usuario<HostPolicy>& otro) { copiarDesde(otro); }

    Usuario<HostPolicy>& operator=(const Usuario<HostPolicy>& otro) {
        if (this != &otro) {
            delete[] documento;
            delete[] contraseña;
            alojamientos.limpiar();
            copiarDesde(otro);
        }
        return *this;
    }

    ~Usuario() {
        delete[] documento;
        delete[] contraseña;
    }

    void agregarAlojamiento(const Alojamiento& alojamiento) {
        alojamientos.insertar(alojamiento);
    }

    bool eliminarAlojamiento(const char* codigo) {
        for (int i = 0; i < alojamientos.tamano(); ++i) {
            if (strcmp(alojamientos.obtener(i).getCodigo(), codigo) == 0) {
                // CORRECTO: Usa eliminarEnIndice
                return alojamientos.eliminarEnIndice(i);
            }
        }
        return false;
    }



    Lista<Alojamiento> obtenerAlojamientos() const { return alojamientos; }

    char* toCSV() const {
        std::ostringstream oss;
        oss << documento << "," << contraseña << ","
            << antiguedadMeses << "," << puntuacion << ","
            << HostPolicy::getNombreColeccion();

        typename Lista<Alojamiento>::ConstIterador it = alojamientos.obtenerIterador();
        while (it.haySiguiente()) {
            char* csv = it.siguiente().toCSV();
            oss << "," << csv;
            delete[] csv;
        }

        char* resultado = new char[oss.str().size() + 1];
        strcpy(resultado, oss.str().c_str());
        return resultado;
    }
};

using Anfitrion = Usuario<HostPolicy>;

#endif // CLASES_H <-- Asegúrate de que esto esté al final

