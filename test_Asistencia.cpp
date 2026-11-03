// Archivo: test_Asistencia.cpp
// Pruebas de unidad de la clase Asistencia: constructores (con copia de la hora de franja),
// transiciones de estado 0 -> 1 -> 2, casos que no deben cambiar nada, setters de Hora*,
// clave y mostrar().

#include "doctest.h"
#include "Asistencia.h"
#include "Hora.h"
#include <iostream>
#include <sstream>
#include <string>

// Clase auxiliar de prueba con métodos static para crear asistencias y registrar horas
// liberando las horas temporales que se usan solo para consultar.
class AyudaAsistencia
{
public:
   // Crea la asistencia de 1710034065 el 2026-11-03 para la franja que empieza en "HH:MM".
   static Asistencia* crear(std::string inicioFranja)
   {
      Hora* inicio = new Hora(inicioFranja);
      Asistencia* asistencia = new Asistencia("1710034065", "2026-11-03", inicio);
      delete inicio;   // la asistencia guardó una copia, la original se puede liberar
      return asistencia;
   }

   // Registra la entrada a la hora "HH:MM" (la asistencia guarda una copia).
   static void entrada(Asistencia* asistencia, std::string hhmm)
   {
      Hora* hora = new Hora(hhmm);
      asistencia->registrarEntrada(hora);
      delete hora;
   }

   // Registra la salida a la hora "HH:MM" (la asistencia guarda una copia).
   static void salida(Asistencia* asistencia, std::string hhmm)
   {
      Hora* hora = new Hora(hhmm);
      asistencia->registrarSalida(hora);
      delete hora;
   }
};

// Clase auxiliar de prueba que redirige std::cout a un buffer mientras existe.
class CapturaSalidaAsistencia
{
public:
   // Guarda el buffer original de std::cout y lo reemplaza por el interno.
   CapturaSalidaAsistencia() : original(std::cout.rdbuf(buffer.rdbuf())) {}

   // Devuelve std::cout a su buffer original.
   ~CapturaSalidaAsistencia()
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
// Constructores y getters
// ---------------------------------------------------------------------

// Verifica que el constructor por defecto deje todo vacío y en estado 0.
TEST_CASE("Asistencia_constructorDefecto_vacia")
{
   Asistencia* a = new Asistencia();
   CHECK(a->getCedulaProfesor().empty());
   CHECK(a->getFecha().empty());
   CHECK(a->getHoraInicioFranja() == nullptr);
   CHECK(a->getHoraEntrada() == nullptr);
   CHECK(a->getHoraSalida() == nullptr);
   CHECK(a->getEstado() == 0);
   delete a;
}

// Verifica que el constructor con parámetros guarde los datos y arranque sin entrada ni salida.
TEST_CASE("Asistencia_constructorParametros_valido")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   CHECK(a->getCedulaProfesor() == "1710034065");
   CHECK(a->getFecha() == "2026-11-03");
   REQUIRE(a->getHoraInicioFranja() != nullptr);
   CHECK(a->getHoraInicioFranja()->toString() == "07:00");
   CHECK(a->getHoraEntrada() == nullptr);
   CHECK(a->getHoraSalida() == nullptr);
   delete a;
}

// Verifica que se guarde una COPIA de la hora de franja (decisión 5): el puntero es distinto
// y la asistencia sigue funcionando después de liberar la hora original.
TEST_CASE("Asistencia_constructorParametros_copiaHoraFranja")
{
   Hora* original = new Hora(9, 0);
   Asistencia* a = new Asistencia("1710034065", "2026-11-03", original);
   CHECK(a->getHoraInicioFranja() != original);
   CHECK(a->getHoraInicioFranja()->getTotalMinutos() == original->getTotalMinutos());
   delete original;                                        // la original es de la prueba
   CHECK(a->getHoraInicioFranja()->toString() == "09:00"); // la copia sigue intacta
   delete a;
}

