#include "clases.h"

#include <iostream>
#include <stdexcept>

    class Fecha {
private:
    int dia;
    int mes;
    int año;

    bool esBisiesto() const {
        return (año % 4 == 0 && año % 100 != 0) || (año % 400 == 0);
    }

    int diasEnMes() const {
        switch (mes) {
        case 2: return esBisiesto() ? 29 : 28;
        case 4: case 6: case 9: case 11: return 30;
        default: return 31;
        }
    }

    void validar() const {
        if (año < 1900 || año > 2100) throw std::invalid_argument("Año inválido");
        if (mes < 1 || mes > 12) throw std::invalid_argument("Mes inválido");
        if (dia < 1 || dia > diasEnMes()) throw std::invalid_argument("Día inválido");
    }

public:
    Fecha(int d, int m, int a) : dia(d), mes(m), año(a) {
        validar();
    }

    // Constructor de copia
    Fecha(const Fecha& otra) : dia(otra.dia), mes(otra.mes), año(otra.año) {}

    // Operadores de comparación
    bool operator==(const Fecha& otra) const {
        return dia == otra.dia && mes == otra.mes && año == otra.año;
    }

    bool operator<(const Fecha& otra) const {
        if (año != otra.año) return año < otra.año;
        if (mes != otra.mes) return mes < otra.mes;
        return dia < otra.dia;
    }

    bool operator>(const Fecha& otra) const {
        return otra < *this;
    }

    // Sumar días a la fecha
    Fecha sumarDias(int n) const {
        Fecha nueva = *this;
        while (n > 0) {
            int diasRestantesEnMes = nueva.diasEnMes() - nueva.dia;
            if (n > diasRestantesEnMes) {
                n -= (diasRestantesEnMes + 1);
                nueva.dia = 1;
                if (nueva.mes == 12) {
                    nueva.mes = 1;
                    nueva.año++;
                } else {
                    nueva.mes++;
                }
            } else {
                nueva.dia += n;
                n = 0;
            }
        }
        return nueva;
    }

    // Diferencia en días entre dos fechas
    int diferenciaDias(const Fecha& otra) const {
        Fecha menor = (*this < otra) ? *this : otra;
        Fecha mayor = (*this < otra) ? otra : *this;
        int dias = 0;
        while (menor < mayor) {
            menor = menor.sumarDias(1);
            dias++;
        }
        return dias;
    }

    // Método para mostrar la fecha en formato legible
    void mostrar() const {
        const char* meses[] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
                               "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
        std::cout << dia << " de " << meses[mes-1] << " del " << año;
    }
};



#include "Fecha.h"
#include <cstring>

class Alojamiento {
private:
    char* codigo;
    char* nombre;
    char* departamento;
    char* municipio;
    char* direccion;
    double precioPorNoche;

    struct Rango {
        Fecha inicio;
        Fecha fin;
        char* codigoReserva;
    };

    struct NodoRango {
        Rango dato;
        NodoRango* siguiente;
    };

    NodoRango* rangosReservados;

public:
    Alojamiento(const char* cod, const char* nom, const char* dep,
                const char* mun, const char* dir, double precio)
        : precioPorNoche(precio) {
        codigo = new char[strlen(cod)+1];
        strcpy(codigo, cod);
        nombre = new char[strlen(nom)+1];
        strcpy(nombre, nom);
        departamento = new char[strlen(dep)+1];
        strcpy(departamento, dep);
        municipio = new char[strlen(mun)+1];
        strcpy(municipio, mun);
        direccion = new char[strlen(dir)+1];
        strcpy(direccion, dir);
        rangosReservados = nullptr;
    }

    // Constructor de copia
    Alojamiento(const Alojamiento& otro) {
        // Implementación similar al constructor, copiando todos los strings
        // y la lista de rangos reservados (no implementado por brevedad)
    }

    ~Alojamiento() {
        delete[] codigo;
        delete[] nombre;
        delete[] departamento;
        delete[] municipio;
        delete[] direccion;
        // Liberar lista de rangos reservados (no implementado por brevedad)
    }

    bool disponible(const Fecha& inicio, int noches) const {
        Fecha fin = inicio.sumarDias(noches);
        NodoRango* actual = rangosReservados;
        while (actual != nullptr) {
            if (!(fin < actual->dato.inicio || inicio > actual->dato.fin)) {
                return false; // Hay solapamiento
            }
            actual = actual->siguiente;
        }
        return true;
    }

    void reservar(const Fecha& inicio, int noches, const char* codigoReserva) {
        if (!disponible(inicio, noches)) {
            throw std::runtime_error("Alojamiento no disponible en esas fechas");
        }

        Fecha fin = inicio.sumarDias(noches);
        NodoRango* nuevo = new NodoRango;
        nuevo->dato.inicio = inicio;
        nuevo->dato.fin = fin;
        nuevo->dato.codigoReserva = new char[strlen(codigoReserva)+1];
        strcpy(nuevo->dato.codigoReserva, codigoReserva);

        // Insertar ordenado por fecha de inicio
        NodoRango** actual = &rangosReservados;
        while (*actual != nullptr && (*actual)->dato.inicio < inicio) {
            actual = &((*actual)->siguiente);
        }
        nuevo->siguiente = *actual;
        *actual = nuevo;
    }

    const char* getCodigo() const { return codigo; }
    const char* getNombre() const { return nombre; }
    const char* getMunicipio() const { return municipio; }
    double getPrecioPorNoche() const { return precioPorNoche; }
};
