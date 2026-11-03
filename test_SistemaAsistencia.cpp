// Archivo: test_SistemaAsistencia.cpp
// Pruebas de unidad de la clase SistemaAsistencia: alta de profesores y franjas, registro de
// entrada y salida con las ventanas de 10 minutos, duplicados, flujo completo y listados.
// Ninguna prueba usa el reloj real: la fecha y la hora se pasan siempre como parámetros.
// Regla de memoria (decisión 4): si agregar... devuelve false, la prueba libera el objeto.

#include "doctest.h"
#include "SistemaAsistencia.h"
#include "Profesor.h"
#include "FranjaHoraria.h"
#include "Hora.h"
#include <iostream>
#include <sstream>
#include <string>

// Clase auxiliar de prueba con métodos static para preparar el sistema y registrar
// sin repetir la gestión de memoria en cada prueba.
class AyudaSistema
{
public:
   // Intenta agregar un profesor; si el sistema lo rechaza, lo libera aquí.
   static bool agregarProfesor(SistemaAsistencia* sistema, std::string cedula,
                               std::string nombre = "Juan", std::string apellido = "Pérez")
   {
      Profesor* profesor = new Profesor(cedula, nombre, apellido);
      bool aceptado = sistema->agregarProfesor(profesor);
      if (!aceptado)
      {
         delete profesor;
      }
      return aceptado;
   }

   // Intenta agregar la franja "HH:MM"-"HH:MM" al profesor; si se rechaza, la libera aquí.
   static bool agregarFranja(SistemaAsistencia* sistema, std::string cedula,
                             std::string inicio, std::string fin, std::string aula = "A-101")
   {
      FranjaHoraria* franja = new FranjaHoraria(new Hora(inicio), new Hora(fin), aula, cedula);
      bool aceptada = sistema->agregarFranja(cedula, franja);
      if (!aceptada)
      {
         delete franja;
      }
      return aceptada;
   }

   // Registra la entrada a la hora "HH:MM" y libera la hora (el sistema solo la consulta).
   static bool entrada(SistemaAsistencia* sistema, std::string cedula, std::string fecha, std::string hhmm)
   {
      Hora* hora = new Hora(hhmm);
      bool resultado = sistema->registrarEntrada(cedula, fecha, hora);
      delete hora;
      return resultado;
   }

   // Registra la salida a la hora "HH:MM" y libera la hora (el sistema solo la consulta).
   static bool salida(SistemaAsistencia* sistema, std::string cedula, std::string fecha, std::string hhmm)
   {
      Hora* hora = new Hora(hhmm);
      bool resultado = sistema->registrarSalida(cedula, fecha, hora);
      delete hora;
      return resultado;
   }

   // Crea un sistema con el profesor 1710034065 y la franja 07:00-09:00.
   static SistemaAsistencia* sistemaConFranja(void)
   {
      SistemaAsistencia* sistema = new SistemaAsistencia();
      agregarProfesor(sistema, "1710034065");
      agregarFranja(sistema, "1710034065", "07:00", "09:00");
      return sistema;
   }
};

// Clase auxiliar de prueba que redirige std::cout a un buffer mientras existe.
class CapturaSalidaSistema
{
public:
   // Guarda el buffer original de std::cout y lo reemplaza por el interno.
   CapturaSalidaSistema() : original(std::cout.rdbuf(buffer.rdbuf())) {}

   // Devuelve std::cout a su buffer original.
   ~CapturaSalidaSistema()
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
// Constructor y destructor
// ---------------------------------------------------------------------

// Verifica que un sistema nuevo no tenga asistencias y que se destruya sin fallar.
TEST_CASE("SistemaAsistencia_constructor_vacio")
{
   SistemaAsistencia* sistema = new SistemaAsistencia();
   std::string impreso;
   {
      CapturaSalidaSistema captura;
      sistema->mostrarAsistencias();
      impreso = captura.texto();
   }
   CHECK(impreso == "No hay asistencias registradas.\n");
   CHECK_NOTHROW(delete sistema);
}

// Verifica que destruir un sistema con profesores, franjas y asistencias no falle:
// el destructor libera todo en cascada.
TEST_CASE("SistemaAsistencia_destructor_conDatos")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:05");
   CHECK_NOTHROW(delete sistema);
}