// Verifica que con hora de franja nullptr la asistencia quede sin hora de franja, sin fallar.
TEST_CASE("Asistencia_constructorParametros_horaFranjaNula")
{
   Asistencia* a = new Asistencia("1710034065", "2026-11-03", nullptr);
   CHECK(a->getHoraInicioFranja() == nullptr);
   CHECK(a->estaSinRegistro() == true);
   delete a;
}

// ---------------------------------------------------------------------
// Estados
// ---------------------------------------------------------------------

// Verifica que una asistencia nueva esté sin registro (estado 0) y no en los otros estados.
TEST_CASE("Asistencia_estaSinRegistro_nueva")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   CHECK(a->estaSinRegistro() == true);
   CHECK(a->tieneEntrada() == false);
   CHECK(a->estaCompleta() == false);
   CHECK(a->getEstado() == 0);
   delete a;
}

// ---------------------------------------------------------------------
// registrarEntrada
// ---------------------------------------------------------------------

// Verifica la transición 0 -> 1: queda con entrada, estado 1 y la hora guardada.
TEST_CASE("Asistencia_registrarEntrada_valida")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:05");
   CHECK(a->tieneEntrada() == true);
   CHECK(a->getEstado() == 1);
   CHECK(a->estaSinRegistro() == false);
   REQUIRE(a->getHoraEntrada() != nullptr);
   CHECK(a->getHoraEntrada()->toString() == "07:05");
   delete a;
}

// Verifica que se guarde una COPIA de la hora de entrada: puntero distinto al recibido.
TEST_CASE("Asistencia_registrarEntrada_guardaCopia")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   Hora* hora = new Hora(7, 2);
   a->registrarEntrada(hora);
   CHECK(a->getHoraEntrada() != hora);
   delete hora;                                         // la original es de la prueba
   CHECK(a->getHoraEntrada()->toString() == "07:02");   // la copia sigue intacta
   delete a;
}

// Verifica que registrar la entrada dos veces no cambie nada la segunda vez:
// se conserva la primera hora y el estado sigue en 1.
TEST_CASE("Asistencia_registrarEntrada_dosVeces")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:01");
   AyudaAsistencia::entrada(a, "07:09");
   CHECK(a->getEstado() == 1);
   CHECK(a->getHoraEntrada()->toString() == "07:01");
   delete a;
}

// Verifica que no se pueda registrar entrada en una asistencia ya completa.
TEST_CASE("Asistencia_registrarEntrada_trasCompleta")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:00");
   AyudaAsistencia::salida(a, "09:00");
   AyudaAsistencia::entrada(a, "07:05");
   CHECK(a->getEstado() == 2);
   CHECK(a->getHoraEntrada()->toString() == "07:00");
   delete a;
}

// Verifica que registrarEntrada(nullptr) no cambie el estado ni la hora.
TEST_CASE("Asistencia_registrarEntrada_nula")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   a->registrarEntrada(nullptr);
   CHECK(a->estaSinRegistro() == true);
   CHECK(a->getHoraEntrada() == nullptr);
   delete a;
}

// ---------------------------------------------------------------------
// registrarSalida
// ---------------------------------------------------------------------

// Verifica la transición 1 -> 2: tras la entrada, la salida deja la asistencia completa.
TEST_CASE("Asistencia_registrarSalida_trasEntrada")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:05");
   AyudaAsistencia::salida(a, "09:03");
   CHECK(a->estaCompleta() == true);
   CHECK(a->getEstado() == 2);
   CHECK(a->tieneEntrada() == false);   // ya no está en el estado 1
   REQUIRE(a->getHoraSalida() != nullptr);
   CHECK(a->getHoraSalida()->toString() == "09:03");
   delete a;
}

// Verifica que sin entrada previa la salida no cambie el estado: no hay salida sin entrada.
TEST_CASE("Asistencia_registrarSalida_sinEntrada")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::salida(a, "09:00");
   CHECK(a->estaSinRegistro() == true);
   CHECK(a->getEstado() == 0);
   CHECK(a->getHoraSalida() == nullptr);
   delete a;
}

