#ifndef MENU_USUARIO_H
#define MENU_USUARIO_H

#include "clases.h"
#include "file_respository.h"

// ----------------------------------------------
// Enumeración para roles de usuario
// ----------------------------------------------
enum class RolUsuario {
    Huesped,     // Permite reservar y anular reservas
    Anfitrion,    // Permite consultar reservas y gestionar alojamientos
    NoAutenticado // Estado inicial o credenciales inválidas
};

// ----------------------------------------------
// Clase: ControladorAutenticacion
// Gestiona la autenticación de usuarios
// ----------------------------------------------
class ControladorAutenticacion {
private:
    FileRepository<Usuario<GuestPolicy>>& repoHuespedes; // Repositorio de huéspedes
    FileRepository<Usuario<HostPolicy>>& repoAnfitriones; // Repositorio de anfitriones

public:
    // Constructor (recibe repositorios por referencia)
    ControladorAutenticacion(
        FileRepository<Usuario<GuestPolicy>>& repoHuespedes,
        FileRepository<Usuario<HostPolicy>>& repoAnfitriones
        );

    // Autentica un usuario y devuelve su rol
    RolUsuario autenticar(const char* documento, const char* contraseña);

    // Método auxiliar: Busca un usuario en una lista
    template<typename TPolicy>
    bool buscarUsuario(const Lista<Usuario<TPolicy>>& lista, const char* documento, const char* contraseña);
};

// ----------------------------------------------
// Clase: MenuUsuario
// Controla la interacción con el menú según el rol
// ----------------------------------------------
class MenuUsuario {
private:
    Controller& controlador; // Referencia al controlador principal

public:
    // Constructor
    explicit MenuUsuario(Controller& ctrl);

    // Muestra el menú según el rol del usuario
    void mostrarMenu(RolUsuario rol);

private:
    // Menús específicos
    void menuHuesped();
    void menuAnfitrion();

    // Métodos auxiliares
    void limpiarPantalla() const;
    void pausa() const;
};

#endif // MENU_USUARIO_H
