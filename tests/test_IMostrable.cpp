// Archivo: test_IMostrable.cpp
// Pruebas de unidad de la interfaz IMostrable.
// Como la interfaz no tiene lógica, se verifica que una clase concreta pueda
// implementarla y que el polimorfismo y el destructor virtual funcionen.

#include "doctest.h"
#include "IMostrable.h"
#include <iostream>
#include <sstream>
#include <string>

// Clase de prueba que implementa IMostrable.
// Imprime un texto fijo y, al destruirse, marca una bandera externa
// para poder comprobar que su destructor se ejecutó.
class MostrableDePrueba : public IMostrable
{
public:
   // Recibe el texto que imprimirá y la dirección de la bandera de destrucción.
   MostrableDePrueba(std::string texto, bool* bandera) : texto(texto), destruido(bandera) {}

   // Al destruirse marca la bandera (si se entregó una) como verdadera.
   ~MostrableDePrueba() override
   {
      if (destruido != nullptr)
      {
         *destruido = true;
      }
   }

   // Imprime el texto guardado.
   void mostrar(void) const override
   {
      std::cout << texto;
   }

private:
   std::string texto;   // texto que se imprime en mostrar()
   bool* destruido;     // bandera externa que indica si el destructor se ejecutó (no es dueña)
};

// Clase auxiliar de prueba que redirige std::cout hacia un buffer en memoria
// mientras existe, para poder leer lo que se imprimió sin depender de la consola.
class CapturaSalidaMostrable
{
public:
   // Guarda el buffer original de std::cout y lo reemplaza por el buffer interno.
   CapturaSalidaMostrable() : original(std::cout.rdbuf(buffer.rdbuf())) {}

   // Devuelve std::cout a su buffer original.
   ~CapturaSalidaMostrable()
   {
      std::cout.rdbuf(original);
   }

   // Devuelve todo lo que se imprimió mientras la captura estuvo activa.
   std::string texto(void) const
   {
      return buffer.str();
   }

private:
   std::ostringstream buffer;   // destino temporal de la salida
   std::streambuf* original;    // buffer original de std::cout (no es dueño)
};

// Verifica que mostrar(), llamado desde un puntero a la interfaz,
// ejecute la versión de la clase concreta (polimorfismo).
TEST_CASE("IMostrable_mostrar_implementacion")
{
   IMostrable* objeto = new MostrableDePrueba("hola", nullptr);
   std::string impreso;
   {
      CapturaSalidaMostrable captura;
      objeto->mostrar();
      impreso = captura.texto();
   }
   // Se espera "hola" porque es el texto que imprime la clase concreta
   CHECK(impreso == "hola");
   delete objeto;
}

// Verifica que mostrar() con un texto vacío no imprima nada (caso vacío).
TEST_CASE("IMostrable_mostrar_textoVacio")
{
   IMostrable* objeto = new MostrableDePrueba("", nullptr);
   std::string impreso;
   {
      CapturaSalidaMostrable captura;
      objeto->mostrar();
      impreso = captura.texto();
   }
   // La clase concreta no tiene nada que imprimir, así que la salida queda vacía
   CHECK(impreso.empty());
   delete objeto;
}

// Verifica que delete sobre un IMostrable* ejecute el destructor de la clase concreta.
// Si el destructor de la interfaz no fuera virtual, la bandera quedaría en false.
TEST_CASE("IMostrable_destructor_virtual")
{
   bool destruido = false;
   IMostrable* objeto = new MostrableDePrueba("x", &destruido);
   delete objeto;
   CHECK(destruido == true);
}
