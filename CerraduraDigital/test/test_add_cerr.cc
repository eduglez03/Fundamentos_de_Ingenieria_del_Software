#include <iostream>
#include <fstream>
#include <map>
#include "../src/models/UsuarioSinRegistrar.h"
#include "../src/models/Propietario.h"


int main() {
  Middleware<std::string> BaseDatos("BaseDatos.DB");
  std::map<std::string, Cerradura> Cerraduras;
  Cerraduras.insert(std::pair<std::string, Cerradura>("casatrasera", Cerradura("casatrasera", false)));
  Propietario<std::string> propietario("admin1@gmail.com", "admin1", &BaseDatos, Cerraduras);
  propietario.AddCerradura();
  if (BaseDatos.search_substring("jardin[CLOSE]") != -1) {
    std::cout << "\033[32mSe ha dado añadido la cerradura correctamente\033[0m" << std::endl;
  } else {
    std::cout << "\033[31mNo se ha dado añadido la cerradura correctamente\033[0m" << std::endl;
  }
  return 0;
}