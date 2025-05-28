/*#ifndef ACCIONES_ANFITRION_H
#define ACCIONES_ANFITRION_H

#include "clases.h"

// ========================================
// Mostrar todas las reservas activas de un anfitrión
// en un rango de fechas para todos sus alojamientos
// ========================================
template<typename R>
void mostrarReservasDeAnfitrion(const Usuario<HostPolicy>& anfitrion,
                                const Lista<Alojamiento>& alojamientos,
                                const Lista<R>& reservas,
                                const Fecha& inicio,
                                const Fecha& fin) {
    typename Lista<char*>::ConstIterador itCod = anfitrion.getColeccion().obtenerIterador();
    while (itCod.haySiguiente()) {
        const char* codAloj = itCod.siguiente();

        typename Lista<Alojamiento>::ConstIterador itAloj = alojamientos.obtenerIterador();
        while (itAloj.haySiguiente()) {
            const Alojamiento& a = itAloj.siguiente();
            if (strcmp(a.getCodigo(), codAloj) == 0) {
                typename Lista<R>::ConstIterador itRes = reservas.obtenerIterador();
                while (itRes.haySiguiente()) {
                    const R& r = itRes.siguiente();
                    if (strcmp(r.getCodigoAlojamiento(), codAloj) == 0 &&
                        r.getEstado() == EstadoReserva::Activa) {
                        Fecha finReserva = r.getFechaFin();
                        if (!(fin < r.getFechaEntrada() || inicio > finReserva)) {
                            char* linea = r.toCSV();
                            std::cout << linea << "\n";
                            delete[] linea;
                        }
                    }
                }
            }
        }
    }
}

// ========================================
// Anular una reserva si el alojamiento es del anfitrión
// ========================================
template<typename R>
bool anularReservaDeAnfitrion(Usuario<HostPolicy>& anfitrion,
                              Lista<R>& reservas,
                              Alojamiento& alojamiento,
                              const char* codigoReserva) {
    // Verificar si el alojamiento pertenece al anfitrión
    bool esMio = false;
    typename Lista<char*>::ConstIterador it = anfitrion.getColeccion().obtenerIterador();
    while (it.haySiguiente()) {
        if (strcmp(it.siguiente(), alojamiento.getCodigo()) == 0) {
            esMio = true;
            break;
        }
    }

    if (!esMio) return false;

    // Buscar y anular la reserva
    typename Lista<R>::Iterador itRes = reservas.obtenerIterador();
    while (itRes.haySiguiente()) {
        R& r = itRes.siguiente();
        if (strcmp(r.getCodigo(), codigoReserva) == 0 &&
            strcmp(r.getCodigoAlojamiento(), alojamiento.getCodigo()) == 0) {
            r.anular();
            alojamiento.eliminarReserva(codigoReserva);
            return true;
        }
    }

    return false;
}

#endif // ACCIONES_ANFITRION_H
*/
