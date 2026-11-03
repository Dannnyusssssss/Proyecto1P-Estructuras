// Archivo: test_Profesor.cpp
// Pruebas de unidad de la clase Profesor: constructores, validación de franjas en
// agregarFranja, búsqueda de la franja de entrada y de salida, mostrar() y destructor.
// Regla de memoria que siguen las pruebas (decisión 4): si agregarFranja devuelve false,
// la prueba sigue siendo dueña de la franja y la libera.

#include "doctest.h"
#include "Profesor.h"
#include "FranjaHoraria.h"
#include "Hora.h"
#include <iostream>
#include <sstream>
#include <string>

// Clase auxiliar de prueba con métodos static para crear franjas y consultar al profesor
// sin repetir la gestión de memoria en cada prueba.
class AyudaProfesor
{
public:
   // Cédula real usada como profesor de las pruebas.
   static std::string cedula(void)
   {
      return "1710034065";
   }

   // Crea una franja "HH:MM"-"HH:MM" con la cédula y el aula indicadas.
   static FranjaHoraria* franja(std::string inicio, std::string fin,
                                std::string cedulaProfesor = "1710034065", std::string aula = "A-101")
   {
      return new FranjaHoraria(new Hora(inicio), new Hora(fin), aula, cedulaProfesor);
   }

   // Intenta agregar la franja. Si el profesor la rechaza, la libera aquí (la prueba era su dueña).
   static bool agregar(Profesor* profesor, FranjaHoraria* franja)
   {
      bool aceptada = profesor->agregarFranja(franja);
      if (!aceptada)
      {
         delete franja;
      }
      return aceptada;
   }

   // Devuelve la clave de la franja de entrada a la hora "HH:MM", o "" si no hay ninguna.
   static std::string claveEntrada(const Profesor* profesor, std::string hhmm)
   {
      Hora* hora = new Hora(hhmm);
      FranjaHoraria* encontrada = profesor->buscarFranjaParaEntrada(hora);
      delete hora;   // el profesor solo consulta la hora
      return (encontrada != nullptr) ? encontrada->getClave() : "";
   }

   // Devuelve la clave de la franja de salida a la hora "HH:MM", o "" si no hay ninguna.
   static std::string claveSalida(const Profesor* profesor, std::string hhmm)
   {
      Hora* hora = new Hora(hhmm);
      FranjaHoraria* encontrada = profesor->buscarFranjaParaSalida(hora);
      delete hora;   // el profesor solo consulta la hora
      return (encontrada != nullptr) ? encontrada->getClave() : "";
   }
};

// Clase auxiliar de prueba que redirige std::cout a un buffer mientras existe.
class CapturaSalidaProfesor
{
public:
   // Guarda el buffer original de std::cout y lo reemplaza por el interno.
   CapturaSalidaProfesor() : original(std::cout.rdbuf(buffer.rdbuf())) {}

   // Devuelve std::cout a su buffer original.
   ~CapturaSalidaProfesor()
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
// Constructores y getFranjas
// ---------------------------------------------------------------------

// Verifica que el constructor por defecto cree la lista de franjas vacía (no nullptr).
TEST_CASE("Profesor_constructorDefecto_listaVacia")
{
   Profesor* p = new Profesor();
   REQUIRE(p->getFranjas() != nullptr);
   CHECK(p->getFranjas()->estaVacia() == true);
   CHECK(p->getCedula().empty());
   delete p;
}

// Verifica que el constructor con parámetros guarde los datos de Persona
// y cree la lista de franjas vacía.
TEST_CASE("Profesor_constructorParametros_valido")
{
   Profesor* p = new Profesor("1710034065", "Juan", "Pérez");
   CHECK(p->getCedula() == "1710034065");
   CHECK(p->getNombre() == "Juan");
   CHECK(p->getApellido() == "Pérez");
   CHECK(p->getClave() == "1710034065");   // heredado de Persona
   REQUIRE(p->getFranjas() != nullptr);
   CHECK(p->getFranjas()->getTamanio() == 0);
   delete p;
}

// Verifica que getFranjas refleje las franjas agregadas.
TEST_CASE("Profesor_getFranjas_trasAgregar")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "09:00"));
   CHECK(p->getFranjas()->getTamanio() == 1);
   CHECK(p->getFranjas()->buscar("1710034065-07:00") != nullptr);
   delete p;
}

