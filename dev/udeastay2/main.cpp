#include <iostream>
#include "clases.h"
#include "file_respository.h"
#include <string>

int main() {
    try {
        // 1. Crear y guardar alojamiento
        Alojamiento alo("CASA1", "Cabaña Premium", "Antioquia", "Jardín", "Vereda La Esperanza", 250000.0);
        FileRepository<Alojamiento> repoAlo("alojamientos.dat");
        repoAlo.guardar(alo);

        // 2. Crear y guardar reserva
        Fecha entrada(20, 12, 2024);
        Fecha pago(1, 12, 2024);
        Reserva res("RES2024", entrada, 5, "CASA1", "1000001", "Transferencia", pago, 1250000.0, "Check-in temprano");
        FileRepository<Reserva> repoRes("reservas.dat");
        repoRes.guardar(res);

        // 3. Cargar y verificar datos
        Lista<Reserva> reservas = repoRes.cargar();
        if (reservas.tamano() > 0) {
            Reserva& r = reservas.obtener(0);
            std::cout << "Reserva cargada:\n"
                      << "Alojamiento: " << r.getCodigoAlojamiento() << "\n"
                      << "Fecha Pago: " << r.getFechaPago().getDia() << "/"
                      << r.getFechaPago().getMes() << "/"
                      << r.getFechaPago().getAño() << "\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
