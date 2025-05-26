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
    int antiguedadMeses;
    double puntuacion;
    Lista<char*> coleccion;

    void limpiarColeccion();
    void copiarColeccion(const Lista<char*>& otra);

public:
    Usuario(const char* doc, int antiguedad, double punt);
    Usuario(const Usuario& otro);
    Usuario& operator=(const Usuario& otro);
    ~Usuario();

    // Getters/Setters
    const char* getDocumento() const { return documento; }
    int getAntiguedad() const { return antiguedadMeses; }
    double getPuntuacion() const { return puntuacion; }
    const Lista<char*>& getColeccion() const { return coleccion; }

    void agregarItem(const char* item);
    bool eliminarItem(const char* item);

    // Serialización
    char* toCSV() const;
    static Usuario fromCSV(const char* linea);
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
