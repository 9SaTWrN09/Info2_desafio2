#include <iostream>
#include "clases.h"

int main() {
    try {
        // Crear un alojamiento
        Alojamiento alojamiento("A001", "Hotel Sol", "Antioquia", "Medellín", "Calle 123", 150.0);

        // Fechas para las reservas
        Fecha fecha1(10, 6, 2025);
        Fecha fecha2(15, 6, 2025);
        Fecha fecha3(12, 6, 2025);  // Se solapa con la primera reserva
        Fecha fecha4(20, 6, 2025);  // No se solapa

        // Realizar primera reserva (5 noches desde 10/06/2025)
        alojamiento.reservar(fecha1, 5, "R001");
        std::cout << "Reserva R001 creada con éxito.\n";

        // Comprobar disponibilidad para fecha2 (5 noches desde 15/06/2025) - debería estar disponible
        if (alojamiento.disponible(fecha2, 5)) {
            alojamiento.reservar(fecha2, 5, "R002");
            std::cout << "Reserva R002 creada con éxito.\n";
        } else {
            std::cout << "Reserva R002 no disponible.\n";
        }

        // Intentar reserva que se solapa con la primera (desde 12/06/2025)
        if (alojamiento.disponible(fecha3, 3)) {
            alojamiento.reservar(fecha3, 3, "R003");
            std::cout << "Reserva R003 creada con éxito.\n";
        } else {
            std::cout << "Reserva R003 no disponible (solapamiento detectado).\n";
        }

        // Intentar reserva no solapada (desde 20/06/2025)
        if (alojamiento.disponible(fecha4, 4)) {
            alojamiento.reservar(fecha4, 4, "R004");
            std::cout << "Reserva R004 creada con éxito.\n";
        } else {
            std::cout << "Reserva R004 no disponible.\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}