// ---------------------------------------------------------------------
// agregarProfesor
// ---------------------------------------------------------------------

// Verifica que se acepte un profesor con cédula real y nombre y apellido válidos.
TEST_CASE("SistemaAsistencia_agregarProfesor_valido")
{
   SistemaAsistencia* sistema = new SistemaAsistencia();
   CHECK(AyudaSistema::agregarProfesor(sistema, "1710034065", "Juan", "Pérez") == true);
   CHECK(AyudaSistema::agregarProfesor(sistema, "0926687856", "María José", "Ñañez") == true);
   delete sistema;
}

// Verifica que se rechace una segunda alta con la misma cédula.
TEST_CASE("SistemaAsistencia_agregarProfesor_cedulaDuplicada")
{
   SistemaAsistencia* sistema = new SistemaAsistencia();
   CHECK(AyudaSistema::agregarProfesor(sistema, "1710034065", "Juan", "Pérez") == true);
   CHECK(AyudaSistema::agregarProfesor(sistema, "1710034065", "Ana", "López") == false);
   delete sistema;
}

// Verifica que se rechacen cédulas inválidas: verificador incorrecto, provincia inexistente y vacía.
TEST_CASE("SistemaAsistencia_agregarProfesor_cedulaInvalida")
{
   SistemaAsistencia* sistema = new SistemaAsistencia();
   CHECK(AyudaSistema::agregarProfesor(sistema, "1710034066") == false);
   CHECK(AyudaSistema::agregarProfesor(sistema, "2510034065") == false);
   CHECK(AyudaSistema::agregarProfesor(sistema, "") == false);
   delete sistema;
}

// Verifica que se rechace un nombre con números.
TEST_CASE("SistemaAsistencia_agregarProfesor_nombreConNumeros")
{
   SistemaAsistencia* sistema = new SistemaAsistencia();
   CHECK(AyudaSistema::agregarProfesor(sistema, "1710034065", "Juan3", "Pérez") == false);
   delete sistema;
}

// Verifica que se rechace un apellido inválido (vacío o con símbolos).
TEST_CASE("SistemaAsistencia_agregarProfesor_apellidoInvalido")
{
   SistemaAsistencia* sistema = new SistemaAsistencia();
   CHECK(AyudaSistema::agregarProfesor(sistema, "1710034065", "Juan", "") == false);
   CHECK(AyudaSistema::agregarProfesor(sistema, "1710034065", "Juan", "Pérez!") == false);
   delete sistema;
}

// Verifica que agregar nullptr devuelva false sin fallar.
TEST_CASE("SistemaAsistencia_agregarProfesor_nulo")
{
   SistemaAsistencia* sistema = new SistemaAsistencia();
   CHECK(sistema->agregarProfesor(nullptr) == false);
   delete sistema;
}

// Verifica que un profesor rechazado NO sea liberado por el sistema: la prueba aún puede usarlo.
TEST_CASE("SistemaAsistencia_agregarProfesor_rechazadoNoSeLibera")
{
   SistemaAsistencia* sistema = new SistemaAsistencia();
   Profesor* invalido = new Profesor("123", "Juan", "Pérez");
   CHECK(sistema->agregarProfesor(invalido) == false);
   CHECK(invalido->getCedula() == "123");   // sigue vivo
   delete invalido;
   delete sistema;
}

// ---------------------------------------------------------------------
// agregarFranja
// ---------------------------------------------------------------------

// Verifica que se acepte una franja predefinida para un profesor existente.
TEST_CASE("SistemaAsistencia_agregarFranja_profesorExistente")
{
   SistemaAsistencia* sistema = new SistemaAsistencia();
   AyudaSistema::agregarProfesor(sistema, "1710034065");
   CHECK(AyudaSistema::agregarFranja(sistema, "1710034065", "07:00", "09:00") == true);
   delete sistema;
}

