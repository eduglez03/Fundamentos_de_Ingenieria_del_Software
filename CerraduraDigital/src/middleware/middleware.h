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
 * @brief Archivo: Middleware.h
 *        Implementación de la clase "Middleware"
 *        ----> Constructor
 *        ----> Insertar una línea
 *        ----> Localizar una línea
 *        ----> Cambiar una línea
 *        ----> Localizar un substring
 */

#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>

template <class InfoType>
class Middleware {
 public:
  /// Constructor
  Middleware(std::string);

  // Métodos para abrir, cerrar y refrescar
  bool open();
  bool openCreate();
  bool open(std::string);
  bool close();
  bool refresh();
  bool is_open();

  /// Métodos para insertar

  bool insert(InfoType);
  bool insert_in_line(int, InfoType);
  void DeleteBlankLines();
  bool insert_new_line_back(InfoType);
  std::string GetAllInformation();
  void writeUniqueLine(InfoType);
  void sobrescribir_linea(int numero_linea, const std::string& nueva_linea);
  std::string get_line(int);

  int SearchUsr(std::string);

 private:
  bool arrive_at_line(int);
  std::string file_name_;
  std::fstream file_;
};

/**
 * @brief Función que sobrescribe una línea
 * @tparam InfoType
 * @param[in] numero_linea
 * @param[in] nueva_linea
 */
template <class InfoType>
void Middleware<InfoType>::sobrescribir_linea(int numero_linea, const std::string& nueva_linea) {
  open();
  std::string linea;
  int i = 1;
  std::streampos posicion_sobrescribir = 0;
  bool encontrado = false;
  while (std::getline(file_, linea)) {
    if (i == numero_linea) {
      posicion_sobrescribir = file_.tellg();
      encontrado = true;
      break;
    }
    i++;
  }
  file_.seekp(posicion_sobrescribir);
  std::string linea_anterior;
  std::getline(file_, linea_anterior);
  std::streampos posicion_fin_linea = file_.tellg();
  file_.seekp(posicion_sobrescribir);
  file_ << nueva_linea;
  if (linea_anterior.length() > nueva_linea.length()) {
    file_ << std::string(linea_anterior.length() - nueva_linea.length(), ' ');
  } else if (linea_anterior.length() < nueva_linea.length()) {
    std::streampos posicion_actual = file_.tellg();
    std::string resto_archivo = "";

    while (std::getline(file_, linea)) {
      resto_archivo += linea + '\n';
    }
    file_.seekp(posicion_sobrescribir + nueva_linea.length());
    file_ << resto_archivo;
  }
  file_.close();
}

/**
 * @brief Constructor de la clase Middleware
 * @tparam InfoType
 * @param[in] string
 */
template <class InfoType>
Middleware<InfoType>::Middleware(std::string filename) {
  file_name_ = filename;
}

/**
 * @brief Función para comprobar si abrir un fichero
 * @tparam InfoType
 */
template <class InfoType>
bool Middleware<InfoType>::open() {
  file_.open(file_name_, std::fstream::in | std::fstream::out);
  return file_.is_open();
}

/**
 * @brief funcion para crear un fichero al leerlo
 * @tparam InfoType
 * @param[in] string
 */

template <class InfoType>
bool Middleware<InfoType>::openCreate() {
  file_.open(file_name_, std::fstream::out);
  return file_.is_open();
}

/**
 * @brief Función para la lectura de los permisos del archivo
 * @tparam InfoType
 * @param[in] type
 */
template <class InfoType>
bool Middleware<InfoType>::open(std::string type) {
  std::ios_base::openmode mode;
  // Lectura parametros permisos
  for (size_t i = 0; i < type.size(); i++) {
    switch (type[i]) {
      case 'r':
        mode |= std::fstream::in;
        break;
      case 'w':
        mode |= std::fstream::out;
        break;
      case 'e':
        mode |= std::fstream::ate;
        break;
      case 't':
        mode |= std::fstream::trunc;
        break;
      default:
        return false;
        break;  // error al introducir los parametros de apertura
    }
  }
  file_.open(file_name_, mode);
  return file_.is_open();
}

/**
 * @brief Función para cerrar un archivo
 * @tparam InfoType
 */
