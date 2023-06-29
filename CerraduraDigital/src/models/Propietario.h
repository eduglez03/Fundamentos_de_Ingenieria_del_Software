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
 * @brief Archivo: Propietario.h
 *        Implementación de la clase "Propietario"
 *        ----> Constructores
 *        ----> Guardar información
 *        ----> Mostrar el portal
 *        ----> Dar permisos a un cliente
 *        ----> Añadir cerradura
 */

#ifndef PROPIETARIO_H
#define PROPIETARIO_H

#include <iostream>
#include <list>
#include <map>

#include "Cerradura.h"
#include "UsuarioRegistrado.h"
#include "Cliente.h"

/// Clase para la gestion de usuarios de las cerraduras
template <class InfoType>
class Propietario : public UsuarioRegistrado<InfoType> {
 public:
  /// Constructor registrarse
  Propietario(Middleware<InfoType>*);
  /// Constructor iniciar sesión
  Propietario(std::string email, std::string password, Middleware<InfoType>* BD, std::map<std::string, Cerradura> Cerraduras);

  /// Métodos
  void SaveInformation();
  void ShowPortal() override;
  void CheckCerraduraState();
  void AddClienteAcerradura();
  void AddCerradura();

 private:
  std::map<std::string, Cerradura> TodasCerradura_;
};

/**
 * @brief Método que guarda la información
 * @tparam InfoType 
 * @return void
 */
template <class InfoType>
void Propietario<InfoType>::SaveInformation() {
  std::string raw_information = ConstructRawInformation(UsuarioRegistrado<InfoType>::email_, "P", UsuarioRegistrado<InfoType>::password_, TodasCerradura_);
  int line = UsuarioRegistrado<InfoType>::BaseDatos_->SearchUsr(UsuarioRegistrado<InfoType>::email_);
  UsuarioRegistrado<InfoType>::BaseDatos_->insert_in_line(line, raw_information); 
  for (auto &i : TodasCerradura_) {
    Middleware<std::string> CerraduraFile("src/CERRADURAS_DATA/" + i.second.GetName() + ".crd");
    CerraduraFile.open();
    raw_information = ConstructRawCerradurasInformation(i.second);
    CerraduraFile.writeUniqueLine(raw_information);
  }
}

/**
 * @brief Constructor registrarse
 * @param[out] BD Base de datos 
 * @tparam InfoType 
 * @return void
 */
template <class InfoType>
Propietario<InfoType>::Propietario(Middleware<InfoType>* BD) {
  UsuarioRegistrado<InfoType>::BaseDatos_ = BD;
  std::cout << "Introduce tu correo electrónico -> "; 
  std::cin >> UsuarioRegistrado<InfoType>::email_;
  std::cout << "Introduce tu contraseña -> "; 
  std::cin >> UsuarioRegistrado<InfoType>::password_;
  std::string raw_information = ConstructRawInformation(UsuarioRegistrado<InfoType>::email_, "P", UsuarioRegistrado<InfoType>::password_, TodasCerradura_);
  UsuarioRegistrado<InfoType>::BaseDatos_->insert_new_line_back(raw_information);
  AddActualLogInfo("Propietario " + UsuarioRegistrado<InfoType>::email_ + " ha iniciado sesión" + "\n");
  SaveInformation();
}

/**
 * @brief Constructor iniciar sesión
 * @param[out] BD Base de datos 
 * @tparam InfoType 
 * @return void
 */
template <class InfoType>
Propietario<InfoType>::Propietario(std::string email, std::string password, Middleware<InfoType>* BD, std::map<std::string, Cerradura> Cerraduras) : UsuarioRegistrado<InfoType>(email, password, BD) {
  TodasCerradura_ = Cerraduras;
  AddActualLogInfo("Propietario " + UsuarioRegistrado<InfoType>::email_ + " ha iniciado sesión" + "\n");
  SaveInformation();
}

/**
 * @brief Dar permisos a un cliente
 * @tparam InfoType 
 * @return void
 */
