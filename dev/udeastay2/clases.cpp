#include "clases.h"
#include <iostream>
#include <stdexcept>
#include <cstring>

// ==================== Implementación de Fecha ====================
bool Fecha::esBisiesto() const {
    return (año % 4 == 0 && año % 100 != 0) || (año % 400 == 0);
}

int Fecha::diasEnMes() const {
    switch (mes) {
    case 2:  return esBisiesto() ? 29 : 28;
    case 4: case 6: case 9: case 11: return 30;
    default: return 31;
    }
}

void Fecha::validar() const {
    if (año < 1900 || año > 2100) throw std::invalid_argument("Año inválido");
    if (mes < 1 || mes > 12) throw std::invalid_argument("Mes inválido");
    if (dia < 1 || dia > diasEnMes()) throw std::invalid_argument("Día inválido");
}

Fecha::Fecha(int d, int m, int a) : dia(d), mes(m), año(a) {
    validar();
}

bool Fecha::operator==(const Fecha& otra) const {
    return dia == otra.dia && mes == otra.mes && año == otra.año;
}

bool Fecha::operator<(const Fecha& otra) const {
    if (año != otra.año) return año < otra.año;
    if (mes != otra.mes) return mes < otra.mes;
    return dia < otra.dia;
}

bool Fecha::operator>(const Fecha& otra) const {
    return otra < *this;
}

Fecha Fecha::sumarDias(int n) const {
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
    nueva.validar();
    return nueva;
}

int Fecha::diferenciaDias(const Fecha& otra) const {
    Fecha menor = (*this < otra) ? *this : otra;
    Fecha mayor = (*this < otra) ? otra : *this;
    int dias = 0;
    while (menor < mayor) {
        menor = menor.sumarDias(1);
        dias++;
    }
    return dias;
}

void Fecha::mostrar() const {
    const char* meses[] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
                           "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
    std::cout << dia << " de " << meses[mes-1] << " del " << año;
}

// ==================== Implementación de Alojamiento ====================
Alojamiento::Alojamiento(const char* cod, const char* nom, const char* dep,
                         const char* mun, const char* dir, double precio) : precioPorNoche(precio) {
    codigo = new char[strlen(cod) + 1];
    strcpy(codigo, cod);
    nombre = new char[strlen(nom) + 1];
    strcpy(nombre, nom);
    departamento = new char[strlen(dep) + 1];
    strcpy(departamento, dep);
    municipio = new char[strlen(mun) + 1];
    strcpy(municipio, mun);
    direccion = new char[strlen(dir) + 1];
    strcpy(direccion, dir);
    rangosReservados = nullptr;
}

Alojamiento::Alojamiento(const Alojamiento& otro) : precioPorNoche(otro.precioPorNoche) {
    // Copia profunda de cadenas
    codigo = new char[strlen(otro.codigo) + 1];
    strcpy(codigo, otro.codigo);
    nombre = new char[strlen(otro.nombre) + 1];
    strcpy(nombre, otro.nombre);
    departamento = new char[strlen(otro.departamento) + 1];
    strcpy(departamento, otro.departamento);
    municipio = new char[strlen(otro.municipio) + 1];
    strcpy(municipio, otro.municipio);
    direccion = new char[strlen(otro.direccion) + 1];
    strcpy(direccion, otro.direccion);

    // Copia profunda de la lista de reservas
    rangosReservados = nullptr;
    NodoRango* actualOtro = otro.rangosReservados;
    NodoRango** actualThis = &rangosReservados;
    while (actualOtro != nullptr) {
        *actualThis = new NodoRango(actualOtro->dato);
        actualThis = &((*actualThis)->siguiente);
        actualOtro = actualOtro->siguiente;
    }
}

Alojamiento::~Alojamiento() {
    delete[] codigo;
    delete[] nombre;
    delete[] departamento;
    delete[] municipio;
    delete[] direccion;

    // Liberar lista de reservas
    NodoRango* actual = rangosReservados;
    while (actual != nullptr) {
        NodoRango* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }
}

bool Alojamiento::disponible(const Fecha& inicio, int noches) const {
    Fecha fin = inicio.sumarDias(noches);
    NodoRango* actual = rangosReservados;
    while (actual != nullptr) {
        if (!(fin < actual->dato.inicio || inicio > actual->dato.fin)) {
            return false;
        }
        actual = actual->siguiente;
    }
    return true;
}

void Alojamiento::reservar(const Fecha& inicio, int noches, const char* codigoReserva) {
    if (!disponible(inicio, noches)) {
        throw std::runtime_error("Alojamiento no disponible en esas fechas");
    }

    Fecha fin = inicio.sumarDias(noches);
    Rango nuevoRango(inicio, fin, codigoReserva);
    NodoRango* nuevoNodo = new NodoRango(nuevoRango);

    // Insertar en orden
    NodoRango** actual = &rangosReservados;
    while (*actual != nullptr && (*actual)->dato.inicio < inicio) {
        actual = &((*actual)->siguiente);
    }
    nuevoNodo->siguiente = *actual;
    *actual = nuevoNodo;
}

const char* Alojamiento::getCodigo() const { return codigo; }
const char* Alojamiento::getNombre() const { return nombre; }
const char* Alojamiento::getMunicipio() const { return municipio; }
double Alojamiento::getPrecioPorNoche() const { return precioPorNoche; }