// ---------------------------------------------------------------------
// agregarFranja: franjas predefinidas (aceptadas)
// ---------------------------------------------------------------------

// Verifica que se acepte la primera franja del día, 07:00-09:00.
TEST_CASE("Profesor_agregarFranja_primeraFranja")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "09:00")) == true);
   CHECK(p->getFranjas()->getTamanio() == 1);
   delete p;
}

// Verifica que se acepte la última franja del día, 19:00-21:00.
TEST_CASE("Profesor_agregarFranja_ultimaFranja")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("19:00", "21:00")) == true);
   delete p;
}

// Verifica que un profesor pueda tener las 7 franjas a la vez: son consecutivas, no se cruzan.
TEST_CASE("Profesor_agregarFranja_lasSieteFranjas")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "09:00")) == true);
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("09:00", "11:00")) == true);
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("11:00", "13:00")) == true);
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("13:00", "15:00")) == true);
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("15:00", "17:00")) == true);
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("17:00", "19:00")) == true);
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("19:00", "21:00")) == true);
   CHECK(p->getFranjas()->getTamanio() == 7);
   delete p;   // libera las 7 franjas y sus horas
}

// ---------------------------------------------------------------------
// agregarFranja: franjas no predefinidas (rechazadas)
// ---------------------------------------------------------------------

// Verifica que se rechace 07:30-08:15: los minutos no son 00.
TEST_CASE("Profesor_agregarFranja_minutosNoCero")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("07:30", "08:15")) == false);
   CHECK(p->getFranjas()->estaVacia() == true);
   delete p;
}

// Verifica que se rechace 07:00-08:00: no dura 2 horas.
TEST_CASE("Profesor_agregarFranja_noDuraDosHoras")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "08:00")) == false);
   delete p;
}

// Verifica que se rechace 08:00-10:00: dura 2 horas pero no empieza en un bloque.
TEST_CASE("Profesor_agregarFranja_noEmpiezaEnBloque")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("08:00", "10:00")) == false);
   delete p;
}

// Verifica que se rechace 05:00-07:00: empieza antes de las 07:00.
TEST_CASE("Profesor_agregarFranja_antesDeLasSiete")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("05:00", "07:00")) == false);
   delete p;
}

// Verifica que se rechace 21:00-23:00: pasa de las 21:00.
TEST_CASE("Profesor_agregarFranja_despuesDeLasVeintiuna")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("21:00", "23:00")) == false);
   delete p;
}

// Verifica que se rechace 09:00-07:00: el inicio es mayor que el final.
TEST_CASE("Profesor_agregarFranja_inicioMayorQueFinal")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("09:00", "07:00")) == false);
   delete p;
}

// ---------------------------------------------------------------------
// agregarFranja: duplicadas, otra cédula y datos nulos (rechazadas)
// ---------------------------------------------------------------------

// Verifica que se rechace agregar dos veces la misma franja.
TEST_CASE("Profesor_agregarFranja_duplicada")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "09:00")) == true);
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "09:00")) == false);
   CHECK(p->getFranjas()->getTamanio() == 1);
   delete p;
}

// Verifica que la misma franja con otra aula también sea duplicada: el aula no forma parte
// de la clave. Es el único "cruce" posible con franjas fijas, y lo rechaza la validación de
// duplicadas; la validación de cruce no se puede activar hoy y no se prueba por separado.
TEST_CASE("Profesor_agregarFranja_duplicadaOtraAula")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("09:00", "11:00", AyudaProfesor::cedula(), "A-101")) == true);
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("09:00", "11:00", AyudaProfesor::cedula(), "B-202")) == false);
   CHECK(p->getFranjas()->getTamanio() == 1);
   CHECK(p->getFranjas()->obtener(0)->getAula() == "A-101");   // se conserva la primera
   delete p;
}

