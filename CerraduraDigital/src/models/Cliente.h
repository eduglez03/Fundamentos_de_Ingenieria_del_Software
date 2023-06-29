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
 * @brief Archivo: Cliente.h
 *        Implementación de la clase "Cliente"
 *        ----> Constructores
 *        ----> Comprobar estado
 *        ----> Guardar información
 *        ----> Enseñar portal
 *        ----> Abrir cerradura
 *        ----> Cerrar cerradura
 */

#ifndef CLIENTE_H
#define CLIENTE_H

#include <iostream>
#include <list>
#include <map>
#include <regex>

#include "Cerradura.h"
#include "UsuarioSinRegistrar.h"
#include "UsuarioRegistrado.h"
#include "Utility.h"

/// Clase para la gestión de usuarios de las cerraduras
template <class InfoType>
class Cliente : public UsuarioRegistrado<InfoType> {
 public:
  /// Constructor registrarse
  Cliente(Middleware<InfoType>*);
  /// Constructor inicio sesión
  Cliente(std::string email, std::string password, Middleware<InfoType>* BD, std::map<std::string, Cerradura> Cerraduras);

  /// Métodos
  void CheckCerraduraState();
  void SaveInformation();
  void ShowPortal() override;
  void Abrir();
  void Cerrar();
  void AddCerradura(Cerradura);

 private:
  std::map<std::string, Cerradura> Cerraduras_;
};

/**
 * @brief Guarda la información del cliente
 * @tparam InfoType 
 * @return void
 */
