// controller.h
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "clases.h"
#include "file_respository.h"

class Controller {
private:
    FileRepository<Huesped> repoHuespedes;
    FileRepository<Anfitrion> repoAnfitriones;
    FileRepository<Alojamiento> repoAlojamientos;
    FileRepository<Reserva> repoReservas;

public:
    Controller(const FileRepository<Huesped>& repoH,
               const FileRepository<Anfitrion>& repoA,
               const FileRepository<Alojamiento>& repoAlo,
               const FileRepository<Reserva>& repoRes)
        : repoHuespedes(repoH),
        repoAnfitriones(repoA),
        repoAlojamientos(repoAlo),
        repoReservas(repoRes) {}

    void buscarYReservar(const Fecha& fecha, int noches);
    void anularReserva(const char* codigo, bool esAnfitrion);
    void mostrarReservasUsuario();
    void consultarReservasHost(const Fecha& inicio, const Fecha& fin);
    void actualizarHistorico(const Fecha& fechaCorte);
    void gestionarAlojamientos();
};

#endif // CONTROLLER_H
