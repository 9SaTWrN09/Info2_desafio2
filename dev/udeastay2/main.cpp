// Ejemplo en main.cpp
#include "clases.h"
#include <iostream>

int main() {
    // Pruebas de Fecha
    Fecha hoy(29, 2, 2024);
    Fecha manana = hoy.sumarDias(1);
    std::cout << "Hoy: "; hoy.mostrar(); std::cout << std::endl;
    std::cout << "Mañana: "; manana.mostrar(); std::cout << std::endl;

    // Pruebas de Alojamiento
    Alojamiento hotel("HOT123", "Hotel Ejemplo", "Antioquia", "Medellín", "Calle 123", 150.0);
    hotel.reservar(hoy, 3, "RES001");
    std::cout << "Precio por noche: $" << hotel.getPrecioPorNoche() << std::endl;

    return 0;
}
