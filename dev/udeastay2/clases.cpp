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
    while (n > 0) {
        int diasRestantesEnMes = nueva.diasEnMes() - nueva.dia;
        if (n > diasRestantesEnMes) {
            n -= (diasRestantesEnMes + 1);
            nueva.dia = 1;
            nueva.mes = (nueva.mes == 12) ? 1 : nueva.mes + 1;
            nueva.año += (nueva.mes == 1) ? 1 : 0;
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
    std::cout << dia << " de " << meses[mes - 1] << " del " << año;
}

// ==================== Implementación de Alojamiento ====================
Alojamiento::Rango::Rango(const Fecha& i, const Fecha& f, const char* cod)
    : inicio(i), fin(f) {
    codigoReserva = new char[strlen(cod) + 1];
    strcpy(codigoReserva, cod);
}

Alojamiento::Rango::~Rango() {
    delete[] codigoReserva;
}

Alojamiento::NodoRango::NodoRango(const Rango& r, NodoRango* sig)
    : dato(r.inicio, r.fin, r.codigoReserva), siguiente(sig) {}

Alojamiento::Alojamiento(const char* cod, const char* nom, const char* dep,
                         const char* mun, const char* dir, double precio)
    : precioPorNoche(precio) {
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

    rangosReservados = nullptr;
    NodoRango* actualOtro = otro.rangosReservados;
    NodoRango** actualThis = &rangosReservados;
    while (actualOtro != nullptr) {
        *actualThis = new NodoRango(actualOtro->dato);
        actualThis = &(*actualThis)->siguiente;
        actualOtro = actualOtro->siguiente;
    }
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
    if (codigoReserva == nullptr || strlen(codigoReserva) == 0) {
        throw std::invalid_argument("Código de reserva inválido");
    }
    if (!disponible(inicio, noches)) {
        throw std::runtime_error("Alojamiento no disponible en esas fechas");
    }

    Fecha fin = inicio.sumarDias(noches);
    NodoRango* nuevoNodo = new NodoRango(Rango(inicio, fin, codigoReserva));
    NodoRango** actual = &rangosReservados;
    while (*actual != nullptr && (*actual)->dato.inicio < inicio) {
        actual = &(*actual)->siguiente;
    }
    nuevoNodo->siguiente = *actual;
    *actual = nuevoNodo;
}

void Alojamiento::eliminarReserva(const char* codigoReserva) {
    NodoRango** actual = &rangosReservados;
    while (*actual != nullptr) {
        if (strcmp((*actual)->dato.codigoReserva, codigoReserva) == 0) {
            NodoRango* temp = *actual;
            *actual = temp->siguiente;
            delete temp;
            return;
        }
        actual = &(*actual)->siguiente;
    }
    throw std::runtime_error("Código de reserva no encontrado");
}

const char* Alojamiento::getCodigo() const { return codigo; }
const char* Alojamiento::getNombre() const { return nombre; }
const char* Alojamiento::getMunicipio() const { return municipio; }
double Alojamiento::getPrecioPorNoche() const { return precioPorNoche; }

// ==================== Implementación de Reserva ====================
Reserva::Reserva(const char* cod, const Fecha& entrada, int duracion,
                 Alojamiento* alo, const char* docHuesped,
                 const char* metodo, const Fecha& fPago, double monto,
                 const char* anot)
    : codigo(nullptr),
    fechaEntrada(entrada),
    duracionNoches(duracion),
    alojamiento(alo),
    documentoHuesped(nullptr),
    estado(EstadoReserva::Activa),
    metodoPago(nullptr),
    fechaPago(fPago),
    monto(monto),
    anotaciones(nullptr) {

    if (alo == nullptr) throw std::invalid_argument("Alojamiento no puede ser nulo");
    if (cod == nullptr || strlen(cod) == 0) throw std::invalid_argument("Código de reserva inválido");

    codigo = new char[strlen(cod) + 1];
    strcpy(codigo, cod);
    documentoHuesped = new char[strlen(docHuesped) + 1];
    strcpy(documentoHuesped, docHuesped);
    metodoPago = new char[strlen(metodo) + 1];
    strcpy(metodoPago, metodo);
    anotaciones = new char[1001];
    strncpy(anotaciones, anot, 1000);
    anotaciones[1000] = '\0';

    try {
        alojamiento->reservar(fechaEntrada, duracionNoches, codigo);
    } catch (...) {
        delete[] codigo;
        delete[] documentoHuesped;
        delete[] metodoPago;
        delete[] anotaciones;
        throw;
    }
}

Reserva::Reserva(const Reserva& otra)
    : codigo(new char[strlen(otra.codigo) + 1]),
    fechaEntrada(otra.fechaEntrada),
    duracionNoches(otra.duracionNoches),
    alojamiento(otra.alojamiento),
    documentoHuesped(new char[strlen(otra.documentoHuesped) + 1]),
    estado(otra.estado),
    metodoPago(new char[strlen(otra.metodoPago) + 1]),
    fechaPago(otra.fechaPago),
    monto(otra.monto),
    anotaciones(new char[1001]) {

    strcpy(codigo, otra.codigo);
    strcpy(documentoHuesped, otra.documentoHuesped);
    strcpy(metodoPago, otra.metodoPago);
    strncpy(anotaciones, otra.anotaciones, 1000);
    anotaciones[1000] = '\0';
}

Reserva::~Reserva() {
    delete[] codigo;
    delete[] documentoHuesped;
    delete[] metodoPago;
    delete[] anotaciones;
}

void Reserva::anular() {
    if (estado != EstadoReserva::Activa) return;
    estado = EstadoReserva::Anulada;
    if (alojamiento != nullptr) {
        alojamiento->eliminarReserva(codigo);
    } else {
        throw std::logic_error("Alojamiento no asociado");
    }
}

bool Reserva::estaActiva() const { return estado == EstadoReserva::Activa; }
const char* Reserva::getCodigo() const { return codigo; }
Alojamiento* Reserva::getAlojamiento() const { return alojamiento; }
const char* Reserva::getDocumentoHuesped() const { return documentoHuesped; }
EstadoReserva Reserva::getEstado() const { return estado; }
Fecha Reserva::getFechaFin() const { return fechaEntrada.sumarDias(duracionNoches); }
int Reserva::getDuracionNoches() const { return duracionNoches; }
double Reserva::getMonto() const {return monto;}


#include <sstream>

// ==================== Alojamiento CSV ====================
std::string Alojamiento::toCSV() const {
    std::ostringstream oss;
    oss << codigo << "," << nombre << "," << departamento << "," << municipio << ","
        << direccion << "," << precioPorNoche;
    return oss.str();
}

Alojamiento Alojamiento::fromCSV(const std::string& linea) {
    std::istringstream iss(linea);
    std::string cod, nom, dep, mun, dir;
    double precio;

    std::getline(iss, cod, ',');
    std::getline(iss, nom, ',');
    std::getline(iss, dep, ',');
    std::getline(iss, mun, ',');
    std::getline(iss, dir, ',');
    iss >> precio;

    return Alojamiento(cod.c_str(), nom.c_str(), dep.c_str(), mun.c_str(), dir.c_str(), precio);
}

// ==================== Reserva CSV ====================
std::string Reserva::toCSV() const {
    std::ostringstream oss;
    oss << codigo << "," << fechaEntrada.sumarDias(0).diferenciaDias(Fecha(1,1,1900)) << ","
        << duracionNoches << "," << alojamiento->getCodigo() << "," << documentoHuesped
        << "," << static_cast<int>(estado) << "," << metodoPago << ","
        << fechaPago.diferenciaDias(Fecha(1,1,1900)) << "," << monto << "," << anotaciones;
    return oss.str();
}

Reserva Reserva::fromCSV(const std::string& linea) {
    std::istringstream iss(linea);
    std::string cod, doc, metodo, anot;
    int diasDesdeBaseEntrada, diasDesdeBasePago, duracion, estadoInt;
    double monto;
    std::string codigoAloj;

    std::getline(iss, cod, ',');
    iss >> diasDesdeBaseEntrada;
    iss.ignore(1); // coma
    iss >> duracion;
    iss.ignore(1);
    std::getline(iss, codigoAloj, ',');
    std::getline(iss, doc, ',');
    iss >> estadoInt;
    iss.ignore(1);
    std::getline(iss, metodo, ',');
    iss >> diasDesdeBasePago;
    iss.ignore(1);
    iss >> monto;
    iss.ignore(1);
    std::getline(iss, anot);

    Fecha base(1,1,1900);
    Fecha entrada = base.sumarDias(diasDesdeBaseEntrada);
    Fecha pago = base.sumarDias(diasDesdeBasePago);

    // ¡OJO! Necesitas pasar el alojamiento correspondiente real, se pone nullptr temporalmente
    return Reserva(cod.c_str(), entrada, duracion, nullptr,
                   doc.c_str(), metodo.c_str(), pago, monto, anot.c_str());
}