template <class InfoType>
void Cliente<InfoType>::SaveInformation() {
  std::string raw_information = ConstructRawInformation(UsuarioRegistrado<InfoType>::email_, "C", UsuarioRegistrado<InfoType>::password_, Cerraduras_);
  int line = UsuarioRegistrado<InfoType>::BaseDatos_->SearchUsr(UsuarioRegistrado<InfoType>::email_);
  UsuarioRegistrado<InfoType>::BaseDatos_->insert_in_line(line, raw_information); 
  for (auto &i : Cerraduras_) {
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
 */
template <class InfoType>
Cliente<InfoType>::Cliente(Middleware<InfoType>* BD) {
  UsuarioRegistrado<InfoType>::BaseDatos_ = BD;
  std::regex email_regex("^[a-zA-Z0-9._%+-]+@(gmail|hotmail)\\.(com)$");
  std::string email_input;
  do {
    std::cout << "Introduce tu correo electrónico -> "; 
    std::cin >> email_input;
     if (!std::regex_match(email_input, email_regex)) {
        std::cout << "El formato del correo electrónico no es válido." << std::endl;
        std::cout << "El formato debe ser: xxxxxx@gmail.com o xxxxxx@hotmail.com" << std::endl;
        continue;
      }
  } while (!std::regex_match(email_input, email_regex));
  UsuarioRegistrado<InfoType>::email_ = email_input;
  std::cout << "Introduce tu contraseña -> "; 
  std::cin >> UsuarioRegistrado<InfoType>::password_;
  std::string raw_information = ConstructRawInformation(UsuarioRegistrado<InfoType>::email_, "C", UsuarioRegistrado<InfoType>::password_, Cerraduras_);
  UsuarioRegistrado<InfoType>::BaseDatos_->insert_new_line_back(raw_information);
  AddActualLogInfo("Cliente " + UsuarioRegistrado<InfoType>::email_ + " ha iniciado sesión" + "\n");
  SaveInformation();
}

/**
 * @brief Constructor iniciar sesión
 * @param[in] email 
 * @param[in] password 
 * @param[out] BD Base de datos
 * @param[in] Cerraduras 
 * @tparam InfoType 
 */
template <class InfoType>
Cliente<InfoType>::Cliente(std::string email, std::string password, Middleware<InfoType>* BD, std::map<std::string, Cerradura> Cerraduras) : UsuarioRegistrado<InfoType>(email, password, BD) {
  Cerraduras_ = Cerraduras;
  AddActualLogInfo("Cliente " + UsuarioRegistrado<InfoType>::email_ + " ha iniciado sesión" + "\n");
}

/**
 * @brief Abrir una cerradura
 * @tparam InfoType 
 * @return void
 */
template <class InfoType>
void Cliente<InfoType>::Abrir() {
  std::string nameCerraduraInput;
  std::cout << "¿Cómo se llama la cerradura que quieres abrir? -> ";
  std::cin >> nameCerraduraInput;
  // EXISTE
  auto value = Cerraduras_.find(nameCerraduraInput);
  if (value != Cerraduras_.end()) {
    value->second.Abrir();
    std::cout << "\n" << value->first << ": " << ((value->second.GetState()) ? "Abierta\n" : "Error al abrir la cerradura\n");
    AddActualLogInfo("Cliente " + UsuarioRegistrado<InfoType>::email_ + " ha abierto la cerradura "+ nameCerraduraInput + "\n");
  } else {
    std::cout << "No existe esa cerradura..." << std::endl;
    AddActualLogInfo("Cliente " + UsuarioRegistrado<InfoType>::email_ + " ha intentado abrir la cerradura "+ nameCerraduraInput + "\n");
  } 
  SaveInformation();
}

/**
 * @brief Cerrar una cerradura
 * @tparam InfoType 
 * @return void
 */
template <class InfoType>
void Cliente<InfoType>::Cerrar() {
  std::string nameCerraduraInput;
  std::cout << "¿Cómo se llama la cerradura que quieres cerrar? -> ";
  std::cin >> nameCerraduraInput;
  // EXISTE
  auto value = Cerraduras_.find(nameCerraduraInput);
  if (value != Cerraduras_.end()) {
    value->second.Cerrar();
    std::cout << "\n" << value->first << ": " << ((value->second.GetState()) ? "\033[31mError al cerrar la cerraduraura\n\033[0m" : "Cerrada\n");
    AddActualLogInfo("Cliente " + UsuarioRegistrado<InfoType>::email_ + " ha cerrado la cerradura "+ nameCerraduraInput + "\n");
  } else {
    std::cout << "\033[31mNo existe esa cerradura...\033[0m" << std::endl;
    AddActualLogInfo("Cliente " + UsuarioRegistrado<InfoType>::email_ + " ha intentado cerrar la cerradura "+ nameCerraduraInput + "\n");
  } 
  SaveInformation();
}

/**
 * @brief Comprobar estado de una cerradura
 * @tparam InfoType 
 * @return void
 */
template <class InfoType>
void Cliente<InfoType>::CheckCerraduraState() {
  std::cout << "Tus cerraduras:\n";
  for (auto &&i : Cerraduras_) {
    std::cout << i.second.GetName() << ": " << ((i.second.GetState()) ? "OPEN" : "CLOSE") << std::endl;
  }
  AddActualLogInfo("Cliente " + UsuarioRegistrado<InfoType>::email_ + " ha checkeado todas sus cerraduras" + "\n");
}

template <class InfoType>
void Cliente<InfoType>::AddCerradura(Cerradura cerradura) {
  Cerraduras_.insert(std::pair<std::string, Cerradura>(cerradura.GetName(), cerradura));
}

/**
 * @brief Enseñar el portal
 * @tparam InfoType 
 * @return void
 */
template <class InfoType>
void Cliente<InfoType>::ShowPortal() {
  std::cout << "\033[1m\n------------ Portal del Cliente ------------\033[0m" << std::endl;
  std::string Question = "\n¿Qué quieres hacer?";
  std::string Options = "1) Abrir cerradura\n2) Cerrar cerradura\n3) Mostrar estado cerraduras\n0) Salir del portal de cliente\n";
  int eleccion{-1};
  while (eleccion != 0) {
    eleccion = QuestionAndSelection(0, 3, Question, Options);
    switch (eleccion) {
      case 1: Abrir(); break;
      case 2: Cerrar(); break;
      case 3: CheckCerraduraState(); break;
      case 0: break;
    } 
    SaveInformation();
  }
}

#endif