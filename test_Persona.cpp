// Archivo: test_Persona.cpp
// Pruebas de unidad de la clase Persona: constructores, getters, setters,
// clave, salida de mostrar() y uso a través de las interfaces.

#include "doctest.h"
#include "Persona.h"
#include <iostream>
#include <sstream>
#include <string>

// Clase auxiliar de prueba que redirige std::cout a un buffer mientras existe.
class CapturaSalidaPersona
{
public:
   // Guarda el buffer original de std::cout y lo reemplaza por el interno.
   CapturaSalidaPersona() : original(std::cout.rdbuf(buffer.rdbuf())) {}

   // Devuelve std::cout a su buffer original.
   ~CapturaSalidaPersona()
   {
      std::cout.rdbuf(original);
   }

   // Devuelve lo que se imprimió mientras la captura estuvo activa.
   std::string texto(void) const
   {
      return buffer.str();
   }

private:
   std::ostringstream buffer;   // destino temporal de la salida
   std::streambuf* original;    // buffer original de std::cout (no es dueño)
};

// ---------------------------------------------------------------------
// Constructores
// ---------------------------------------------------------------------

// Verifica que el constructor por defecto deje los tres datos vacíos.
TEST_CASE("Persona_constructorDefecto_vacio")
{
   Persona* p = new Persona();
   CHECK(p->getCedula().empty());
   CHECK(p->getNombre().empty());
   CHECK(p->getApellido().empty());
   CHECK(p->getClave().empty());
   delete p;
}

// Verifica que el constructor con parámetros guarde cada dato en su atributo.
TEST_CASE("Persona_constructorParametros_valido")
{
   Persona* p = new Persona("1710034065", "Juan", "Pérez");
   CHECK(p->getCedula() == "1710034065");
   CHECK(p->getNombre() == "Juan");
   CHECK(p->getApellido() == "Pérez");
   delete p;
}

// Verifica que el constructor guarde los datos tal cual, aunque sean inválidos:
// Persona no valida, eso le corresponde a SistemaAsistencia con el Validador.
TEST_CASE("Persona_constructorParametros_sinValidacion")
{
   Persona* p = new Persona("123", "Juan3", "");
   CHECK(p->getCedula() == "123");
   CHECK(p->getNombre() == "Juan3");
   CHECK(p->getApellido().empty());
   delete p;
}

// ---------------------------------------------------------------------
// Getters y setters
// ---------------------------------------------------------------------

// Verifica que setCedula reemplace la cédula.
TEST_CASE("Persona_setCedula_valido")
{
   Persona* p = new Persona("1710034065", "Juan", "Pérez");
   p->setCedula("0926687856");
   CHECK(p->getCedula() == "0926687856");
   delete p;
}

// Verifica que setCedula acepte el texto vacío (caso vacío).
TEST_CASE("Persona_setCedula_vacio")
{
   Persona* p = new Persona("1710034065", "Juan", "Pérez");
   p->setCedula("");
   CHECK(p->getCedula().empty());
   delete p;
}

// Verifica que setNombre reemplace el nombre, incluido uno con tildes y espacios.
TEST_CASE("Persona_setNombre_valido")
{
   Persona* p = new Persona("1710034065", "Juan", "Pérez");
   p->setNombre("María José");
   CHECK(p->getNombre() == "María José");
   delete p;
}

// Verifica que setNombre acepte el texto vacío.
TEST_CASE("Persona_setNombre_vacio")
{
   Persona* p = new Persona("1710034065", "Juan", "Pérez");
   p->setNombre("");
   CHECK(p->getNombre().empty());
   delete p;
}

// Verifica que setApellido reemplace el apellido.
TEST_CASE("Persona_setApellido_valido")
{
   Persona* p = new Persona("1710034065", "Juan", "Pérez");
   p->setApellido("Ñañez");
   CHECK(p->getApellido() == "Ñañez");
   delete p;
}

// Verifica que setApellido acepte el texto vacío.
TEST_CASE("Persona_setApellido_vacio")
{
   Persona* p = new Persona("1710034065", "Juan", "Pérez");
   p->setApellido("");
   CHECK(p->getApellido().empty());
   delete p;
}

// Verifica que cambiar un dato no altere los otros dos.
TEST_CASE("Persona_setters_independientes")
{
   Persona* p = new Persona("1710034065", "Juan", "Pérez");
   p->setNombre("Ana");
   CHECK(p->getCedula() == "1710034065");
   CHECK(p->getApellido() == "Pérez");
   delete p;
}

// ---------------------------------------------------------------------
// getClave
// ---------------------------------------------------------------------

// Verifica que la clave sea la cédula (sección 4, tabla de claves).
TEST_CASE("Persona_getClave_esCedula")
{
   Persona* p = new Persona("1710034065", "Juan", "Pérez");
   CHECK(p->getClave() == "1710034065");
   CHECK(p->getClave() == p->getCedula());
   delete p;
}

// Verifica que la clave cambie cuando cambia la cédula, porque se calcula a partir de ella.
TEST_CASE("Persona_getClave_trasSetCedula")
{
   Persona* p = new Persona("1710034065", "Juan", "Pérez");
   p->setCedula("0926687856");
   CHECK(p->getClave() == "0926687856");
   delete p;
}

// ---------------------------------------------------------------------
// mostrar
// ---------------------------------------------------------------------

// Verifica el formato exacto acordado para mostrar().
TEST_CASE("Persona_mostrar_formato")
{
   Persona* p = new Persona("1710034065", "Juan", "Pérez");
   std::string impreso;
   {
      CapturaSalidaPersona captura;
      p->mostrar();
      impreso = captura.texto();
   }
   CHECK(impreso == "Cédula: 1710034065 | Nombre: Juan Pérez\n");
   delete p;
}

// Verifica que mostrar() con datos vacíos imprima el formato sin valores y sin fallar.
TEST_CASE("Persona_mostrar_vacia")
{
   Persona* p = new Persona();
   std::string impreso;
   {
      CapturaSalidaPersona captura;
      p->mostrar();
      impreso = captura.texto();
   }
   CHECK(impreso == "Cédula:  | Nombre:  \n");
   delete p;
}

// ---------------------------------------------------------------------
// Uso a través de las interfaces
// ---------------------------------------------------------------------

// Verifica que getClave y mostrar funcionen desde punteros a las interfaces,
// que es como los usa la ListaDoble.
TEST_CASE("Persona_interfaces_polimorfismo")
{
   Persona* p = new Persona("1710034065", "Juan", "Pérez");
   IIdentificable* identificable = p;
   IMostrable* mostrable = p;
   CHECK(identificable->getClave() == "1710034065");
   std::string impreso;
   {
      CapturaSalidaPersona captura;
      mostrable->mostrar();
      impreso = captura.texto();
   }
   CHECK(impreso == "Cédula: 1710034065 | Nombre: Juan Pérez\n");
   delete p;
}
