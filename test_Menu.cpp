// Archivo: test_Menu.cpp
// Pruebas de unidad de la clase Menu. Solo se prueba la parte sin interacción
// (construcción y destrucción); los menús, la lectura de datos y el uso de la fecha
// y hora actuales se verifican manualmente ejecutando el programa.

#include "doctest.h"
#include "Menu.h"

// Verifica que crear y destruir un Menu no falle: el constructor crea el SistemaAsistencia
// y el destructor lo libera (sin leer del teclado).
TEST_CASE("Menu_constructorDestructor_sinFugas")
{
   Menu* menu = nullptr;
   CHECK_NOTHROW(menu = new Menu());
   CHECK(menu != nullptr);
   CHECK_NOTHROW(delete menu);
}

// Verifica que se puedan crear y destruir varios menús seguidos: cada uno tiene su propio
// sistema, así que no comparten memoria ni hay doble delete.
TEST_CASE("Menu_constructorDestructor_variasInstancias")
{
   Menu* primero = new Menu();
   Menu* segundo = new Menu();
   CHECK(primero != segundo);
   CHECK_NOTHROW(delete primero);
   CHECK_NOTHROW(delete segundo);
}