// Verifica que una segunda salida no cambie nada: se conserva la primera hora de salida.
TEST_CASE("Asistencia_registrarSalida_dosVeces")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:00");
   AyudaAsistencia::salida(a, "09:01");
   AyudaAsistencia::salida(a, "09:08");
   CHECK(a->getEstado() == 2);
   CHECK(a->getHoraSalida()->toString() == "09:01");
   delete a;
}

// Verifica que registrarSalida(nullptr) no cambie el estado.
TEST_CASE("Asistencia_registrarSalida_nula")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:00");
   a->registrarSalida(nullptr);
   CHECK(a->getEstado() == 1);
   CHECK(a->getHoraSalida() == nullptr);
   delete a;
}

// ---------------------------------------------------------------------
// Setters de Hora*
// ---------------------------------------------------------------------

// Verifica que setHoraEntrada reemplace la hora (liberando la anterior) sin cambiar el estado.
TEST_CASE("Asistencia_setHoraEntrada_reemplazaSinCambiarEstado")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:05");
   Hora* nueva = new Hora(7, 8);
   a->setHoraEntrada(nueva);   // la asistencia se queda con nueva y libera la anterior
   CHECK(a->getHoraEntrada() == nueva);
   CHECK(a->getEstado() == 1);
   delete a;
}

// Verifica que setHoraEntrada en una asistencia nueva no la haga pasar a estado 1:
// solo registrarEntrada cambia el estado.
TEST_CASE("Asistencia_setHoraEntrada_noCambiaEstado")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   a->setHoraEntrada(new Hora(7, 3));
   CHECK(a->estaSinRegistro() == true);
   // Después, registrarEntrada reemplaza esa hora (liberándola) y cambia el estado
   AyudaAsistencia::entrada(a, "07:04");
   CHECK(a->getHoraEntrada()->toString() == "07:04");
   CHECK(a->getEstado() == 1);
   delete a;
}

// Verifica que pasar el mismo puntero a setHoraEntrada no lo libere.
TEST_CASE("Asistencia_setHoraEntrada_mismoPuntero")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:05");
   a->setHoraEntrada(a->getHoraEntrada());
   CHECK(a->getHoraEntrada()->toString() == "07:05");
   delete a;
}

// Verifica que setHoraEntrada(nullptr) libere la anterior y deje la entrada vacía.
TEST_CASE("Asistencia_setHoraEntrada_nulo")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:05");
   a->setHoraEntrada(nullptr);
   CHECK(a->getHoraEntrada() == nullptr);
   delete a;
}

// Verifica que setHoraSalida reemplace la hora (liberando la anterior) sin cambiar el estado.
TEST_CASE("Asistencia_setHoraSalida_reemplazaSinCambiarEstado")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:00");
   AyudaAsistencia::salida(a, "09:00");
   Hora* nueva = new Hora(9, 5);
   a->setHoraSalida(nueva);
   CHECK(a->getHoraSalida() == nueva);
   CHECK(a->getEstado() == 2);
   delete a;
}

// Verifica que setHoraSalida no haga pasar la asistencia a completa.
TEST_CASE("Asistencia_setHoraSalida_noCambiaEstado")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:00");
   a->setHoraSalida(new Hora(9, 0));
   CHECK(a->getEstado() == 1);
   delete a;
}

// Verifica que pasar el mismo puntero a setHoraSalida no lo libere.
TEST_CASE("Asistencia_setHoraSalida_mismoPuntero")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:00");
   AyudaAsistencia::salida(a, "09:02");
   a->setHoraSalida(a->getHoraSalida());
   CHECK(a->getHoraSalida()->toString() == "09:02");
   delete a;
}

// Verifica que setHoraSalida(nullptr) libere la anterior y deje la salida vacía.
TEST_CASE("Asistencia_setHoraSalida_nulo")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:00");
   AyudaAsistencia::salida(a, "09:02");
   a->setHoraSalida(nullptr);
   CHECK(a->getHoraSalida() == nullptr);
   delete a;
}

// ---------------------------------------------------------------------
// getClave
// ---------------------------------------------------------------------

// Verifica el formato de la clave: cédula + "-" + fecha + "-" + hora de franja (sección 4).
TEST_CASE("Asistencia_getClave_formato")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   CHECK(a->getClave() == "1710034065-2026-11-03-07:00");
   delete a;
}

