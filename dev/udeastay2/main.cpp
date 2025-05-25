#include "clases.h"
#include <cassert>
#include <iostream>

int main() {
    try {
        // ========== Configuración inicial ==========
        // Crear un alojamiento en Medellín
        Alojamiento casa("CASA_123", "Casa Campo", "Antioquia", "Medellín", "Calle 10 #45-22", 150.0);

        // ========== Prueba 1: Creación de reserva válida ==========
        Reserva reserva(
            "RES_001",
            Fecha(15, 7, 2025),  // Fecha de entrada: 15/07/2025
            5,                   // 5 noches de estadía
            &casa,               // Alojamiento asociado
            "1001234567",        // Documento del huésped
            "TarjetaCrédito",    // Método de pago
            Fecha(10, 7, 2025),  // Fecha de pago: 10/07/2025
            750.0,               // Monto total (5 noches * $150)
            "Necesito check-in temprano."
            );

        // Verificar datos básicos
        assert(strcmp(reserva.getCodigo(), "RES_001") == 0);
        assert(reserva.getDuracionNoches() == 5);
        assert(reserva.getMonto() == 750.0);
        assert(reserva.estaActiva()); // Estado inicial: Activa

        // Verificar que el alojamiento tiene la reserva registrada
        assert(casa.disponible(Fecha(15, 7, 2025), 5) == false); // Debe estar ocupado

        // ========== Prueba 2: Anulación de reserva ==========
        reserva.anular();

        // Verificar estado actualizado
        assert(reserva.getEstado() == EstadoReserva::Anulada);
        assert(!reserva.estaActiva());

        // Verificar que el alojamiento eliminó la reserva
        assert(casa.disponible(Fecha(15, 7, 2025), 5) == true); // Ahora está disponible

        // ========== Prueba 3: Intento de anular reserva ya anulada ==========
        reserva.anular(); // No debería hacer nada
        assert(reserva.getEstado() == EstadoReserva::Anulada); // Estado se mantiene

        // ========== Prueba 4: Error al crear reserva con código vacío ==========
        try {
            Reserva reservaInvalida(
                "",                   // Código vacío (inválido)
                Fecha(20, 7, 2025),
                3,
                &casa,
                "1007654321",
                "PSE",
                Fecha(18, 7, 2025),
                450.0,
                ""
                );
            assert(false); // Esta línea no debe ejecutarse (debe lanzar excepción)
        } catch (const std::invalid_argument& e) {
            assert(strcmp(e.what(), "Código de reserva inválido") == 0);
        }

        // ========== Prueba 5: Error al crear reserva en fechas ocupadas ==========
        Reserva reserva2("RES_002", Fecha(15, 7, 2025), 3, &casa, "2009876543", "Efectivo", Fecha(12, 7, 2025), 450.0, "");
        try {
            // Intentar reservar en fechas solapadas
            Reserva reservaConflictiva(
                "RES_003",
                Fecha(16, 7, 2025),  // Solapa con RES_002 (15-18/07)
                2,
                &casa,
                "3001234567",
                "TarjetaCrédito",
                Fecha(14, 7, 2025),
                300.0,
                ""
                );
            assert(false); // No debe llegar aquí
        } catch (const std::runtime_error& e) {
            assert(strcmp(e.what(), "Alojamiento no disponible en esas fechas") == 0);
        }

        std::cout << "¡Todas las pruebas pasaron con éxito!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error inesperado: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
