#include <cassert>
#include "clases.h"
#include <iostream>
#include <cstring>

// Función para imprimir fechas
void imprimirFecha(const Fecha& fecha) {
    std::cout << fecha.getDia() << "/" << fecha.getMes() << "/" << fecha.getAño();
}

int main() {
    std::cout << "=== Simulación de Sistema de Reservas ===" << std::endl;

    // Crear alojamiento
    Alojamiento alojamiento("A001", "Casa en El Poblado", "Antioquia", "Medellín", "Calle 10 #40-20", 200.0);
    std::cout << "Alojamiento creado: " << alojamiento.getNombre() << std::endl;

    // Fechas para reservas
    Fecha fechaInicio1(15, 5, 2025);
    Fecha fechaInicio2(17, 5, 2025);
    int duracion = 4; // 4 noches

    // Huésped 1 intenta reservar
    std::cout << "\n=== Huésped 1 intenta reservar ===" << std::endl;
    std::cout << "Fecha inicio: ";
    imprimirFecha(fechaInicio1);
    std::cout << "\nDuración: " << duracion << " noches" << std::endl;

    if (alojamiento.disponible(fechaInicio1, duracion)) {
        std::cout << "Disponible! Creando reserva..." << std::endl;
        alojamiento.reservar(fechaInicio1, duracion, "RES-001");
        std::cout << "Reserva exitosa para Huésped 1" << std::endl;
    } else {
        std::cout << "No disponible para Huésped 1" << std::endl;
    }

    // Huésped 2 intenta reservar en fechas solapadas
    std::cout << "\n=== Huésped 2 intenta reservar ===" << std::endl;
    std::cout << "Fecha inicio: ";
    imprimirFecha(fechaInicio2);
    std::cout << "\nDuración: " << duracion << " noches" << std::endl;

    try {
        if (alojamiento.disponible(fechaInicio2, duracion)) {
            std::cout << "Disponible! Creando reserva..." << std::endl;
            alojamiento.reservar(fechaInicio2, duracion, "RES-002");
            std::cout << "Reserva exitosa para Huésped 2" << std::endl;
        } else {
            std::cout << "No disponible para Huésped 2" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Error en reserva: " << e.what() << std::endl;
    }

    // Verificar disponibilidad después de las reservas
    std::cout << "\n=== Verificación final ===" << std::endl;
    std::cout << "Disponible del 15-19/5? "
              << (alojamiento.disponible(fechaInicio1, duracion) ? "Sí" : "No") << std::endl;
    std::cout << "Disponible del 17-21/5? "
              << (alojamiento.disponible(fechaInicio2, duracion) ? "Sí" : "No") << std::endl;

    return 0;
}
