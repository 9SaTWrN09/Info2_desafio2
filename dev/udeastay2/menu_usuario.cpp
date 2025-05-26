#include "menu_usuario.h"
#include <iostream>
#include <cstring>
#include <cstdlib> // Para system() en limpiarPantalla()

// ==============================================
// Implementación de ControladorAutenticacion
// ==============================================

ControladorAutenticacion::ControladorAutenticacion(
    FileRepository<Usuario<GuestPolicy>>& repoHuespedes,
    FileRepository<Usuario<HostPolicy>>& repoAnfitriones
    ) : repoHuespedes(repoHuespedes), repoAnfitriones(repoAnfitriones) {}

template<typename TPolicy>
bool ControladorAutenticacion::buscarUsuario(const Lista<Usuario<TPolicy>>& lista,
                                             const char* documento,
                                             const char* contraseña) {
    typename Lista<Usuario<TPolicy>>::Iterador it = lista.obtenerIterador();
    while (it.haySiguiente()) {
        Usuario<TPolicy>& usuario = it.siguiente();
        if (strcmp(usuario.getDocumento(), documento) == 0 &&
            strcmp(usuario.getContraseña(), contraseña) == 0) {
            return true;
        }
    }
    return false;
}

RolUsuario ControladorAutenticacion::autenticar(const char* documento, const char* contraseña) {
    // Buscar en huéspedes
    Lista<Usuario<GuestPolicy>> huespedes = repoHuespedes.cargar();
    if (buscarUsuario<GuestPolicy>(huespedes, documento, contraseña)) {
        return RolUsuario::Huesped;
    }

    // Buscar en anfitriones
    Lista<Usuario<HostPolicy>> anfitriones = repoAnfitriones.cargar();
    if (buscarUsuario<HostPolicy>(anfitriones, documento, contraseña)) {
        return RolUsuario::Anfitrion;
    }

    return RolUsuario::NoAutenticado;
}

// ==============================================
// Implementación de MenuUsuario
// ==============================================

MenuUsuario::MenuUsuario(Controller& ctrl) : controlador(ctrl) {}

void MenuUsuario::mostrarMenu(RolUsuario rol) {
    limpiarPantalla();
    switch (rol) {
    case RolUsuario::Huesped: menuHuesped(); break;
    case RolUsuario::Anfitrion: menuAnfitrion(); break;
    default: throw std::runtime_error("Rol no válido");
    }
}

// --------------------------
// Menú Huésped
// --------------------------
void MenuUsuario::menuHuesped() {
    int opcion;
    do {
        limpiarPantalla();
        std::cout << "\n=== MENÚ HUÉSPED ==="
                  << "\n1. Buscar y reservar alojamiento"
                  << "\n2. Anular reserva"
                  << "\n3. Ver mis reservas"
                  << "\n4. Salir"
                  << "\nSeleccione: ";

        std::cin >> opcion;
        limpiarPantalla();

        try {
            switch (opcion) {
            case 1: {
                Fecha fecha;
                int noches;
                std::cout << "Fecha de entrada (dd mm aaaa): ";
                std::cin >> fecha.dia >> fecha.mes >> fecha.anio;
                std::cout << "Noches: ";
                std::cin >> noches;
                controlador.buscarYReservar(fecha, noches);
                break;
            }
            case 2: {
                char codigo[16];
                std::cout << "Código de reserva: ";
                std::cin >> codigo;
                controlador.anularReserva(codigo, false);
                break;
            }
            case 3:
                controlador.mostrarReservasUsuario();
                break;
            case 4:
                std::cout << "Saliendo...\n";
                return;
            default:
                std::cout << "Opción inválida!\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
        pausa();
    } while (true);
}

// --------------------------
// Menú Anfitrión
// --------------------------
void MenuUsuario::menuAnfitrion() {
    int opcion;
    do {
        limpiarPantalla();
        std::cout << "\n=== MENÚ ANFITRIÓN ==="
                  << "\n1. Ver reservas activas"
                  << "\n2. Actualizar histórico"
                  << "\n3. Gestionar alojamientos"
                  << "\n4. Salir"
                  << "\nSeleccione: ";

        std::cin >> opcion;
        limpiarPantalla();

        try {
            switch (opcion) {
            case 1: {
                Fecha inicio, fin;
                std::cout << "Fecha inicial (dd mm aaaa): ";
                std::cin >> inicio.dia >> inicio.mes >> inicio.anio;
                std::cout << "Fecha final (dd mm aaaa): ";
                std::cin >> fin.dia >> fin.mes >> fin.anio;
                controlador.consultarReservasHost(inicio, fin);
                break;
            }
            case 2: {
                Fecha corte;
                std::cout << "Fecha de corte (dd mm aaaa): ";
                std::cin >> corte.dia >> corte.mes >> corte.anio;
                controlador.actualizarHistorico(corte);
                break;
            }
            case 3:
                controlador.gestionarAlojamientos();
                break;
            case 4:
                std::cout << "Saliendo...\n";
                return;
            default:
                std::cout << "Opción inválida!\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
        pausa();
    } while (true);
}

// --------------------------
// Métodos auxiliares
// --------------------------
void MenuUsuario::limpiarPantalla() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void MenuUsuario::pausa() const {
    std::cout << "\nPresione Enter para continuar...";
    std::cin.ignore();
    std::cin.get();
}
