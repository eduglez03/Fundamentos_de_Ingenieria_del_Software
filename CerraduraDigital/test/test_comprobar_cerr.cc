#include <iostream>
#include <fstream>

#include "../src/models/Interfaz.h"

using namespace std;
int main() {
  Middleware<string> BaseDatos("BaseDatos.DB");
  Cerradura cerradura("casatrasera", true);
  if (cerradura.GetState() == true) {
    cout << "\033[32mLa cerradura está abierta\033[0m" << endl;
  } else {
    cout << "\033[31mLa cerradura está cerrada\033[0m" << endl;
  }
  return 0;
}