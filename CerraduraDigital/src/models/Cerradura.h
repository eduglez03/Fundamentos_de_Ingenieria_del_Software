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
 * @brief Archivo: Cerradura.h
 *        Implementación de la clase "Cerradura"
 *        ----> Constructor
 *        ----> Abrir
 *        ----> Cerrar
 *        ----> GetName
 *        ----> GetState
 *        ----> GetCerraduraPermisions
 *        ----> AddClient
 */

#ifndef CERRADURA_H
#define CERRADURA_H

#include <iostream>
#include <list>

/// Clase para gestion de cerraduras
class Cerradura {
 public:
  Cerradura(std::string, bool, std::list<std::string>);

  // Métodos clase
  void Abrir();
  void Cerrar();
  std::string GetName() { return nombreCerradura_; }
  bool GetState() { return abierto_; }
  std::list<std::string> GetCerraduraPermisions() { return AccesoCerradura_; }
  void AddClient(std::string emailclient) { AccesoCerradura_.push_front(emailclient); }

 private:
  std::string nombreCerradura_;
  bool abierto_;
  std::list<std::string> AccesoCerradura_;
};

/**
 * @brief Crea cerradura
 * @param[in] nombreCerradura
 * @param[in] apertura
 * @param[in] AccesoCerradura
 */
Cerradura::Cerradura(std::string nombreCerradura, bool apertura, std::list<std::string> AccesoCerradura) {
  nombreCerradura_ = nombreCerradura;
  abierto_ = apertura;
  AccesoCerradura_ = AccesoCerradura;
}

/**
 * @brief Abre Cerradura
 */
void Cerradura::Abrir() {
  abierto_ = true;
}

/**
 * @brief Cierra Cerradura
 */
void Cerradura::Cerrar() {
  abierto_ = false;
}

#endif