// Verifica que dos franjas del mismo día tengan claves distintas: se registran por separado.
TEST_CASE("Asistencia_getClave_franjasDistintas")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   Asistencia* b = AyudaAsistencia::crear("09:00");
   CHECK(a->getClave() != b->getClave());
   CHECK(b->getClave() == "1710034065-2026-11-03-09:00");
   delete a;
   delete b;
}

// Verifica que la clave no cambie al registrar entrada y salida: solo depende de la franja.
TEST_CASE("Asistencia_getClave_noCambiaConRegistros")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:05");
   AyudaAsistencia::salida(a, "09:05");
   CHECK(a->getClave() == "1710034065-2026-11-03-07:00");
   delete a;
}

// Verifica la clave sin hora de franja: "cedula-fecha-".
TEST_CASE("Asistencia_getClave_sinHoraFranja")
{
   Asistencia* a = new Asistencia("1710034065", "2026-11-03", nullptr);
   CHECK(a->getClave() == "1710034065-2026-11-03-");
   delete a;
}

// ---------------------------------------------------------------------
// mostrar
// ---------------------------------------------------------------------

// Verifica el formato de mostrar() en estado 0: entrada y salida como "--:--".
TEST_CASE("Asistencia_mostrar_sinRegistro")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   std::string impreso;
   {
      CapturaSalidaAsistencia captura;
      a->mostrar();
      impreso = captura.texto();
   }
   CHECK(impreso == "2026-11-03 | Cédula: 1710034065 | Franja: 07:00 | Entrada: --:-- | "
                    "Salida: --:-- | Estado: Sin registro\n");
   delete a;
}

// Verifica el formato de mostrar() en estado 1: con entrada y sin salida.
TEST_CASE("Asistencia_mostrar_entradaRegistrada")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:05");
   std::string impreso;
   {
      CapturaSalidaAsistencia captura;
      a->mostrar();
      impreso = captura.texto();
   }
   CHECK(impreso == "2026-11-03 | Cédula: 1710034065 | Franja: 07:00 | Entrada: 07:05 | "
                    "Salida: --:-- | Estado: Entrada registrada\n");
   delete a;
}

// Verifica el formato de mostrar() en estado 2: con entrada y salida.
TEST_CASE("Asistencia_mostrar_completa")
{
   Asistencia* a = AyudaAsistencia::crear("07:00");
   AyudaAsistencia::entrada(a, "07:05");
   AyudaAsistencia::salida(a, "09:03");
   std::string impreso;
   {
      CapturaSalidaAsistencia captura;
      a->mostrar();
      impreso = captura.texto();
   }
   CHECK(impreso == "2026-11-03 | Cédula: 1710034065 | Franja: 07:00 | Entrada: 07:05 | "
                    "Salida: 09:03 | Estado: Completa\n");
   delete a;
}

// Verifica que una asistencia vacía se muestre sin fallar por los nullptr.
TEST_CASE("Asistencia_mostrar_vacia")
{
   Asistencia* a = new Asistencia();
   std::string impreso;
   {
      CapturaSalidaAsistencia captura;
      a->mostrar();
      impreso = captura.texto();
   }
   CHECK(impreso == " | Cédula:  | Franja: --:-- | Entrada: --:-- | Salida: --:-- | Estado: Sin registro\n");
   delete a;
}

// ---------------------------------------------------------------------
// Interfaces y destructor
// ---------------------------------------------------------------------

// Verifica que la asistencia se pueda usar y destruir a través de IIdentificable*,
// como hará la ListaDoble (el destructor virtual libera sus tres horas).
TEST_CASE("Asistencia_interfaces_polimorfismo")
{
   Asistencia* a = AyudaAsistencia::crear("11:00");
   AyudaAsistencia::entrada(a, "11:00");
   AyudaAsistencia::salida(a, "13:10");
   IIdentificable* identificable = a;
   CHECK(identificable->getClave() == "1710034065-2026-11-03-11:00");
   CHECK_NOTHROW(delete identificable);
}
