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
 * @brief Archivo: Administrador.h
 *        Implementación de la clase "Administrador"
 *        ----> Constructor
 *        ----> Comprobar logs
 *        ----> Portal
 */

#ifndef ADMINISTRADOR_H
#define ADMINISTRADOR_H

#include <iostream>
#include <list>
#include <map>

#include "Cerradura.h"
#include "UsuarioRegistrado.h"
#include "UsuarioSinRegistrar.h"
#include "Utility.h"



// Clase para la gestion de usuarios de las cerraduras
template <class InfoType>
class Administrador : public UsuarioRegistrado<InfoType> {
 public:
  // Constructor registrarse
  Administrador(Middleware<InfoType>*);
  // Constructor inicio sesion
  Administrador(std::string email, std::string password, Middleware<InfoType>* BD);
  // Métodos
  void CheckLogs();
  void ShowPortal();
  void ManageUsers();
 private:
  std::map<std::string, Cerradura> Cerraduras_;
};

/**
 * @brief Constructor registrarse
 * @tparam InfoType
 * @param[in] BD
 */
template <class InfoType>
Administrador<InfoType>::Administrador(Middleware<InfoType>* BD) {
  UsuarioRegistrado<InfoType>::BaseDatos_ = BD;
  std::cout << "Introduce tu correo electrónico -> ";
  std::cin >> UsuarioRegistrado<InfoType>::email_;
  std::cout << "Introduce tu contraseña -> ";
  std::cin >> UsuarioRegistrado<InfoType>::password_;
  std::string raw_information = ConstructRawInformation(UsuarioRegistrado<InfoType>::email_, "A", UsuarioRegistrado<InfoType>::password_, Cerraduras_);
  UsuarioRegistrado<InfoType>::BaseDatos_->insert_new_line_back(raw_information);
}

/**
 * @brief Constructor iniciar sesión
 * @tparam InfoType
 * @param[in] email
 * @param[in] password
 * @param[in] BD
 */
template <class InfoType>
Administrador<InfoType>::Administrador(std::string email, std::string password, Middleware<InfoType>* BD) : UsuarioRegistrado<InfoType>(email, password, BD) {}

/**
 * @brief Función comprobar logs
 * @tparam InfoType
 */
template <class InfoType>
void Administrador<InfoType>::CheckLogs() {
  std::string dia{""};
  std::cout << "¿De qué día quieres recibir los registros?\nEscríbelo en el formato (D)D-(M)M-YYYY\n -> ";
  std::cin >> dia;
  Middleware<std::string> FicheroDiaConcreto("src/logs/" + dia + ".log");
  if (FicheroDiaConcreto.open()) {  // Abrio el archivo log
    std::cout << "\nInformación del día " << dia << ":\n"
              << FicheroDiaConcreto.GetAllInformation() << std::endl;
  } else {  // No pudo abrir el archivo
    std::cout << "\033[31mNo se encontró el log.\n\033[0m";
  }
}

