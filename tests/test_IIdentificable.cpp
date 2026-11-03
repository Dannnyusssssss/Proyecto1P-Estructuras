// Archivo: test_IIdentificable.cpp
// Pruebas de unidad de la interfaz IIdentificable.
// Como la interfaz no tiene lógica, se verifica que una clase concreta pueda
// implementarla y que el polimorfismo y el destructor virtual funcionen.

#include "doctest.h"
#include "IIdentificable.h"
#include <string>

// Clase de prueba que implementa IIdentificable.
// Devuelve la clave que recibe en el constructor y, al destruirse,
// marca una bandera externa para comprobar que su destructor se ejecutó.
class IdentificableDePrueba : public IIdentificable
{
public:
   // Recibe la clave que devolverá y la dirección de la bandera de destrucción.
   IdentificableDePrueba(std::string clave, bool* bandera) : clave(clave), destruido(bandera) {}

   // Al destruirse marca la bandera (si se entregó una) como verdadera.
   ~IdentificableDePrueba() override
   {
      if (destruido != nullptr)
      {
         *destruido = true;
      }
   }

   // Devuelve la clave guardada.
   std::string getClave(void) const override
   {
      return clave;
   }

private:
   std::string clave;   // clave que identifica al objeto
   bool* destruido;     // bandera externa que indica si el destructor se ejecutó (no es dueña)
};

// Verifica que getClave(), llamado desde un puntero a la interfaz,
// devuelva la clave de la clase concreta.
TEST_CASE("IIdentificable_getClave_implementacion")
{
   IIdentificable* objeto = new IdentificableDePrueba("1710034065", nullptr);
   // Se espera la misma clave que se entregó al constructor
   CHECK(objeto->getClave() == "1710034065");
   delete objeto;
}

// Verifica que una clave vacía se devuelva tal cual (caso vacío):
// la interfaz no impone formato, eso lo valida cada clase o el Validador.
TEST_CASE("IIdentificable_getClave_vacia")
{
   IIdentificable* objeto = new IdentificableDePrueba("", nullptr);
   CHECK(objeto->getClave().empty());
   delete objeto;
}

// Verifica que delete sobre un IIdentificable* ejecute el destructor de la clase concreta.
// Si el destructor de la interfaz no fuera virtual, la bandera quedaría en false.
TEST_CASE("IIdentificable_destructor_virtual")
{
   bool destruido = false;
   IIdentificable* objeto = new IdentificableDePrueba("clave", &destruido);
   delete objeto;
   CHECK(destruido == true);
}
