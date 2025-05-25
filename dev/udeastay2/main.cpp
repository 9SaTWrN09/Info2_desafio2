// Ejemplo en main.cpp
#include "clases.h"
#include <iostream>

int main() {
    Alojamiento aloj("A1", "Casa", "Antioquia", "Medellín", "Calle 1", 100.0);
    Reserva res("R1", Fecha(1, 1, 2025), 3, &aloj, "123", "PSE", Fecha(1, 1, 2025), 300.0, "Notas");
    res.anular(); // Debe eliminar la reserva de "aloj" y cambiar estado.

    return 0;
}