template <class InfoType>
void Administrador<InfoType>::ManageUsers() {
  std::string user_email;
  std::cout << "Indica el correo del usuario que deseas actualizar -> ";
  std::cin >> user_email;
  int lineUsrUbication = UsuarioRegistrado<InfoType>::BaseDatos_->SearchUsr(user_email);
  if (lineUsrUbication == -1) {
    std::cout << "\033[31mNo existe ese usuario en la base de datos...\n\033[0m";
    return;
  } else {
    std::string Question = "\n¿Qué quieres hacer?";
    std::string Options = "1) Cambiar contraseña\n2) Cambiar rol\n3) Cambiar correo electrónico\n0) Salir del portal de gestión de usuarios\n";
    int eleccion{-1};
    while (eleccion != 0) {
      eleccion = QuestionAndSelection(0, 3, Question, Options);
      switch (eleccion) {
        case 1: {
          std::string password1, password2;
          lineUsrUbication = UsuarioRegistrado<InfoType>::BaseDatos_->SearchUsr(user_email);
          // Doble autenticacion al cambiar contraseña
          do {
            std::cout << "Ingrese una nueva contraseña: ";
            std::cin >> password1;
            std::cout << "Ingrese la nueva contraseña nuevamente: ";
            std::cin >> password2;
            if (password1 == password2) {
              std::cout << "Las contraseñas coinciden. La nueva contraseña es: " << password1 << std::endl;
            } else {
              std::cout << "\033[31mLas contraseñas no coinciden. Inténtelo de nuevo.\033[0m" << std::endl;
            }
          } while (password1 != password2);
          // EMAIL:TYPE:PASSWORD:CERRADURAS
          std::string raw_information = UsuarioRegistrado<InfoType>::BaseDatos_->get_line(lineUsrUbication);
          std::vector<std::string> fields_usr = BreakDownFields(raw_information, ':');
          std::string usr_email{fields_usr[0]},  type_usr{fields_usr[1]}, password_usr{password1}, cerraduras_usr{fields_usr[3]}; 
          std::string new_raw_information = ConstructRawInformation(user_email, type_usr, password_usr, BreakDownCerradurasInformation(cerraduras_usr));
          UsuarioRegistrado<InfoType>::BaseDatos_->insert_in_line(lineUsrUbication, new_raw_information);
          AddActualLogInfo(UsuarioRegistrado<InfoType>::email_ + " ha modificado la contraseña " + fields_usr[2] + " por " + password1 + "\n");
        }
        break;
        case 2: {
            std::string new_type;
            lineUsrUbication = UsuarioRegistrado<InfoType>::BaseDatos_->SearchUsr(user_email);
            do {
              std::cout << "Indica el nuevo rol del usuario (A, P, C) -> ";
              std::cin >> new_type;
              if (new_type != "A" && new_type != "P" && new_type != "C") {
                std::cout << "\033[31mEl rol introducido no es válido...\n\033[0m";
              }
            } while (new_type != "A" && new_type != "P" && new_type != "C");
            std::string raw_information = UsuarioRegistrado<InfoType>::BaseDatos_->get_line(lineUsrUbication);
            std::vector<std::string> fields_usr = BreakDownFields(raw_information, ':');
            std::string usr_email{fields_usr[0]},  type_usr{new_type}, password_usr{fields_usr[2]}, cerraduras_usr{fields_usr[3]}; 
            std::string new_raw_information = ConstructRawInformation(user_email, type_usr, password_usr, BreakDownCerradurasInformation(cerraduras_usr));
            UsuarioRegistrado<InfoType>::BaseDatos_->insert_in_line(lineUsrUbication, new_raw_information);
            AddActualLogInfo(UsuarioRegistrado<InfoType>::email_ + " ha modificado el rol " + fields_usr[1] + " por " + new_type + "\n");         
          }
          break;
        case 3: {
          std::string new_email;
          lineUsrUbication = UsuarioRegistrado<InfoType>::BaseDatos_->SearchUsr(user_email);
          do {
            std::cout << "Indica el nuevo correo del usuario -> ";
            std::cin >> new_email;
            if (UsuarioRegistrado<InfoType>::BaseDatos_->SearchUsr(new_email) != -1) {
              std::cout << "\033[31mEl correo introducido ya existe...\n\033[0m";
            }
          } while (UsuarioRegistrado<InfoType>::BaseDatos_->SearchUsr(new_email) != -1);
          user_email = new_email;
          std::string raw_information = UsuarioRegistrado<InfoType>::BaseDatos_->get_line(lineUsrUbication);
          std::vector<std::string> fields_usr = BreakDownFields(raw_information, ':');
          std::string type_usr{fields_usr[1]}, password_usr{fields_usr[2]}, cerraduras_usr{fields_usr[3]};
          std::string new_raw_information = ConstructRawInformation(new_email, type_usr, password_usr, BreakDownCerradurasInformation(cerraduras_usr));
          UsuarioRegistrado<InfoType>::BaseDatos_->insert_in_line(lineUsrUbication, new_raw_information);
          std::cout << "El correo ha sido modificado correctamente...\n";
          AddActualLogInfo(UsuarioRegistrado<InfoType>::email_ + " ha modificado el correo " + fields_usr[0] + " por " + new_email + "\n");
        }
        break;
        case 0: break;
      }
    }
  }
}

/**
 * @brief Función muestra el menú
 * @tparam InfoType
 */
template <class InfoType>
void Administrador<InfoType>::ShowPortal() {
  std::cout << "\033[1m\n------------ Portal del Administrador ------------\033[0m" << std::endl;
  std::string Question = "\n¿Qué quieres hacer?";
  std::string Options = "1) Comprobar registros\n2) Gestionar usuarios\n0) Salir del portal de administrador\n";
  int eleccion{-1};
  while (eleccion != 0) {
    eleccion = QuestionAndSelection(0, 2, Question, Options);
    switch (eleccion) {
      case 1:
        CheckLogs();
        break;
      case 2:
        ManageUsers();
        break;
      case 0:
        break;
    }
  }
}

#endif