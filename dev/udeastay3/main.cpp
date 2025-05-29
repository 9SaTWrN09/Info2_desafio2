// main.cpp (versión simplificada)
#include <cassert>
#include <cstdio>
#include <iostream>
#include <cctype>
#include <string>
#include <limits>
#include "clases.h"
#include "file_repository.h"

FileRepository<Reserva> repoActivas("reservas_activas_test.dat");
FileRepository<Reserva> repoHistoricas("reservas_historicas_test.dat");

void probarPersistencia() {
    // ... (tu implementación existente) ...
}

int main() {
    std::string input;
    bool salir = false;

    while (!salir) {
        std::cout << "\n=== MENU SIMPLIFICADO ===";
        std::cout << "\n1. Probar persistencia";
        std::cout << "\n2. Salir";
        std::cout << "\nSeleccione una opción: ";

        std::getline(std::cin, input);
        if (input.empty()) continue;

        char opcion = input[0];

        if (opcion == '1') {
            probarPersistencia();
            std::cout << "Prueba completada correctamente!\n";
        }
        else if (opcion == '2') {
            salir = true;
            std::cout << "Saliendo del sistema...\n";
        }
        else {
            std::cout << "Opción inválida.\n";
        }

        if (!salir) {
            std::cin.clear();
            std::cout << "Presione Enter para continuar...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return 0;
}
