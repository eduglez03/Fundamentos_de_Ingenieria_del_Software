#include <iostream>
#include <fstream>

#include "../src/models/Interfaz.h"

using namespace std;
int main() {
  Middleware<string> BaseDatos("BaseDatosTest.DB");
  Cerradura cerradura("casatrasera", false);
  cerradura.Abrir(&BaseDatos);
  if (BaseDatos.search_substring("casatrasera[OPEN]") != -1) {
    cout << "\033[32mLa cerradura se ha abierto correctamente\033[0m" << endl;
  } else {
    cout << "\033[31mLa cerradura no se ha abierto correctamente\033[0m" << endl;
  }
  return 0;
}