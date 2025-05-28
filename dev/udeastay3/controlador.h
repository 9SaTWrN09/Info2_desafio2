// controlador.h
#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include "clases.h"
#include "file_repository.h"

class Controlador {
private:
    FileRepository<Alojamiento> repoAlojamientos;
    FileRepository<Huesped> repoHuespedes;
    FileRepository<Anfitrion> repoAnfitriones;
    FileRepository<Reserva> repoReservasActivas;
    FileRepository<Reserva> repoReservasHistoricas;

    Lista<Alojamiento> alojamientos;
    Lista<Huesped> huespedes;
    Lista<Anfitrion> anfitriones;
    Lista<Reserva> reservasActivas;

public:
    Controlador() :
        repoAlojamientos("data/alojamientos.dat"),
        repoHuespedes("data/huespedes.dat"),
        repoAnfitriones("data/anfitriones.dat"),
        repoReservasActivas("data/reservas_activas.dat"),
        repoReservasHistoricas("data/reservas_historicas.dat") {}

    void cargarDatos() {
        alojamientos = repoAlojamientos.cargar();
        huespedes = repoHuespedes.cargar();
        anfitriones = repoAnfitriones.cargar();
        reservasActivas = repoReservasActivas.cargar();

        // Validar consistencia
        validarReservas();
    }

    void guardarDatos() {
        repoAlojamientos.guardarTodos(alojamientos);
        repoHuespedes.guardarTodos(huespedes);
        repoAnfitriones.guardarTodos(anfitriones);
        repoReservasActivas.guardarTodos(reservasActivas);
    }

    void validarReservas() {
        Lista<Reserva> validas;
        auto it = reservasActivas.obtenerIterador();

        while (it.haySiguiente()) {
            Reserva& reserva = it.siguiente();

            // Verificar existencia de alojamiento
            bool alojamientoExiste = false;
            auto itAloj = alojamientos.obtenerIterador();
            while (itAloj.haySiguiente()) {
                if (strcmp(itAloj.siguiente().getCodigo(), reserva.getCodigoAlojamiento()) == 0) {
                    alojamientoExiste = true;
                    break;
                }
            }

            // Verificar existencia de huésped
            bool huespedExiste = false;
            auto itHues = huespedes.obtenerIterador();
            while (itHues.haySiguiente()) {
                if (strcmp(itHues.siguiente().getDocumento(), reserva.getDocumentoHuesped()) == 0) {
                    huespedExiste = true;
                    break;
                }
            }

            if (alojamientoExiste && huespedExiste) {
                validas.insertar(reserva);
            }
        }

        reservasActivas = validas;
    }

    void actualizarHistoricos(const Fecha& fechaCorte) {
        Lista<Reserva> nuevasActivas;
        auto it = reservasActivas.obtenerIterador();

        while (it.haySiguiente()) {
            Reserva& reserva = it.siguiente();
            Fecha finReserva = reserva.getFechaFin();

            if (finReserva < fechaCorte) {
                reserva.setEstado(EstadoReserva::Historica);
                repoReservasHistoricas.guardar(reserva);
            } else {
                nuevasActivas.insertar(reserva);
            }
        }

        reservasActivas = nuevasActivas;
        repoReservasActivas.guardarTodos(reservasActivas);
    }

    // ... otras funciones del sistema ...
};

#endif // CONTROLADOR_H
