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
 * 
 * @brief Archivo: main.cc
 *        Programa principal de Cerraduras Inteligentes
 */

#include <iostream>
#include <fstream>
#include <ctime>
#include "models/Interfaz.h"
#include "middleware/middleware.h"

using namespace std;
int main() {
  
  GenerateLogFile();

  Middleware<string> BaseDatos("BaseDatos.BS");
  BaseDatos.open();
  InterfazVisual<string> interfaz_visual;
  UsuarioRegistrado<std::string>* Usr;
  
  while ((Usr = interfaz_visual.ShowIdentifyPortal(&BaseDatos)) != nullptr)
    interfaz_visual.ShowUsrPortal(Usr);
  return 0;
}