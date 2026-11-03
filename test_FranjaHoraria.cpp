// Archivo: test_FranjaHoraria.cpp
// Pruebas de unidad de la clase FranjaHoraria: ventanas de entrada y salida de 10 minutos
// en varias franjas, constructores, getters, setters (con liberación de memoria), clave y mostrar().

#include "doctest.h"
#include "FranjaHoraria.h"
#include "Hora.h"
#include <iostream>
#include <sstream>
#include <string>

// Clase auxiliar de prueba con métodos static que crean la hora a consultar,
// preguntan a la franja y liberan la hora, para que cada CHECK quede en una línea.
class ConsultaFranja
{
public:
   // Devuelve si la franja acepta la entrada a la hora "HH:MM" indicada.
   static bool entrada(const FranjaHoraria* franja, std::string hhmm)
   {
      Hora* hora = new Hora(hhmm);
      bool resultado = franja->puedeRegistrarEntrada(hora);
      delete hora;   // la franja solo consulta la hora, la prueba es su dueña
      return resultado;
   }

   // Devuelve si la franja acepta la salida a la hora "HH:MM" indicada.
   static bool salida(const FranjaHoraria* franja, std::string hhmm)
   {
      Hora* hora = new Hora(hhmm);
      bool resultado = franja->puedeRegistrarSalida(hora);
      delete hora;   // la franja solo consulta la hora, la prueba es su dueña
      return resultado;
   }

   // Crea una franja con inicio y fin en "HH:MM"; la franja se queda con las dos horas.
   static FranjaHoraria* crear(std::string inicio, std::string fin)
   {
      return new FranjaHoraria(new Hora(inicio), new Hora(fin), "A-101", "1710034065");
   }
};

// Clase auxiliar de prueba que redirige std::cout a un buffer mientras existe.
class CapturaSalidaFranja
{
public:
   // Guarda el buffer original de std::cout y lo reemplaza por el interno.
   CapturaSalidaFranja() : original(std::cout.rdbuf(buffer.rdbuf())) {}

   // Devuelve std::cout a su buffer original.
   ~CapturaSalidaFranja()
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
// puedeRegistrarEntrada
// ---------------------------------------------------------------------

// Verifica la ventana de entrada de la franja 07:00-09:00 (07:00 a 07:10, extremos incluidos):
// 06:59 es antes del inicio y 07:11 pasa de los 10 minutos.
TEST_CASE("FranjaHoraria_puedeRegistrarEntrada_franja07a09")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   CHECK(ConsultaFranja::entrada(franja, "06:59") == false);
   CHECK(ConsultaFranja::entrada(franja, "07:00") == true);
   CHECK(ConsultaFranja::entrada(franja, "07:05") == true);
   CHECK(ConsultaFranja::entrada(franja, "07:10") == true);
   CHECK(ConsultaFranja::entrada(franja, "07:11") == false);
   delete franja;
}

// Verifica que la hora de salida no sirva como hora de entrada (09:00 en la franja 07-09).
TEST_CASE("FranjaHoraria_puedeRegistrarEntrada_horaDeSalida")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   CHECK(ConsultaFranja::entrada(franja, "09:00") == false);
   delete franja;
}

// Verifica los límites de entrada en la franja 09:00-11:00.
TEST_CASE("FranjaHoraria_puedeRegistrarEntrada_franja09a11")
{
   FranjaHoraria* franja = ConsultaFranja::crear("09:00", "11:00");
   CHECK(ConsultaFranja::entrada(franja, "08:59") == false);
   CHECK(ConsultaFranja::entrada(franja, "09:00") == true);
   CHECK(ConsultaFranja::entrada(franja, "09:10") == true);
   CHECK(ConsultaFranja::entrada(franja, "09:11") == false);
   delete franja;
}

// Verifica los límites de entrada en la franja 11:00-13:00.
TEST_CASE("FranjaHoraria_puedeRegistrarEntrada_franja11a13")
{
   FranjaHoraria* franja = ConsultaFranja::crear("11:00", "13:00");
   CHECK(ConsultaFranja::entrada(franja, "10:59") == false);
   CHECK(ConsultaFranja::entrada(franja, "11:00") == true);
   CHECK(ConsultaFranja::entrada(franja, "11:10") == true);
   CHECK(ConsultaFranja::entrada(franja, "11:11") == false);
   delete franja;
}

