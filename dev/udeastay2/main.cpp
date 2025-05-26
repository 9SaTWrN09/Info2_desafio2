
#include <iostream>       // Para std::cout, std::cerr
#include <stdexcept>      // Para manejo de excepciones (std::exception)
#include "clases.h"       // Para las clases Huesped, Anfitrion y Lista
#include "listas.h"
void probarUsuarioBasico() {
    try {
        // --- Crear huésped con reservas ---
        Huesped huesped("1001", "claveH", 3, 4.5);
        huesped.agregarItem("RES1");
        huesped.agregarItem("RES2");

        // Verificar colección
        const Lista<char*>& reservas = huesped.getColeccion();
        if (reservas.tamano() != 2) {
            std::cerr << "Error: Colección no tiene 2 elementos\n";
        }

        // --- Crear anfitrión con alojamientos ---
        Anfitrion anfitrion("2001", "claveA", 12, 4.9);
        anfitrion.agregarItem("ALO1");
        anfitrion.agregarItem("ALO2");

        // Verificar colección
        const Lista<char*>& alojamientos = anfitrion.getColeccion();
        if (alojamientos.tamano() != 2) {
            std::cerr << "Error: Colección no tiene 2 elementos\n";
        }

        std::cout << "Prueba Usuario Básico: Éxito\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