// Verifica que se rechace la franja de un profesor que no está registrado.
TEST_CASE("SistemaAsistencia_agregarFranja_profesorInexistente")
{
   SistemaAsistencia* sistema = new SistemaAsistencia();
   CHECK(AyudaSistema::agregarFranja(sistema, "1710034065", "07:00", "09:00") == false);
   delete sistema;
}

// Verifica que se rechace un aula con formato inválido.
TEST_CASE("SistemaAsistencia_agregarFranja_aulaInvalida")
{
   SistemaAsistencia* sistema = new SistemaAsistencia();
   AyudaSistema::agregarProfesor(sistema, "1710034065");
   CHECK(AyudaSistema::agregarFranja(sistema, "1710034065", "07:00", "09:00", "A 101!") == false);
   CHECK(AyudaSistema::agregarFranja(sistema, "1710034065", "07:00", "09:00", "") == false);
   delete sistema;
}

// Verifica que el sistema respete las reglas del profesor: franja no predefinida y duplicada.
TEST_CASE("SistemaAsistencia_agregarFranja_rechazadaPorProfesor")
{
   SistemaAsistencia* sistema = new SistemaAsistencia();
   AyudaSistema::agregarProfesor(sistema, "1710034065");
   CHECK(AyudaSistema::agregarFranja(sistema, "1710034065", "08:00", "10:00") == false);
   CHECK(AyudaSistema::agregarFranja(sistema, "1710034065", "07:00", "09:00") == true);
   CHECK(AyudaSistema::agregarFranja(sistema, "1710034065", "07:00", "09:00") == false);
   delete sistema;
}

// Verifica que agregar una franja nullptr devuelva false sin fallar.
TEST_CASE("SistemaAsistencia_agregarFranja_nula")
{
   SistemaAsistencia* sistema = new SistemaAsistencia();
   AyudaSistema::agregarProfesor(sistema, "1710034065");
   CHECK(sistema->agregarFranja("1710034065", nullptr) == false);
   delete sistema;
}

// ---------------------------------------------------------------------
// registrarEntrada
// ---------------------------------------------------------------------

// Verifica que se acepte la entrada a tiempo, en el límite inferior de la ventana (07:00).
TEST_CASE("SistemaAsistencia_registrarEntrada_aTiempoInicio")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   CHECK(AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:00") == true);
   delete sistema;
}

// Verifica que se acepte la entrada en el límite superior de la ventana (07:10, incluido).
TEST_CASE("SistemaAsistencia_registrarEntrada_aTiempoLimite")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   CHECK(AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:10") == true);
   delete sistema;
}

// Verifica que se rechace la entrada tarde (07:11): fuera de la ventana no se marca atrasado.
TEST_CASE("SistemaAsistencia_registrarEntrada_tarde")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   CHECK(AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:11") == false);
   delete sistema;
}

// Verifica que se rechace la entrada de una cédula que no está registrada.
TEST_CASE("SistemaAsistencia_registrarEntrada_profesorInexistente")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   CHECK(AyudaSistema::entrada(sistema, "0926687856", "2026-11-03", "07:05") == false);
   delete sistema;
}

// Verifica que se rechace la entrada a una hora en la que el profesor no tiene franja.
TEST_CASE("SistemaAsistencia_registrarEntrada_sinFranjaAEsaHora")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   CHECK(AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "11:00") == false);
   delete sistema;
}