template <class InfoType>
void Propietario<InfoType>::AddClienteAcerradura() {
  std::string inputCorreo, inputCerraduraName;
  std::cout << "Correo del cliente -> ";
  std::cin >> inputCorreo;
  // No está el usuario en la base de datos
  int lineUsrUbication = UsuarioRegistrado<InfoType>::BaseDatos_->SearchUsr(inputCorreo);
  if (lineUsrUbication == -1) {
    std::cout << "\033[31mNo existe ese usuario en la base de datos...\n\033[0m";
    return;
  } else {
    std::cout << "Introduce el nombre de la cerradura a la que quieres agregarlo -> ";
    std::cin >> inputCerraduraName;
    auto value = TodasCerradura_.find(inputCerraduraName);
    if (value != TodasCerradura_.end()) {
      value->second.AddClient(inputCorreo);
      // Crear el objeto del cliente para agregarlo
      std::string raw_information = UsuarioRegistrado<InfoType>::BaseDatos_->get_line(lineUsrUbication);
      // EMAIL:PASSWORD:CERRADURAS
      std::vector<std::string> fields = BreakDownFields(raw_information, ':');
      std::string email{fields[0]},  typeUsr{fields[1]}, password{fields[2]}, cerraduras{fields[3]};
      Cliente<InfoType> client(email, password, UsuarioRegistrado<InfoType>::BaseDatos_, BreakDownCerradurasInformation(cerraduras));
      client.AddCerradura(value->second);
      client.SaveInformation();
      std::cout << "Se ha añadido los permisos al usuario indicado\n";
    } else {
      std::cout << "\033[31mNo existe esa cerradura en la base de datos...\n\033[0m";
      return;
    }
  }
  AddActualLogInfo("Propietario " + UsuarioRegistrado<InfoType>::email_ + " ha dado permisos en la cerradura " + inputCerraduraName + " a " + inputCorreo + "\n");
  SaveInformation();
}

/**
 * @brief Añadir una nueva cerradura
 * @tparam InfoType 
 * @return void
 */
template <class InfoType>
void Propietario<InfoType>::AddCerradura() {
  std::string nombre_cerradura;
  std::cout << "Nombre de la nueva cerradura: ";
  std::cin >> nombre_cerradura;
  TodasCerradura_.insert(std::pair<std::string,Cerradura>(nombre_cerradura, Cerradura(nombre_cerradura, false, std::list<std::string>())));
  AddActualLogInfo("Propietario " + UsuarioRegistrado<InfoType>::email_ + " ha creado la cerradura " + nombre_cerradura + "\n");
  SaveInformation();
}

/**
 * @brief Mostrar el portal del propietario
 * @tparam InfoType 
 * @return void
 */
template <class InfoType>
void Propietario<InfoType>::CheckCerraduraState() {
  std::cout << "Tus cerraduras:\n";
  for (auto &&i : TodasCerradura_) {
    std::cout << i.second.GetName() << ": ";
    for (auto &&j : i.second.GetCerraduraPermisions()) {
      std::cout << "(" << j << ")";
    }
    std::cout << std::endl;
  }
  AddActualLogInfo("Propietario " + UsuarioRegistrado<InfoType>::email_ + " ha checkeado todas sus cerraduras" + "\n");
  SaveInformation();
}

template <class InfoType>
void Propietario<InfoType>::ShowPortal() {
  std::cout << "\033[1m\n------------ Portal del Propietario ------------\033[0m" << std::endl;
  std::string Question = "\n¿Qué quieres hacer?";
  std::string Options = "1) Añadir cerradura\n2) Añadir permisos a cliente\n3) Listar cerraduras en propiedad\n0) Salir del portal de propietario\n";
  int eleccion{-1};
  while (eleccion != 0) {
    eleccion = QuestionAndSelection(0, 3, Question, Options);
    switch (eleccion) {
      case 0: break;
      case 1: AddCerradura(); break;
      case 2: AddClienteAcerradura(); break;
      case 3: CheckCerraduraState(); break;
    }
    SaveInformation();
  }
}

#endif