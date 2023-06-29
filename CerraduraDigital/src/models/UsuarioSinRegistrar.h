/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Fundamentos de Ingeniería del Software
 *
 * @author Juan Aday Siverio Glez (alu0101503950@ull.edu.es)
 * @author Samuel Lorenzo Sánchez (alu0101210681@ull.edu.es)
 * @author Esther Medina Quintero (alu0101434780@ull.edu.es)
 * @author Paula María Darias Sosa (alu0101398594@ull.edu.es)
 * @author Eduardo González Gutiérrez (alu0101461588@ull.edu.es)
 * @author Pablo García Pérez (alu0101496139@ull.edu.es)
 * 
 * @brief Archivo: UsuarioSinRegistrar.h
 *        Implementación de la clase "UsuarioSinRegistrar"
 *        ----> Constructores
 *        ----> Registrarse
 *        ----> Iniciar sesión
 */

#ifndef USUARIOSINREGISTRAR_H
#define USUARIOSINREGISTRAR_H

#include <iostream>
#include <sstream>
#include <vector>

#include "../middleware/middleware.h"
#include "Cliente.h"
#include "Propietario.h"
#include "Administrador.h"
#include "Utility.h"

template <class InfoType>
class UsuarioRegistrado;

/// Clase para la gestión de usuarios de las cerraduras
template <class InfoType>
class UsuarioSinRegistrar {
 public:
  /// Constructor por defecto
  UsuarioSinRegistrar() {};
  /// Constructor con el nombre de la base de datos
  UsuarioSinRegistrar(std::string);
  UsuarioSinRegistrar(Middleware<InfoType>* BD);
  /// Función para registrarse
  UsuarioRegistrado<InfoType>* Registrarse();
  /// Función para iniciar sesión
  UsuarioRegistrado<InfoType>* IniciarSesion();

 protected:
  Middleware<InfoType>* middleware;
};

/**
 * @brief Constructor para almacenar el nombre del archivo a ser usado por Middleware
 * @tparam InfoType
 * @param[in] std::string 
 */
template <class InfoType>
UsuarioSinRegistrar<InfoType>::UsuarioSinRegistrar(std::string file_name) {
  middleware.file_name_ = file_name;
}

/**
 * @brief Constructor a partir de un Middleware
 * @tparam InfoType
 * @param[out] BD Base de datos 
 */
template <class InfoType>
UsuarioSinRegistrar<InfoType>::UsuarioSinRegistrar(Middleware<InfoType>* BD) {
  middleware = BD;
}

/**
 * @brief Función para registrar un usuario
 * @tparam InfoType
 * @return UsuarioRegistrado<InfoType>*
 */
template <class InfoType>
UsuarioRegistrado<InfoType>* UsuarioSinRegistrar<InfoType>::Registrarse() {
  std::string Question = "\n¿Qué tipo de usuario será?";
  std::string Options = "1) Cliente\n2) Propietario\n";
  switch (QuestionAndSelection(1,2,Question,Options)) {
    case 1: return (new Cliente<InfoType>(middleware)); break;
    case 2: return (new Propietario<InfoType>(middleware)); break;
  }
  return nullptr;
}

/**
 * @brief Función para iniciar sesión
 * @tparam InfoType
 * @return UsuarioRegistrado<InfoType>*
 */
template <class InfoType>
UsuarioRegistrado<InfoType>* UsuarioSinRegistrar<InfoType>::IniciarSesion() {
  std::string email_input, password_input;
  bool founded = false;
  do
  {
    std::cout << "Introduce tu email -> ";
    std::cin >> email_input;
    founded = (middleware->SearchUsr(email_input) != -1);
    if (!founded) { std::cout << "\033[31mNo existe...\033[0m" << std::endl; }
  } while (!founded);
  // TOMAR CAMPOS DEL USR
  std::string raw_information = middleware->get_line(middleware->SearchUsr(email_input));
  // EMAIL:PASSWORD:CERRADURAS
  std::vector<std::string> fields = BreakDownFields(raw_information, ':');
  std::string email{fields[0]},  typeUsr{fields[1]}, password{fields[2]}, cerraduras{fields[3]};
  do
  {
    std::cout << "Introduce tu contraseña -> ";
    std::cin >> password_input;
    if (password_input != password) { std::cout << "\033[31mNo concuerdan, inténtalo de nuevo...\033[0m" << std::endl; }
  } while (password_input != password);
  switch (typeUsr[0]) {
    case 'C': return (new Cliente<InfoType>(email, password, middleware, BreakDownCerradurasInformation(cerraduras))); break;
    case 'P': return (new Propietario<InfoType>(email, password, middleware, BreakDownCerradurasInformation(cerraduras))); break;
    case 'A': return (new Administrador<InfoType>(email, password, middleware)); break;
    default: std::cout << "REBELDE\n"; break;
  }
  return nullptr;
}

#endif