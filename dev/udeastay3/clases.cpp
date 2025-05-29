#include "clases.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <cstdlib>

// ==================== Implementación de Fecha ====================
bool Fecha::esBisiesto() const {
    return (año % 4 == 0 && año % 100 != 0) || (año % 400 == 0);
}

int Fecha::diasEnMes() const {
    switch (mes) {
    case 2: return esBisiesto() ? 29 : 28;
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
    while (n-- > 0) {
        if (++nueva.dia > nueva.diasEnMes()) {
            nueva.dia = 1;
            if (++nueva.mes > 12) {
                nueva.mes = 1;
                nueva.año++;
            }
        }
    }
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

char* Fecha::toCSV() const {
    char* buffer = new char[11];
    snprintf(buffer, 11, "%02d/%02d/%04d", dia, mes, año);
    return buffer;
}

Fecha Fecha::fromCSV(const char* linea) {
    int d, m, a;
    sscanf(linea, "%d/%d/%d", &d, &m, &a);
    return Fecha(d, m, a);
}

// ==================== Implementación de Alojamiento ====================
Alojamiento::Rango::Rango(const Fecha& i, const Fecha& f, const char* cod) : inicio(i), fin(f) {
    codigoReserva = new char[strlen(cod) + 1];
    strcpy(codigoReserva, cod);
}
// Constructor de copia
Alojamiento::Alojamiento(const Alojamiento& otro) {
    // Copiar strings con memoria dinámica
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

    precioPorNoche = otro.precioPorNoche;

    // Copiar la lista de rangos reservados
    rangosReservados = nullptr;
    if (otro.rangosReservados != nullptr) {
        // Copiar nodo cabeza
        rangosReservados = new NodoRango(otro.rangosReservados->dato);
        NodoRango* actualNuevo = rangosReservados;
        NodoRango* actualOtro = otro.rangosReservados->siguiente;

        // Copiar el resto de nodos
        while (actualOtro != nullptr) {
            actualNuevo->siguiente = new NodoRango(actualOtro->dato);
            actualNuevo = actualNuevo->siguiente;
            actualOtro = actualOtro->siguiente;
        }
    }
}


Alojamiento::Rango::~Rango() {
    delete[] codigoReserva;
}

Alojamiento::NodoRango::NodoRango(const Rango& r, NodoRango* sig) : dato(r.inicio, r.fin, r.codigoReserva), siguiente(sig) {}

Alojamiento::Alojamiento(const char* cod, const char* nom, const char* dep,
                         const char* mun, const char* dir, double precio) {
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

    precioPorNoche = precio;
    rangosReservados = nullptr;
}

Alojamiento::~Alojamiento() {
    delete[] codigo;
    delete[] nombre;
    delete[] departamento;
    delete[] municipio;
    delete[] direccion;

    NodoRango* actual = rangosReservados;
    while (actual != nullptr) {
        NodoRango* temp = actual;
        actual = actual->siguiente;
        delete temp;
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
        throw std::runtime_error("Fechas no disponibles");
    }
    NodoRango* nuevo = new NodoRango(Rango(inicio, inicio.sumarDias(noches), codigoReserva));
    nuevo->siguiente = rangosReservados;
    rangosReservados = nuevo;
}

char* Alojamiento::toCSV() const {
    int tamano = strlen(codigo) + strlen(nombre) + strlen(departamento) +
                 strlen(municipio) + strlen(direccion) + 50;
    char* buffer = new char[tamano];
    snprintf(buffer, tamano, "%s,%s,%s,%s,%s,%.2f",
             codigo, nombre, departamento, municipio, direccion, precioPorNoche);
    return buffer;
}

Alojamiento Alojamiento::fromCSV(const char* linea) {
    char* lineaCopia = new char[strlen(linea) + 1];
    strcpy(lineaCopia, linea);

    char* cod = strtok(lineaCopia, ",");
    char* nom = strtok(nullptr, ",");
    char* dep = strtok(nullptr, ",");
    char* mun = strtok(nullptr, ",");
    char* dir = strtok(nullptr, ",");
    char* precioStr = strtok(nullptr, ",");

    double precio = atof(precioStr);

    Alojamiento resultado(cod, nom, dep, mun, dir, precio);
    delete[] lineaCopia;
    return resultado;
}



// ==================== Implementación de Reserva ====================
Reserva::Reserva(const char* cod, const Fecha& entrada, int duracion,
                 const char* codAlojamiento, const char* docHuesped,
                 const char* metodo, const Fecha& fPago, double monto,
                 const char* anot) {
    codigo = new char[strlen(cod) + 1];
    strcpy(codigo, cod);

    codigoAlojamiento = new char[strlen(codAlojamiento) + 1];
    strcpy(codigoAlojamiento, codAlojamiento);

    documentoHuesped = new char[strlen(docHuesped) + 1];
    strcpy(documentoHuesped, docHuesped);

    metodoPago = new char[strlen(metodo) + 1];
    strcpy(metodoPago, metodo);

    anotaciones = new char[(anot ? strlen(anot) : 0) + 1];
    strcpy(anotaciones, anot ? anot : "");

    fechaEntrada = entrada;
    duracionNoches = duracion;
    fechaPago = fPago;
    this->monto = monto;
    estado = EstadoReserva::Activa;
}

Reserva::Reserva(const Reserva& otra) {
    codigo = new char[strlen(otra.codigo) + 1];
    strcpy(codigo, otra.codigo);

    codigoAlojamiento = new char[strlen(otra.codigoAlojamiento) + 1];
    strcpy(codigoAlojamiento, otra.codigoAlojamiento);

    documentoHuesped = new char[strlen(otra.documentoHuesped) + 1];
    strcpy(documentoHuesped, otra.documentoHuesped);

    metodoPago = new char[strlen(otra.metodoPago) + 1];
    strcpy(metodoPago, otra.metodoPago);

    anotaciones = new char[strlen(otra.anotaciones) + 1];
    strcpy(anotaciones, otra.anotaciones);

    fechaEntrada = otra.fechaEntrada;
    duracionNoches = otra.duracionNoches;
    fechaPago = otra.fechaPago;
    monto = otra.monto;
    estado = otra.estado;
}

// clases.cpp (dentro de Reserva)
Reserva& Reserva::operator=(const Reserva& otra) {
    if (this != &otra) {
        // Liberar memoria existente
        delete[] codigo;
        delete[] codigoAlojamiento;
        delete[] documentoHuesped;
        delete[] metodoPago;
        delete[] anotaciones;

        // Copiar cadenas
        codigo = copiarCadena(otra.codigo);
        codigoAlojamiento = copiarCadena(otra.codigoAlojamiento);
        documentoHuesped = copiarCadena(otra.documentoHuesped);
        metodoPago = copiarCadena(otra.metodoPago);
        anotaciones = copiarCadena(otra.anotaciones);

        // Copiar otros campos
        fechaEntrada = otra.fechaEntrada;
        duracionNoches = otra.duracionNoches;
        fechaPago = otra.fechaPago;
        monto = otra.monto;
        estado = otra.estado;
    }
    return *this;
}

// Función auxiliar para copiar cadenas
char* Reserva::copiarCadena(const char* original) {
    if (!original) return nullptr;
    char* copia = new char[strlen(original) + 1];
    strcpy(copia, original);
    return copia;
}



Reserva::~Reserva() {
    delete[] codigo;
    delete[] codigoAlojamiento;
    delete[] documentoHuesped;
    delete[] metodoPago;
    delete[] anotaciones;
}

void Reserva::anular() {
    estado = EstadoReserva::Anulada;
}

// clases.cpp (Reserva::toCSV)
char* Reserva::toCSV() const {
    // Usar formato consistente para fechas
    char* entradaCSV = fechaEntrada.toCSV();
    char* pagoCSV = fechaPago.toCSV();

    char buffer[2000];
    snprintf(buffer, sizeof(buffer), "%s,%s,%d,%s,%s,%d,%s,%s,%.2f,%s",
             codigo,
             entradaCSV,          // Fecha entrada formateada
             duracionNoches,
             codigoAlojamiento,
             documentoHuesped,
             static_cast<int>(estado),  // Guardar estado como entero
             metodoPago,
             pagoCSV,              // Fecha pago formateada
             monto,
             anotaciones);

    delete[] entradaCSV;
    delete[] pagoCSV;

    char* resultado = new char[strlen(buffer) + 1];
    strcpy(resultado, buffer);
    return resultado;
}

// clases.cpp (Reserva::fromCSV)
Reserva Reserva::fromCSV(const char* linea) {
    char cod[50], fechaEntradaStr[20], codAlojamiento[50], docHuesped[50];
    char metodo[50], fechaPagoStr[20], anot[1001];
    int duracion, estadoInt;
    double monto;

    // Usar sscanf para mayor control
    sscanf(linea, "%[^,],%[^,],%d,%[^,],%[^,],%d,%[^,],%[^,],%lf,%[^\n]",
           cod, fechaEntradaStr, &duracion, codAlojamiento, docHuesped,
           &estadoInt, metodo, fechaPagoStr, &monto, anot);

    Fecha entrada = Fecha::fromCSV(fechaEntradaStr);
    Fecha pago = Fecha::fromCSV(fechaPagoStr);

    Reserva res(cod, entrada, duracion, codAlojamiento, docHuesped,
                metodo, pago, monto, anot);

    // Establecer estado correctamente
    res.setEstado(static_cast<EstadoReserva>(estadoInt));
    return res;
}




