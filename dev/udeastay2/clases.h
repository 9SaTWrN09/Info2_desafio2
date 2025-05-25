#ifndef CLASES_H
#define CLASES_H

#include <stdexcept>
#include <cstring>

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
    Fecha() = default;
    Fecha(int d, int m, int a);
    Fecha(const Fecha& otra) = default;
    Fecha& operator=(const Fecha& otra) = default;

    bool operator==(const Fecha& otra) const;
    bool operator<(const Fecha& otra) const;
    bool operator>(const Fecha& otra) const;
    Fecha sumarDias(int n) const;
    int diferenciaDias(const Fecha& otra) const;
    void mostrar() const;
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
};

// ==================== Clase Reserva ====================
enum class EstadoReserva { Activa, Anulada, Historica };

class Reserva {
private:
    char* codigo;
    Fecha fechaEntrada;
    int duracionNoches;
    Alojamiento* alojamiento;
    char* documentoHuesped;
    EstadoReserva estado;
    char* metodoPago;
    Fecha fechaPago;
    double monto;
    char* anotaciones;

public:
    Reserva(const char* cod, const Fecha& entrada, int duracion,
            Alojamiento* alo, const char* docHuesped,
            const char* metodo, const Fecha& fPago, double monto,
            const char* anot);
    Reserva(const Reserva& otra);
    ~Reserva();

    void anular();
    bool estaActiva() const;
    const char* getCodigo() const;
    Alojamiento* getAlojamiento() const;
    const char* getDocumentoHuesped() const;
    EstadoReserva getEstado() const;
    Fecha getFechaFin() const;
    int getDuracionNoches() const;
    double getMonto() const;
};

#endif // CLASES_H
