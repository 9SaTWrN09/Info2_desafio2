#include <iostream>
#include "clases.h"

int main() {
    Huesped huesped("123456", "claveSegura", 12, 4.8);
    huesped.agregarItem("RES-001");
    huesped.agregarItem("RES-002");

    // Serialización
    char* csv = huesped.toCSV();
    std::cout << "CSV: " << csv << "\n";
    delete[] csv;

    // Limpieza
    huesped.eliminarItem("RES-001");
    huesped.eliminarItem("RES-002");
    std::cout << "Reservas restantes: " << huesped.getColeccion().tamano() << "\n"; // Saldría 0
    std::cout<<"Todo correcto";
    return 0;
}