// Verifica que se rechace una segunda entrada en la misma franja y fecha
// (no puede haber dos asistencias con la misma cédula + fecha + franja).
TEST_CASE("SistemaAsistencia_registrarEntrada_duplicada")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   CHECK(AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:01") == true);
   CHECK(AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:08") == false);
   delete sistema;
}

// Verifica que tampoco se pueda volver a registrar la entrada después de completar la asistencia.
TEST_CASE("SistemaAsistencia_registrarEntrada_trasCompleta")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:01");
   AyudaSistema::salida(sistema, "1710034065", "2026-11-03", "09:01");
   CHECK(AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:05") == false);
   delete sistema;
}

// Verifica que se rechace una fecha inválida (formato incorrecto o fecha inexistente).
TEST_CASE("SistemaAsistencia_registrarEntrada_fechaInvalida")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   CHECK(AyudaSistema::entrada(sistema, "1710034065", "03-11-2026", "07:05") == false);
   CHECK(AyudaSistema::entrada(sistema, "1710034065", "2026-02-29", "07:05") == false);
   CHECK(AyudaSistema::entrada(sistema, "1710034065", "", "07:05") == false);
   delete sistema;
}

// Verifica que con hora nullptr se rechace sin fallar.
TEST_CASE("SistemaAsistencia_registrarEntrada_horaNula")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   CHECK(sistema->registrarEntrada("1710034065", "2026-11-03", nullptr) == false);
   delete sistema;
}

// Verifica que la misma franja en fechas distintas genere asistencias distintas:
// la clave incluye la fecha, así que el día siguiente se puede volver a registrar.
TEST_CASE("SistemaAsistencia_registrarEntrada_otraFecha")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   CHECK(AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:05") == true);
   CHECK(AyudaSistema::entrada(sistema, "1710034065", "2026-11-04", "07:05") == true);
   delete sistema;
}

// ---------------------------------------------------------------------
// registrarSalida
// ---------------------------------------------------------------------

// Verifica que se acepte la salida a tiempo después de la entrada (límites 09:00 y 09:10).
TEST_CASE("SistemaAsistencia_registrarSalida_aTiempo")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:05");
   CHECK(AyudaSistema::salida(sistema, "1710034065", "2026-11-03", "09:10") == true);
   delete sistema;
}

// Verifica que se rechace la salida sin entrada previa en esa franja y fecha.
TEST_CASE("SistemaAsistencia_registrarSalida_sinEntrada")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   CHECK(AyudaSistema::salida(sistema, "1710034065", "2026-11-03", "09:00") == false);
   delete sistema;
}

// Verifica que la entrada de otra fecha no sirva para registrar la salida de hoy.
TEST_CASE("SistemaAsistencia_registrarSalida_entradaDeOtraFecha")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:05");
   CHECK(AyudaSistema::salida(sistema, "1710034065", "2026-11-04", "09:00") == false);
   delete sistema;
}

// Verifica que se rechace la salida fuera de la ventana: antes (08:59) y después (09:11).
TEST_CASE("SistemaAsistencia_registrarSalida_fueraDeVentana")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:05");
   CHECK(AyudaSistema::salida(sistema, "1710034065", "2026-11-03", "08:59") == false);
   CHECK(AyudaSistema::salida(sistema, "1710034065", "2026-11-03", "09:11") == false);
   // La asistencia sigue esperando la salida: dentro de la ventana todavía se acepta
   CHECK(AyudaSistema::salida(sistema, "1710034065", "2026-11-03", "09:00") == true);
   delete sistema;
}

// Verifica que se rechace una segunda salida: la asistencia ya está completa.
TEST_CASE("SistemaAsistencia_registrarSalida_duplicada")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:05");
   CHECK(AyudaSistema::salida(sistema, "1710034065", "2026-11-03", "09:01") == true);
   CHECK(AyudaSistema::salida(sistema, "1710034065", "2026-11-03", "09:05") == false);
   delete sistema;
}

// Verifica que se rechace la salida de un profesor inexistente, con fecha inválida o con hora nula.
TEST_CASE("SistemaAsistencia_registrarSalida_datosInvalidos")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:05");
   CHECK(AyudaSistema::salida(sistema, "0926687856", "2026-11-03", "09:00") == false);
   CHECK(AyudaSistema::salida(sistema, "1710034065", "2026-13-03", "09:00") == false);
   CHECK(sistema->registrarSalida("1710034065", "2026-11-03", nullptr) == false);
   delete sistema;
}