// Verifica que se rechace una franja con la cédula de otro profesor: no se corrige en
// silencio, para que el error de quien llamó se note y la clave coincida con su dueño.
TEST_CASE("Profesor_agregarFranja_cedulaDeOtroProfesor")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   CHECK(AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "09:00", "0926687856")) == false);
   CHECK(p->getFranjas()->estaVacia() == true);
   delete p;
}

// Verifica que agregar nullptr devuelva false sin fallar.
TEST_CASE("Profesor_agregarFranja_nula")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   CHECK(p->agregarFranja(nullptr) == false);
   delete p;
}

// Verifica que se rechace una franja sin horas (constructor por defecto de FranjaHoraria).
TEST_CASE("Profesor_agregarFranja_sinHoras")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   FranjaHoraria* vacia = new FranjaHoraria();
   vacia->setCedulaProfesor(AyudaProfesor::cedula());
   CHECK(AyudaProfesor::agregar(p, vacia) == false);
   delete p;
}

// Verifica que, al rechazar una franja, el profesor NO la libere: la prueba todavía
// puede usarla antes de borrarla (decisión 4).
TEST_CASE("Profesor_agregarFranja_rechazadaNoSeLibera")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   FranjaHoraria* invalida = AyudaProfesor::franja("08:00", "10:00");
   CHECK(p->agregarFranja(invalida) == false);
   CHECK(invalida->getHoraInicio()->toString() == "08:00");   // sigue viva
   delete invalida;
   delete p;
}

// ---------------------------------------------------------------------
// buscarFranjaParaEntrada
// ---------------------------------------------------------------------

// Verifica que dentro de la ventana de entrada se devuelva la franja correcta.
TEST_CASE("Profesor_buscarFranjaParaEntrada_dentroDeVentana")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "09:00"));
   AyudaProfesor::agregar(p, AyudaProfesor::franja("11:00", "13:00"));
   CHECK(AyudaProfesor::claveEntrada(p, "07:00") == "1710034065-07:00");
   CHECK(AyudaProfesor::claveEntrada(p, "07:10") == "1710034065-07:00");
   CHECK(AyudaProfesor::claveEntrada(p, "11:05") == "1710034065-11:00");
   delete p;
}

// Verifica que fuera de toda ventana de entrada se devuelva nullptr (llegada tarde o sin franja).
TEST_CASE("Profesor_buscarFranjaParaEntrada_fueraDeVentana")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "09:00"));
   CHECK(AyudaProfesor::claveEntrada(p, "06:59") == "");
   CHECK(AyudaProfesor::claveEntrada(p, "07:11") == "");
   CHECK(AyudaProfesor::claveEntrada(p, "11:00") == "");   // no tiene la franja de las 11
   delete p;
}

// Verifica que con hora nullptr o sin franjas se devuelva nullptr sin fallar.
TEST_CASE("Profesor_buscarFranjaParaEntrada_nuloYSinFranjas")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   CHECK(AyudaProfesor::claveEntrada(p, "07:00") == "");   // sin franjas
   AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "09:00"));
   CHECK(p->buscarFranjaParaEntrada(nullptr) == nullptr);
   delete p;
}

// ---------------------------------------------------------------------
// buscarFranjaParaSalida
// ---------------------------------------------------------------------

// Verifica que dentro de la ventana de salida se devuelva la franja correcta.
TEST_CASE("Profesor_buscarFranjaParaSalida_dentroDeVentana")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "09:00"));
   AyudaProfesor::agregar(p, AyudaProfesor::franja("19:00", "21:00"));
   CHECK(AyudaProfesor::claveSalida(p, "09:00") == "1710034065-07:00");
   CHECK(AyudaProfesor::claveSalida(p, "09:10") == "1710034065-07:00");
   CHECK(AyudaProfesor::claveSalida(p, "21:10") == "1710034065-19:00");
   delete p;
}

