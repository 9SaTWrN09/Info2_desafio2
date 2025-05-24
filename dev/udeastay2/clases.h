#ifndef CLASES_H
#define CLASES_H

#include <stdexcept>
#include <cstring>

class Fecha {
private:
    int dia;
    int mes;
    int año;

    bool esBisiesto() const;
    int diasEnMes() const;
    void validar() const;

public:
    // Constructores y operadores
    Fecha() = default;  // Constructor por defecto
    Fecha(int d, int m, int a);
    Fecha(const Fecha& otra) = default;  // Constructor de copia automático
    Fecha& operator=(const Fecha& otra) = default;  // Operador de asignación automático

    // Métodos
    bool operator==(const Fecha& otra) const;
    bool operator<(const Fecha& otra) const;
    bool operator>(const Fecha& otra) const;
    Fecha sumarDias(int n) const;
    int diferenciaDias(const Fecha& otra) const;
    void mostrar() const;
};

class Alojamiento {
private:
    struct Rango {
        Fecha inicio{};  // Inicialización por defecto
        Fecha fin{};
        char* codigoReserva = nullptr;

        Rango(const Fecha& i, const Fecha& f, const char* cod)
            : inicio(i), fin(f) {
            codigoReserva = new char[strlen(cod) + 1];
            strcpy(codigoReserva, cod);
        }

        ~Rango() {
            delete[] codigoReserva;
        }
    };

    struct NodoRango {
        Rango dato;
        NodoRango* siguiente;

        NodoRango(const Rango& r, NodoRango* sig = nullptr)
            : dato(r.inicio, r.fin, r.codigoReserva), siguiente(sig) {}
    };

    // Miembros
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

    // Métodos
    bool disponible(const Fecha& inicio, int noches) const;
    void reservar(const Fecha& inicio, int noches, const char* codigoReserva);
    const char* getCodigo() const;
    const char* getNombre() const;
    const char* getMunicipio() const;
    double getPrecioPorNoche() const;
};



enum class EstadoReserva { Activa, Anulada, Historica };

class Alojamiento; // Declaración adelantada para evitar dependencia circular

class Reserva {
private:
    char* codigo;
    Fecha fechaEntrada;
    int duracionNoches;
    Alojamiento* alojamiento; // Puntero al alojamiento asociado
    char* documentoHuesped;
    EstadoReserva estado;
    char* metodoPago;
    Fecha fechaPago;
    double monto;
    char* anotaciones; // Buffer dinámico (hasta 1000 caracteres)

public:
    // Constructor principal
    Reserva(const char* cod, const Fecha& entrada, int duracion,
            Alojamiento* alo, const char* docHuesped,
            const char* metodo, const Fecha& fPago, double monto,
            const char* anot);

    // Constructor de copia (copia profunda)
    Reserva(const Reserva& otra);

    // Destructor
    ~Reserva();

    // Métodos
    void anular();
    bool estaActiva() const;
    const char* getCodigo() const;
    Alojamiento* getAlojamiento() const;
    const char* getDocumentoHuesped() const;
    EstadoReserva getEstado() const;
    Fecha getFechaFin() const;
    int getDuracionNoches() const;
};



#endif // CLASES_H
