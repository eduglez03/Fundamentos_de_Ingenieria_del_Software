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
 * @brief Archivo: Interfaz.h
 *        Implementación de la clase "Interfaz"
 *        ----> Constructor
 *        ----> Mostrar el portal
 */

#ifndef INTERFAZ_H
#define INTERFAZ_H

#include <iostream>

#include "Utility.h"
#include "../middleware/middleware.h"
#include "UsuarioSinRegistrar.h"
#include "UsuarioRegistrado.h"

/// Clase para la gestión de la interfaz de usuario
template <class InfoType>
class InterfazVisual {
 public:
  UsuarioRegistrado<InfoType>* ShowIdentifyPortal(Middleware<InfoType>* BD);
  void ShowUsrPortal(UsuarioRegistrado<InfoType>*);
};

/**
 * @brief Muestra la interfaz de identificación de usuarios
 * @param[out] BD Base de datos 
 * @tparam InfoType 
 * @return UsuarioRegistrado<InfoType>* 
 */
template <class InfoType>
UsuarioRegistrado<InfoType>* InterfazVisual<InfoType>::ShowIdentifyPortal(Middleware<InfoType>* BD) {
  UsuarioSinRegistrar<InfoType> usr(BD);
  int eleccion{-1};  
  std::string Bienvenida = "\033[1m\n¡Bienvenido al portal de Cerraduras Digitales!\n\033[0m";
  std::string Question = "\n¿Qué deseas hacer?";
  std::string Options = "1) Registrarse\n2) Iniciar sesión\n0) Salir del servicio\n";
  while (eleccion != 0) {
    std::cout << Bienvenida;
    switch (eleccion = QuestionAndSelection(0, 2, Question, Options)) {
      case 1: return usr.Registrarse(); break;
      case 2: return usr.IniciarSesion(); break;
      case 0: std::cout << "Saliendo del portal...\nGuardando datos...\n"; break;
      default: /* SI LLEGA AQUI HAY UN ERROR (ABORTAR PROGRAMA)*/ break;
    }
  }
  return nullptr;
}

/**
 * @brief Muestra el portal
 * @param[in] usr Usuario
 * @tparam InfoType 
 * @return void
 */
template <class InfoType>
void InterfazVisual<InfoType>::ShowUsrPortal(UsuarioRegistrado<InfoType>* usr) {
  usr->ShowPortal();
}

#endif