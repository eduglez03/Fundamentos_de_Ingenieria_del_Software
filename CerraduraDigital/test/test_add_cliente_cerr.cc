#include <iostream>
#include <fstream>
#include <map>
#include "../src/models/UsuarioSinRegistrar.h"
#include "../src/models/Propietario.h"

int main() {
  Middleware<std::string> BaseDatos("BaseDatos.DB");
  std::map<std::string, Cerradura> Cerraduras;
  Cerraduras.insert(std::pair<std::string, Cerradura>("casatrasera", Cerradura("casatrasera", false)));
  Propietario<std::string> propietario("admin@gmail.com", "admin", &BaseDatos, Cerraduras);
  propietario.AddClienteAcerradura();
  if (BaseDatos.search_substring("casatrasera[CLOSE]") != -1) {
    std::cout << "\033[32mSe ha dado permisos correctamente\033[0m" << std::endl;
  } else {
    std::cout << "\033[31mNo se ha dado permisos correctamente\033[0m" << std::endl;
  }
  return 0;
}