#include <cassert>
#include "clases.h"
#include "file_repository.h"
#include <cstdio>// Para remove()
#include <iostream>

void testPersistencia() {
    // Crear fechas
    Fecha fechaPasada(10, 5, 2025);
    Fecha fechaFutura(20, 6, 2025);
    Fecha pago1(15, 6, 2025);
    Fecha pago2(5, 5, 2025);

    // Crear reservas con todos los parámetros
    Reserva activa("RES-100", fechaFutura, 5, "A001", "HUE-001",
                   "Tarjeta", pago1, 500.0, "Comentario activa");

    Reserva historica("RES-101", fechaPasada, 3, "A002", "HUE-002",
                      "Efectivo", pago2, 300.0, "Comentario historica");

    // Configurar repositorios
    FileRepository<Reserva> repo("reservas_test.dat");
    FileRepository<Reserva> repoHist("reservas_historicas_test.dat");

    // Limpiar archivos previos (opcional)
    remove("reservas_test.dat");
    remove("reservas_historicas_test.dat");

    // Guardar reservas
    repo.guardar(activa);
    repo.guardar(historica);

    // Mover una reserva a histórico
    repo.moverAHistorico(historica);

    // Verificar resultados
    Lista<Reserva> activas = repo.cargar();
    Lista<Reserva> historicas = repoHist.cargar();

    assert(activas.tamano() == 1);
    assert(historicas.tamano() == 1);
    assert(historicas.obtener(0).getEstado() == EstadoReserva::Historica);
    assert(strcmp(activas.obtener(0).getCodigo(), "RES-100") == 0);

    // Limpieza final
    remove("reservas_test.dat");
    remove("reservas_historicas_test.dat");

    std::cout << "¡Test de persistencia pasado con éxito!" << std::endl;
}

int main() {
    testPersistencia();
    return 0;
}