// Verifica los límites de entrada en la última franja del día, 19:00-21:00.
TEST_CASE("FranjaHoraria_puedeRegistrarEntrada_franja19a21")
{
   FranjaHoraria* franja = ConsultaFranja::crear("19:00", "21:00");
   CHECK(ConsultaFranja::entrada(franja, "18:59") == false);
   CHECK(ConsultaFranja::entrada(franja, "19:00") == true);
   CHECK(ConsultaFranja::entrada(franja, "19:10") == true);
   CHECK(ConsultaFranja::entrada(franja, "19:11") == false);
   delete franja;
}

// Verifica que consultar con nullptr devuelva false sin fallar.
TEST_CASE("FranjaHoraria_puedeRegistrarEntrada_horaNula")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   CHECK(franja->puedeRegistrarEntrada(nullptr) == false);
   delete franja;
}

// Verifica que una franja sin hora de inicio (constructor por defecto) rechace la entrada.
TEST_CASE("FranjaHoraria_puedeRegistrarEntrada_franjaVacia")
{
   FranjaHoraria* franja = new FranjaHoraria();
   Hora* hora = new Hora(7, 0);
   CHECK(franja->puedeRegistrarEntrada(hora) == false);
   delete hora;
   delete franja;
}

// Verifica que la consulta no modifique ni libere la hora recibida.
TEST_CASE("FranjaHoraria_puedeRegistrarEntrada_noModificaHora")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   Hora* hora = new Hora(7, 5);
   franja->puedeRegistrarEntrada(hora);
   CHECK(hora->toString() == "07:05");   // sigue existiendo y con el mismo valor
   delete hora;
   delete franja;
}

// ---------------------------------------------------------------------
// puedeRegistrarSalida
// ---------------------------------------------------------------------

// Verifica la ventana de salida de la franja 07:00-09:00 (09:00 a 09:10, extremos incluidos):
// 08:59 es antes de que termine la clase y 09:11 pasa de los 10 minutos.
TEST_CASE("FranjaHoraria_puedeRegistrarSalida_franja07a09")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   CHECK(ConsultaFranja::salida(franja, "08:59") == false);
   CHECK(ConsultaFranja::salida(franja, "09:00") == true);
   CHECK(ConsultaFranja::salida(franja, "09:05") == true);
   CHECK(ConsultaFranja::salida(franja, "09:10") == true);
   CHECK(ConsultaFranja::salida(franja, "09:11") == false);
   delete franja;
}

// Verifica que la hora de entrada no sirva como hora de salida (07:00 en la franja 07-09).
TEST_CASE("FranjaHoraria_puedeRegistrarSalida_horaDeEntrada")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   CHECK(ConsultaFranja::salida(franja, "07:00") == false);
   delete franja;
}

// Verifica los límites de salida en la franja 09:00-11:00.
TEST_CASE("FranjaHoraria_puedeRegistrarSalida_franja09a11")
{
   FranjaHoraria* franja = ConsultaFranja::crear("09:00", "11:00");
   CHECK(ConsultaFranja::salida(franja, "10:59") == false);
   CHECK(ConsultaFranja::salida(franja, "11:00") == true);
   CHECK(ConsultaFranja::salida(franja, "11:10") == true);
   CHECK(ConsultaFranja::salida(franja, "11:11") == false);
   delete franja;
}

// Verifica los límites de salida en la franja 11:00-13:00.
TEST_CASE("FranjaHoraria_puedeRegistrarSalida_franja11a13")
{
   FranjaHoraria* franja = ConsultaFranja::crear("11:00", "13:00");
   CHECK(ConsultaFranja::salida(franja, "12:59") == false);
   CHECK(ConsultaFranja::salida(franja, "13:00") == true);
   CHECK(ConsultaFranja::salida(franja, "13:10") == true);
   CHECK(ConsultaFranja::salida(franja, "13:11") == false);
   delete franja;
}

