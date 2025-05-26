#include <iostream>
#include "clases.h"
#include "file_respository.h"

int main() {
    try {
        // 1. Crear un huésped de prueba
        Usuario<GuestPolicy> huesped(
            "1000001",   // Documento
            "clave123",  // Contraseña
            6,           // Antigüedad (meses)
            4.5          // Puntuación
            );

        FileRepository<Usuario<GuestPolicy>> repoHuespedes("huespedes.dat");
        repoHuespedes.guardar(huesped);

        // 2. Crear un anfitrión de prueba
        Usuario<HostPolicy> anfitrion(
            "2000001",   // Documento
            "clave456",  // Contraseña
            12,          // Antigüedad (meses)
            4.8          // Puntuación
            );

        FileRepository<Usuario<HostPolicy>> repoAnfitriones("anfitriones.dat");
        repoAnfitriones.guardar(anfitrion);

        std::cout << "Usuarios de prueba creados exitosamente.\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
