// Archivo: main.cpp
// Punto de entrada del programa: crea el menú, lo inicia y lo libera al salir.

#include "Menu.h"

int main()
{
   Menu* menu = new Menu();   // el menú crea el sistema de asistencia
   menu->iniciar();           // se queda en el menú hasta que el usuario elige salir
   delete menu;               // libera el menú y, en cascada, todo el sistema
   return 0;
}
