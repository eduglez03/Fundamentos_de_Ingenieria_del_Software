#include <iostream>
#include <fstream>

#include "../src/models/UsuarioSinRegistrar.h"

int main() {
  Middleware<std::string> BaseDatos("BaseDatos.DB");
  UsuarioSinRegistrar<std::string> usu(&BaseDatos);
  UsuarioRegistrado<std::string>* usuario = usu.Registrarse();
  if (usuario != nullptr) {
    std::cout << "\033[32mSe ha registrado el usuario correctamente\033[0m" << std::endl;
  } else {
    std::cout << "\033[31mNo se ha registrado el usuario correctamente\033[0m" << std::endl;
  }
  return 0;
}