// Verifica los límites de salida en la última franja del día, 19:00-21:00.
TEST_CASE("FranjaHoraria_puedeRegistrarSalida_franja19a21")
{
   FranjaHoraria* franja = ConsultaFranja::crear("19:00", "21:00");
   CHECK(ConsultaFranja::salida(franja, "20:59") == false);
   CHECK(ConsultaFranja::salida(franja, "21:00") == true);
   CHECK(ConsultaFranja::salida(franja, "21:10") == true);
   CHECK(ConsultaFranja::salida(franja, "21:11") == false);
   delete franja;
}

// Verifica que consultar con nullptr devuelva false sin fallar.
TEST_CASE("FranjaHoraria_puedeRegistrarSalida_horaNula")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   CHECK(franja->puedeRegistrarSalida(nullptr) == false);
   delete franja;
}

// Verifica que una franja sin hora de fin (constructor por defecto) rechace la salida.
TEST_CASE("FranjaHoraria_puedeRegistrarSalida_franjaVacia")
{
   FranjaHoraria* franja = new FranjaHoraria();
   Hora* hora = new Hora(9, 0);
   CHECK(franja->puedeRegistrarSalida(hora) == false);
   delete hora;
   delete franja;
}

// ---------------------------------------------------------------------
// Constructores y getters
// ---------------------------------------------------------------------

// Verifica que el constructor por defecto deje las horas en nullptr y los textos vacíos.
TEST_CASE("FranjaHoraria_constructorDefecto_vacia")
{
   FranjaHoraria* franja = new FranjaHoraria();
   CHECK(franja->getHoraInicio() == nullptr);
   CHECK(franja->getHoraFinal() == nullptr);
   CHECK(franja->getAula().empty());
   CHECK(franja->getCedulaProfesor().empty());
   delete franja;   // no debe fallar: delete de nullptr no hace nada
}

// Verifica que el constructor guarde los MISMOS punteros recibidos (no copias)
// y los demás datos. La prueba no libera las horas: ahora son de la franja.
TEST_CASE("FranjaHoraria_constructorParametros_valido")
{
   Hora* inicio = new Hora(7, 0);
   Hora* fin = new Hora(9, 0);
   FranjaHoraria* franja = new FranjaHoraria(inicio, fin, "Lab3", "0926687856");
   CHECK(franja->getHoraInicio() == inicio);
   CHECK(franja->getHoraFinal() == fin);
   CHECK(franja->getAula() == "Lab3");
   CHECK(franja->getCedulaProfesor() == "0926687856");
   delete franja;   // libera inicio y fin
}

// ---------------------------------------------------------------------
// Setters de Hora*
// ---------------------------------------------------------------------

// Verifica que setHoraInicio guarde la nueva hora. La anterior la libera la franja
// (si no, la prueba tendría que borrarla y no lo hace).
TEST_CASE("FranjaHoraria_setHoraInicio_reemplaza")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   Hora* nueva = new Hora(9, 0);
   franja->setHoraInicio(nueva);
   CHECK(franja->getHoraInicio() == nueva);
   CHECK(franja->getHoraInicio()->toString() == "09:00");
   delete franja;   // libera la nueva hora; la anterior ya se liberó en el setter
}

// Verifica que pasar el mismo puntero no lo libere: la hora sigue siendo usable.
TEST_CASE("FranjaHoraria_setHoraInicio_mismoPuntero")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   franja->setHoraInicio(franja->getHoraInicio());
   CHECK(franja->getHoraInicio()->toString() == "07:00");
   delete franja;
}

// Verifica que setHoraInicio(nullptr) libere la anterior y deje la franja sin inicio.
TEST_CASE("FranjaHoraria_setHoraInicio_nulo")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   franja->setHoraInicio(nullptr);
   CHECK(franja->getHoraInicio() == nullptr);
   delete franja;
}

// Verifica que setHoraFinal guarde la nueva hora liberando la anterior.
TEST_CASE("FranjaHoraria_setHoraFinal_reemplaza")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   Hora* nueva = new Hora(11, 0);
   franja->setHoraFinal(nueva);
   CHECK(franja->getHoraFinal() == nueva);
   CHECK(franja->getHoraFinal()->toString() == "11:00");
   delete franja;
}

// Verifica que pasar el mismo puntero no lo libere: la hora sigue siendo usable.
TEST_CASE("FranjaHoraria_setHoraFinal_mismoPuntero")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   franja->setHoraFinal(franja->getHoraFinal());
   CHECK(franja->getHoraFinal()->toString() == "09:00");
   delete franja;
}

