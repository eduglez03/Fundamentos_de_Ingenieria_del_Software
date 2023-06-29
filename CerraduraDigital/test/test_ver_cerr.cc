#include <iostream>
#include <fstream>

#include "../src/models/Interfaz.h"

using namespace std;
int main() {
   Middleware<std::string> BaseDatos("BaseDatos.DB");
  std::map<std::string, Cerradura> Cerraduras;
  Cerraduras.insert(std::pair<std::string, Cerradura>("casatrasera", Cerradura("casatrasera", false)));
  Cliente<std::string> cliente("prueba3@gmail.com", "prueba3", &BaseDatos, Cerraduras);
  cliente.CheckCerraduraState();
  BaseDatos.insert_new_line_back("prueba3@gmail.com:C:prueba3:casatrasera[CLOSE]");
  if (BaseDatos.search_substring("casatrasera[CLOSE]") != -1) {
    cout << "\033[32mEl listado de las cerraduras es el correcto\033[0m" << endl;
  } else {
    cout << "\033[31mEl listado de las cerraduras es incorrecto\033[0m" << endl;
  }
  return 0;
}