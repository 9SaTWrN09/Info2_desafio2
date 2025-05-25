#include "clases.h"
#include "file_repository.h"
#include <cassert>

int main() {
    // 1. Crear alojamiento y reserva
    Alojamiento casa("CASA_123", "Casa Campo", "Antioquia", "Medellín", "Calle 10 #45-22", 150.0);
    Reserva reserva("RES_001", Fecha(15, 7, 2025), 5, &casa, "1001234567", "TarjetaCrédito", Fecha(10, 7, 2025), 750.0, "");

    // 2. Verificar monto
    assert(reserva.getMonto() == 750.0);

    // 3. Probar persistencia
    FileRepository<Alojamiento> repoAlojamientos("alojamientos.txt");
    repoAlojamientos.guardar(casa);

    std::cout << "¡Todas las pruebas pasaron! 🎉" << std::endl;
    return 0;
}
