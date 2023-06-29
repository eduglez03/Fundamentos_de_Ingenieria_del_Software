#include <iostream>
#include <fstream>

#include "../src/models/Interfaz.h"

int main() {
  Middleware<std::string> BaseDatos("BaseDatos.DB");
  Cerradura cerradura("casatrasera", true);
  cerradura.Cerrar(&BaseDatos);
  if (BaseDatos.search_substring("casatrasera[CLOSE]") != -1) {
    std::cout << "\033[32mLa cerradura se ha cerrado correctamente\033[0m" << std::endl;
  } else {
    std::cout << "\033[31mLa cerradura no se ha cerrado correctamente\033[0m" << std::endl;
  }
  return 0;
}