template <class InfoType>
bool Middleware<InfoType>::close() {
  file_.close();
  return !file_.is_open();
}

/**
 * @brief Función para refrescar el archivo
 * @tparam InfoType
 */
template <class InfoType>
bool Middleware<InfoType>::refresh() {
  if (!close()) {
    return false;
  }
  return open();
}

/**
 * @brief Función para introducir información al fichero
 * @tparam InfoType
 * @param[in] info
 */
template <class InfoType>
bool Middleware<InfoType>::insert(InfoType info) {
  if (!is_open()) {
    return false;
  }
  file_ << info;
  return true;
}

/**
 * @brief Función para comprobar que el fichero se puede abrir
 * @tparam InfoType
 */
template <class InfoType>
bool Middleware<InfoType>::is_open() {
  return file_.is_open();
}

/**
 * @brief Función para conseguir toda la información del fichero
 * @tparam InfoType
 */
template <class InfoType>
std::string Middleware<InfoType>::GetAllInformation() {
  std::string text, result{""};
  while (getline(file_, text)) {
    result += text;
    result += "\n";
  }
  return result;
}

/**
 * @brief Función para insertar en una línea en concreto información a la base
 * de datos
 * @tparam InfoType
 * @tparam line
 * @param[in] info
 */

template <class InfoType>
bool Middleware<InfoType>::insert_in_line(int line, InfoType info) {
  refresh();
  // RECOGER VALORES PREVIOS AL BORRADO FICHERO
  std::list<std::string> FileLines;
  std::string text;
  int ActualLine{1};
  while (getline(file_, text)) {
    if (!text.empty()) {
      if (ActualLine == line) {
        FileLines.push_back(info);
      } else {
        FileLines.push_back(text);
      }
    }
    ActualLine++;
  }
  // ESCRIBIR DATOS ORDENADOS EN FICHERO
  close();
  file_.open(file_name_, std::ofstream::out | std::ofstream::trunc);
  for (auto&& i : FileLines) {
    file_ << i;
    file_ << std::endl;
  }
  return true;
}

/**
 * @brief Inserta una nueva línea en el archivo
 * @tparam InfoType
 * @param[in] info
 */
template <class InfoType>
bool Middleware<InfoType>::insert_new_line_back(InfoType info) {
  file_.close();
  file_.open(file_name_, std::fstream::in | std::fstream::out | std::fstream::app);
  file_ << info;
  return true;
}

/**
 * @brief Función que escribe una única línea
 * @tparam InfoType
 * @param[in] info
 */
template <class InfoType>
void Middleware<InfoType>::writeUniqueLine(InfoType info) {
  file_.close();
  file_.open(file_name_, std::ofstream::out | std::ofstream::trunc);
  file_ << info;
}

/**
 * @brief Elimina todas las líneas en blanco
 * @tparam InfoType
 */

template <class InfoType>
void Middleware<InfoType>::DeleteBlankLines() {
  close();
  open();
  std::string line, text;
  while (std::getline(file_, line))
    if (!line.empty()) text += line;
  close();
  open();
  file_ << text;
}

/**
 * @brief Devuelve la información contenida en una línea en concreto de la base
 * de datos
 * @tparam InfoType
 * @param[in] line
 */
template <class InfoType>
std::string Middleware<InfoType>::get_line(int line) {
  arrive_at_line(line);
  std::string text;
  getline(file_, text);
  return text;
}

/**
 * @brief Función que llega a una línea
 * @tparam InfoType
 * @param[in] line
 */
template <class InfoType>
bool Middleware<InfoType>::arrive_at_line(int line) {
  if (!refresh()) {
    return false;
  }
  int i = 1;
  std::string text;
  while (i++ < line) std::getline(file_, text);
  return true;
}

/**
 * @brief Función que busca un usuario
 * @tparam InfoType
 * @param[in] email
 */
template <class InfoType>
int Middleware<InfoType>::SearchUsr(std::string email) {
  if (!refresh()) {
    return false;
  }
  int line = 1;
  std::string text;
  while (std::getline(file_, text)) {
    std::string firstEmail;
    std::getline(std::stringstream(text), firstEmail, ':');
    if (firstEmail == email) {
      return line;
    }
    line++;
  }
  return -1;  // Cuando no existe ese usuario en la base de datos
}

#endif
