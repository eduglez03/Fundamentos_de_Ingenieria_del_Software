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
 * @brief Archivo: UsuarioRegistrado.h
 *        Implementación de la clase "UsuarioRegistrado"
 *        ----> Constructores
 *        ----> Mostrar el portal
 *        ----> Configurar cuenta
 *        ----> Setters
 */

#ifndef USUARIOREGISTRADO_H
#define USUARIOREGISTRADO_H

#include <iostream>
#include <list>
#include <map> 
#include <string> 

#include "../middleware/middleware.h"
#include "UsuarioSinRegistrar.h"
#include "Cerradura.h"
#include "Utility.h"

template <class InfoType>
class UsuarioSinRegistrar;

/// Clase para el registro de nuevos usuarios
template <class InfoType>
class UsuarioRegistrado {
 public:
  UsuarioRegistrado() {};
  UsuarioRegistrado(std::string, std::string, std::string, std::string, std::string, Middleware<InfoType>*);
  UsuarioRegistrado(std::string email, std::string password, Middleware<InfoType>* BD) {
    email_ = email; 
    password_ = password; 
    BaseDatos_ = BD;
  };

  virtual void ShowPortal() = 0;
  void ConfigurarCuenta();

 protected:

  void SetName();
  void SetSurname();
  void SetEmail();
  void SetNumberMobile();
  void SetPassword();

  std::string name_;
  std::string surname_;
  std::string email_;
  std::string numbermobile_;
  std::string password_;
  Middleware<InfoType>* BaseDatos_;
};


/**
 * @brief Construye un nuevo Usuario Registrado <Info Type>:: Usuario Registrado object
 * @tparam InfoType 
 * @param[in] name 
 * @param[in] surname 
 * @param[in] email 
 * @param[in] numtlf 
 * @param[in] password 
 */
template <class InfoType>
UsuarioRegistrado<InfoType>::UsuarioRegistrado(std::string name, std::string surname, std::string email, std::string numtlf, std::string password, Middleware<InfoType>* middleware) {
  name_ = name;
  surname_ = surname;
  email_ = email;
  numbermobile_ = numtlf;
  password_ = password;
  BaseDatos_ = middleware;
}

// ---------SETTER DE PROPIEDADES DE LA CUENTA-------------

// + Se está ignorando la posibilidad de que el usuario introduzca el caracter ':' NO PUEDE PONERLO

/**
 * @brief Setter del nombre de usuario
 * @tparam InfoType 
 */
template <class InfoType>
void UsuarioRegistrado<InfoType>::SetName() {
  std::cout << "Introduce el nuevo nombre -> "; 
  std::cin >> name_;
}

/**
 * @brief Setter del apellido del usuario
 * @tparam InfoType 
 */
template <class InfoType>
void UsuarioRegistrado<InfoType>::SetSurname() {
  std::cout << "Introduce el nuevo nombre -> "; 
  std::cin >> surname_;
}

/**
 * @brief Setter email del usuario
 * @tparam InfoType 
 */
template <class InfoType>
void UsuarioRegistrado<InfoType>::SetEmail() {
  std::cout << "Introduce el nuevo email -> ";
  std::cin >> email_;
}

/**
 * @brief Setter numero del usuario
 * @tparam InfoType 
 */
template <class InfoType>
void UsuarioRegistrado<InfoType>::SetNumberMobile() {
  std::cout << "Introduce el nuevo número de teléfono -> "; 
  std::cin >> numbermobile_;
}

/**
 * @brief Setter contraseña del usuario
 * @tparam InfoType 
 */
template <class InfoType>
void UsuarioRegistrado<InfoType>::SetPassword() {
  std::cout << "Introduce la nueva contraseña -> "; 
  std::cin >> password_;
}

// ------------------------------------------------------------------

std::string Question = "¿Qué quieres modificar?";
std::string Options = "1) Nombre\n2) Apellido\n3) Email\n4) Teléfono\n5) Contraseña\n 0) Guardar y salir";

/**
 * @brief Menú para la configuracion de la cuenta de usuario
 * @tparam InfoType 
 */
template <class InfoType>
void UsuarioRegistrado<InfoType>::ConfigurarCuenta() {
  int seleccion;
  while ((seleccion = QuestionAndSelection(0, 5, Question, Options)) != 0) { // La seleccion del 0 es para salir
    switch (seleccion) {
      case 1: SetName(); break;
      case 2: SetSurname(); break;
      case 3: SetEmail(); break;
      case 4: SetNumberMobile(); break;
      case 5: SetPassword(); break;
    }
  }
}

#endif