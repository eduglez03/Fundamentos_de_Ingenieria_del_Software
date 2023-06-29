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
 * @brief Archivo: Utility.h
 */

#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <vector>
#include <map>

#include "Cerradura.h"
#include "../middleware/middleware.h"

/**
 * @brief Función para la selección de opciones
 * @param[in] int 
 * @param[in] int 
 * @param[in] string 
 * @param[in] string 
 * @return int
 */
int QuestionAndSelection (int cotaInf, int cotaSup, std::string Question, std::string Options) {
  int eleccion;
  std::string entrada;
  do {
    std::cout << Question << std::endl;
    std::cout << Options << std::endl;
    std::cout << "\033[36mSelección -> \033[0m"; 
    std::cin >> entrada;
    try {
      eleccion = std::stoi(entrada);
    } catch (std::invalid_argument const &e) {
      std::cout << "\033[31mDebe ingresar un número válido\n\033[0m";
      continue;
    } catch (std::out_of_range const &e) {
      std::cout << "\033[31mEl número ingresado es demasiado grande o demasiado pequeño\n\033[0m";
      continue;
    }
    if (eleccion < cotaInf || eleccion > cotaSup) { 
      std::cout << "\033[31mEsa selección no está contemplada entre las posibilidades\n\033[0m"; 
    }    
  } while (eleccion < cotaInf || eleccion > cotaSup);
  return eleccion;
}

/**
 * @brief Función para separar la información contenida en la base de datos
 * @param[in] string 
 * @param[in] char 
 * @return std::vector<std::string>
 */
std::vector<std::string> BreakDownFields(std::string raw_information, char delimitador) {
  std::vector<std::string> fields; // numero de campos de informacion que hay
  std::string buffer{""};
  for (int i = 0; i < (int)raw_information.size(); i++) {
    if (raw_information[i] != delimitador) {
      buffer += raw_information[i];
    } else {
      fields.push_back(buffer);
      buffer = "";
    }
  }
  fields.push_back(buffer);
  return fields;
}

/**
 * @brief Función para obtener el estado de la cerradura
 * @param[in] bool 
 * @param[out] string 
 */
void GetCerraduraProperty(const std::string &CerraduraInfo, bool &abierto, std::list<std::string> &usersPermisions) {
  // Separador de propiedades -> '|'
  std::vector<std::string> propertyfields = BreakDownFields(CerraduraInfo, '|');
  if (propertyfields[0] == "OPEN") { abierto = true; } 
  else { abierto = false; }
  for (int i = 1; i < (int)propertyfields.size(); i++)
    usersPermisions.push_front(propertyfields[i]);
}

/**
 * @brief Función para construir una cerradura
 * @param[in] string 
 * @return Cerradura
 */
Cerradura ConstructCerradura(const std::string &CerraduraFileName) {
  std::string buffer{""};
  // Propiedades de la cerradura
  std::string nameCerradura;
  std::list<std::string> usersPermisions;
  bool abierto;
  Middleware<std::string> cerraduraFile(CerraduraFileName);
  cerraduraFile.open();
  std::string CerraduraInfo = cerraduraFile.get_line(1);
  for (int i = 0; i < (int)CerraduraInfo.size(); i++) {
    if (CerraduraInfo[i] == '[') {
      nameCerradura = buffer;
      buffer = "";
    } else if (CerraduraInfo[i] == ']') {
      GetCerraduraProperty(buffer, abierto, usersPermisions);
      return Cerradura(nameCerradura, abierto, usersPermisions);
      buffer = "";
    }
    if (CerraduraInfo[i] != '[' && CerraduraInfo[i] != ']') {
      buffer += CerraduraInfo[i];
    }
  }
  return Cerradura("ERROR", 0, std::list<std::string>());
}

/**
 * @brief Función para descomponer la información sobre la cerradura
 * @param[in] string 
 * @return map<string,Cerradura>
 */
std::map<std::string,Cerradura> BreakDownCerradurasInformation (std::string raw_cerraduras_information) {
  std::map<std::string,Cerradura> ListCerradura;
  std::string nameFile{""};
  // std::cout << "raw_cerraduras_information: " << raw_cerraduras_information << std::endl;
  for (int i = 0; i < (int)raw_cerraduras_information.size(); i++) {
    if (raw_cerraduras_information[i] == '(') {
      // Nada
    } else if (raw_cerraduras_information[i] != ')') {
      nameFile += raw_cerraduras_information[i];
    } else {
      // std::cout << "nameFile: " << nameFile << std::endl;
      Cerradura aux = ConstructCerradura("src/CERRADURAS_DATA/" + nameFile + ".crd");
      ListCerradura.insert(std::pair<std::string, Cerradura>(aux.GetName(), aux));
      nameFile = "";
    }
  }
  return ListCerradura;
}

/**
 * @brief Construye la información pertinente a las cerraduras
 * @param[in] map <string, Cerradura> 
 * @return string
 */
std::string ConstructRawCerradurasInformation(Cerradura Cerradura) {
  std::string raw_cerraduras_information;
  raw_cerraduras_information += Cerradura.GetName();
  // Se utilza el nombre de la llave con ".crd" para indicar su archivo correspondiente
  raw_cerraduras_information += "[";
  // Indicar Estado cerradura
  if (Cerradura.GetState()) { raw_cerraduras_information += "OPEN"; }
  else {raw_cerraduras_information += "CLOSE";}
  // Indicar Usuarios con permisos
  for (auto &&j : Cerradura.GetCerraduraPermisions()) {
    raw_cerraduras_information += "|";
    raw_cerraduras_information += j;
  }
  raw_cerraduras_information += "]";
  return raw_cerraduras_information;
}

/**
 * @brief Función para la construcción de la información de usuario
 * @param[in] string 
 * @param[in] string 
 * @param[in] string 
 * @param[in] map<string, Cerradura> 
 * @return string
 */
std::string ConstructRawInformation(std::string email, std::string typeUsr, std::string password, std::map<std::string,Cerradura> Cerraduras) {
  std::string raw_information{email};
  raw_information += ":";
  raw_information += typeUsr;
  raw_information += ":";
  raw_information += password;
  raw_information += ":";
  for (auto &&i : Cerraduras) {
    raw_information += "(";
    raw_information += i.first;
    raw_information += ")";
  }
  return raw_information;
}

/**
 * @brief Función para la creación un log de información
 * @param[in] time_t 
 * @param[in] time_t 
 * @return string
 */
std::string CreateRutaActualLog(time_t date, tm* date_struct) {
  int day = date_struct->tm_mday;
  int month = date_struct->tm_mon + 1;
  int year = date_struct->tm_year + 1900;
  return ("src/logs/" + std::to_string(day) + "-" + std::to_string(month) + "-" + std::to_string(year) + ".log");
}

/**
 * @brief Función para la creación archivo log de información
 */
void GenerateLogFile() {
  time_t date = time(0);
  tm* date_struct = localtime(&date);
  Middleware<std::string> LogFile(CreateRutaActualLog(date, date_struct));
  if (!LogFile.open()) { LogFile.openCreate(); }
}

/**
 * @brief Función para la añadir un log de información
 * @param[in] string 
 */
void AddActualLogInfo (const std::string &action) {
  time_t date = time(0);
  tm* date_struct = localtime(&date);
  GenerateLogFile();
  Middleware<std::string> LogDay(CreateRutaActualLog(date, date_struct));
  LogDay.insert_new_line_back(action);
}

#endif