// Verifica que setHoraFinal(nullptr) libere la anterior y deje la franja sin fin.
TEST_CASE("FranjaHoraria_setHoraFinal_nulo")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   franja->setHoraFinal(nullptr);
   CHECK(franja->getHoraFinal() == nullptr);
   delete franja;
}

// Verifica que al cambiar la hora de inicio cambie también la ventana de entrada.
TEST_CASE("FranjaHoraria_setHoraInicio_cambiaVentana")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   franja->setHoraInicio(new Hora(9, 0));
   CHECK(ConsultaFranja::entrada(franja, "07:05") == false);
   CHECK(ConsultaFranja::entrada(franja, "09:05") == true);
   delete franja;
}

// ---------------------------------------------------------------------
// Setters de texto
// ---------------------------------------------------------------------

// Verifica que setAula reemplace el aula, también por un texto vacío.
TEST_CASE("FranjaHoraria_setAula_validoYVacio")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   franja->setAula("B-202");
   CHECK(franja->getAula() == "B-202");
   franja->setAula("");
   CHECK(franja->getAula().empty());
   delete franja;
}

// Verifica que setCedulaProfesor reemplace la cédula, también por un texto vacío.
TEST_CASE("FranjaHoraria_setCedulaProfesor_validoYVacio")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   franja->setCedulaProfesor("0926687856");
   CHECK(franja->getCedulaProfesor() == "0926687856");
   franja->setCedulaProfesor("");
   CHECK(franja->getCedulaProfesor().empty());
   delete franja;
}

// ---------------------------------------------------------------------
// getClave
// ---------------------------------------------------------------------

// Verifica el formato de la clave: cédula + "-" + hora de inicio (sección 4).
TEST_CASE("FranjaHoraria_getClave_formato")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   CHECK(franja->getClave() == "1710034065-07:00");
   delete franja;
}

// Verifica que la clave cambie al cambiar la hora de inicio o la cédula,
// porque se calcula a partir de ellas.
TEST_CASE("FranjaHoraria_getClave_trasSetters")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   franja->setHoraInicio(new Hora(19, 0));
   CHECK(franja->getClave() == "1710034065-19:00");
   franja->setCedulaProfesor("0926687856");
   CHECK(franja->getClave() == "0926687856-19:00");
   delete franja;
}

// Verifica que una franja sin hora de inicio devuelva solo "cedula-" sin fallar.
TEST_CASE("FranjaHoraria_getClave_sinHoraInicio")
{
   FranjaHoraria* franja = new FranjaHoraria();
   franja->setCedulaProfesor("1710034065");
   CHECK(franja->getClave() == "1710034065-");
   delete franja;
}

// ---------------------------------------------------------------------
// mostrar
// ---------------------------------------------------------------------

// Verifica el formato acordado: "HH:MM - HH:MM | Aula: <aula>".
TEST_CASE("FranjaHoraria_mostrar_formato")
{
   FranjaHoraria* franja = ConsultaFranja::crear("07:00", "09:00");
   std::string impreso;
   {
      CapturaSalidaFranja captura;
      franja->mostrar();
      impreso = captura.texto();
   }
   CHECK(impreso == "07:00 - 09:00 | Aula: A-101\n");
   delete franja;
}

// Verifica que una franja vacía se muestre con "--:--" en lugar de fallar por los nullptr.
TEST_CASE("FranjaHoraria_mostrar_vacia")
{
   FranjaHoraria* franja = new FranjaHoraria();
   std::string impreso;
   {
      CapturaSalidaFranja captura;
      franja->mostrar();
      impreso = captura.texto();
   }
   CHECK(impreso == "--:-- - --:-- | Aula: \n");
   delete franja;
}

// ---------------------------------------------------------------------
// Interfaces
// ---------------------------------------------------------------------

// Verifica que la franja se pueda usar y destruir a través de IIdentificable*,
// como hará la ListaDoble (el destructor virtual libera sus horas).
TEST_CASE("FranjaHoraria_interfaces_polimorfismo")
{
   IIdentificable* identificable = ConsultaFranja::crear("11:00", "13:00");
   CHECK(identificable->getClave() == "1710034065-11:00");
   delete identificable;
}