// ---------------------------------------------------------------------
// Flujo completo
// ---------------------------------------------------------------------

// Verifica el flujo completo con dos franjas consecutivas (7-9 y 9-11): cada franja se registra
// por separado. A las 09:02 la salida es de 7-9; a las 09:05 la entrada es de 9-11.
// Al final ambas asistencias deben aparecer completas.
TEST_CASE("SistemaAsistencia_flujoCompleto_franjasConsecutivas")
{
   SistemaAsistencia* sistema = new SistemaAsistencia();
   REQUIRE(AyudaSistema::agregarProfesor(sistema, "1710034065", "Juan", "Pérez") == true);
   REQUIRE(AyudaSistema::agregarFranja(sistema, "1710034065", "07:00", "09:00", "A-101") == true);
   REQUIRE(AyudaSistema::agregarFranja(sistema, "1710034065", "09:00", "11:00", "B-202") == true);

   CHECK(AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:05") == true);   // entra a 7-9
   CHECK(AyudaSistema::salida(sistema, "1710034065", "2026-11-03", "09:02") == true);    // sale de 7-9
   CHECK(AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "09:05") == true);   // entra a 9-11
   CHECK(AyudaSistema::salida(sistema, "1710034065", "2026-11-03", "11:10") == true);    // sale de 9-11

   std::string impreso;
   {
      CapturaSalidaSistema captura;
      sistema->mostrarAsistencias();
      impreso = captura.texto();
   }
   CHECK(impreso ==
         "2026-11-03 | Cédula: 1710034065 | Franja: 07:00 | Entrada: 07:05 | Salida: 09:02 | Estado: Completa\n"
         "2026-11-03 | Cédula: 1710034065 | Franja: 09:00 | Entrada: 09:05 | Salida: 11:10 | Estado: Completa\n");
   delete sistema;
}

// ---------------------------------------------------------------------
// mostrarAsistencias y mostrarAsistenciaProfesor
// ---------------------------------------------------------------------

// Verifica que mostrarAsistencias muestre una asistencia con entrada y sin salida.
TEST_CASE("SistemaAsistencia_mostrarAsistencias_conRegistros")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:05");
   std::string impreso;
   {
      CapturaSalidaSistema captura;
      sistema->mostrarAsistencias();
      impreso = captura.texto();
   }
   CHECK(impreso == "2026-11-03 | Cédula: 1710034065 | Franja: 07:00 | Entrada: 07:05 | "
                    "Salida: --:-- | Estado: Entrada registrada\n");
   delete sistema;
}

// Verifica que mostrarAsistenciaProfesor muestre solo las asistencias de esa cédula.
TEST_CASE("SistemaAsistencia_mostrarAsistenciaProfesor_filtraPorCedula")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   AyudaSistema::agregarProfesor(sistema, "0926687856", "Ana", "López");
   AyudaSistema::agregarFranja(sistema, "0926687856", "07:00", "09:00");
   AyudaSistema::entrada(sistema, "1710034065", "2026-11-03", "07:05");
   AyudaSistema::entrada(sistema, "0926687856", "2026-11-03", "07:06");
   std::string impreso;
   {
      CapturaSalidaSistema captura;
      sistema->mostrarAsistenciaProfesor("0926687856");
      impreso = captura.texto();
   }
   CHECK(impreso.find("0926687856") != std::string::npos);
   CHECK(impreso.find("1710034065") == std::string::npos);   // la otra cédula no aparece
   delete sistema;
}

// Verifica el mensaje cuando la cédula no tiene asistencias.
TEST_CASE("SistemaAsistencia_mostrarAsistenciaProfesor_sinRegistros")
{
   SistemaAsistencia* sistema = AyudaSistema::sistemaConFranja();
   std::string impreso;
   {
      CapturaSalidaSistema captura;
      sistema->mostrarAsistenciaProfesor("1710034065");
      impreso = captura.texto();
   }
   CHECK(impreso == "No hay asistencias registradas para la cédula 1710034065.\n");
   delete sistema;
}