// Verifica que fuera de toda ventana de salida se devuelva nullptr.
TEST_CASE("Profesor_buscarFranjaParaSalida_fueraDeVentana")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "09:00"));
   CHECK(AyudaProfesor::claveSalida(p, "08:59") == "");
   CHECK(AyudaProfesor::claveSalida(p, "09:11") == "");
   CHECK(AyudaProfesor::claveSalida(p, "07:05") == "");    // es hora de entrada, no de salida
   delete p;
}

// Verifica que con hora nullptr o sin franjas se devuelva nullptr sin fallar.
TEST_CASE("Profesor_buscarFranjaParaSalida_nuloYSinFranjas")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   CHECK(AyudaProfesor::claveSalida(p, "09:00") == "");    // sin franjas
   AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "09:00"));
   CHECK(p->buscarFranjaParaSalida(nullptr) == nullptr);
   delete p;
}

// ---------------------------------------------------------------------
// Franjas consecutivas
// ---------------------------------------------------------------------

// Verifica el caso clave de franjas consecutivas 7-9 y 9-11: a las 09:05 la SALIDA
// corresponde a la franja 7-9 (su ventana de salida es 09:00-09:10) y la ENTRADA a la
// franja 9-11 (su ventana de entrada es 09:00-09:10). Cada franja se registra por separado.
TEST_CASE("Profesor_franjasConsecutivas_a0905")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "09:00"));
   AyudaProfesor::agregar(p, AyudaProfesor::franja("09:00", "11:00"));
   CHECK(AyudaProfesor::claveSalida(p, "09:05") == "1710034065-07:00");
   CHECK(AyudaProfesor::claveEntrada(p, "09:05") == "1710034065-09:00");
   delete p;
}

// ---------------------------------------------------------------------
// mostrar
// ---------------------------------------------------------------------

// Verifica que mostrar imprima los datos de la persona y cada franja con sangría.
TEST_CASE("Profesor_mostrar_conFranjas")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "09:00", AyudaProfesor::cedula(), "A-101"));
   AyudaProfesor::agregar(p, AyudaProfesor::franja("09:00", "11:00", AyudaProfesor::cedula(), "B-202"));
   std::string impreso;
   {
      CapturaSalidaProfesor captura;
      p->mostrar();
      impreso = captura.texto();
   }
   CHECK(impreso == "Cédula: 1710034065 | Nombre: Juan Pérez\n"
                    "  07:00 - 09:00 | Aula: A-101\n"
                    "  09:00 - 11:00 | Aula: B-202\n");
   delete p;
}

// Verifica el mensaje cuando el profesor no tiene franjas.
TEST_CASE("Profesor_mostrar_sinFranjas")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   std::string impreso;
   {
      CapturaSalidaProfesor captura;
      p->mostrar();
      impreso = captura.texto();
   }
   CHECK(impreso == "Cédula: 1710034065 | Nombre: Juan Pérez\n"
                    "  Sin franjas asignadas\n");
   delete p;
}

// Verifica que mostrar() sea polimórfico: llamado desde un Persona* usa la versión de Profesor.
TEST_CASE("Profesor_mostrar_desdePersona")
{
   Persona* persona = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   std::string impreso;
   {
      CapturaSalidaProfesor captura;
      persona->mostrar();
      impreso = captura.texto();
   }
   CHECK(impreso.find("Sin franjas asignadas") != std::string::npos);
   delete persona;
}

// ---------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------

// Verifica que borrar un Profesor a través de un Persona* ejecute el destructor de Profesor
// (virtual, heredado de las interfaces) y libere su lista sin fallar.
TEST_CASE("Profesor_destructor_desdePersona")
{
   Profesor* p = new Profesor(AyudaProfesor::cedula(), "Juan", "Pérez");
   AyudaProfesor::agregar(p, AyudaProfesor::franja("07:00", "09:00"));
   AyudaProfesor::agregar(p, AyudaProfesor::franja("09:00", "11:00"));
   Persona* persona = p;
   CHECK_NOTHROW(delete persona);
}
