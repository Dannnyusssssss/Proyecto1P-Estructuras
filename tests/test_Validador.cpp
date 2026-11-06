// Archivo: test_Validador.cpp
// Pruebas de unidad de la clase Validador: casos válidos, inválidos, límite y vacíos
// de cada uno de sus seis métodos.

#include "doctest.h"
#include "Validador.h"

// ---------------------------------------------------------------------
// validarCedula
// ---------------------------------------------------------------------

// Verifica dos cédulas reales de distintas provincias (Pichincha y Guayas):
// pasan las tres regex y el dígito verificador calculado coincide con el décimo.
TEST_CASE("Validador_validarCedula_validas")
{
   CHECK(Validador::validarCedula("1710034065") == true);
   CHECK(Validador::validarCedula("0926687856") == true);
}

// Verifica que se rechace una cédula con formato correcto pero verificador incorrecto:
// para 171003406 el verificador es 5, no 6.
TEST_CASE("Validador_validarCedula_verificadorIncorrecto")
{
   CHECK(Validador::validarCedula("1710034066") == false);
}

// Verifica que se rechacen las provincias inexistentes 25 y 00.
TEST_CASE("Validador_validarCedula_provinciaInvalida")
{
   CHECK(Validador::validarCedula("2510034065") == false);
   CHECK(Validador::validarCedula("0000000000") == false);
}

// Verifica los límites del rango de provincias: 01, 24 y 30 son válidas.
// Las cédulas se construyeron con el verificador correcto para que solo influya la provincia.
TEST_CASE("Validador_validarCedula_provinciaLimites")
{
   CHECK(Validador::validarCedula("0100000009") == true);    // provincia 01: suma 1 -> verificador 9
   CHECK(Validador::validarCedula("2400000002") == true);    // provincia 24: suma 8 -> verificador 2
   CHECK(Validador::validarCedula("3000000004") == true);    // provincia 30 (exterior): suma 6 -> verificador 4
}

// Verifica que se rechace un tercer dígito mayor que 5 (no es persona natural).
TEST_CASE("Validador_validarCedula_tercerDigitoInvalido")
{
   CHECK(Validador::validarCedula("1770034065") == false);
}

// Verifica que se rechacen cédulas con 9 dígitos o con letras: no cumplen ^\d{10}$.
TEST_CASE("Validador_validarCedula_formatoInvalido")
{
   CHECK(Validador::validarCedula("171003406") == false);
   CHECK(Validador::validarCedula("17100340A5") == false);
   CHECK(Validador::validarCedula("17100340655") == false);   // 11 dígitos
   CHECK(Validador::validarCedula(" 1710034065") == false);   // espacio al inicio
}

// Verifica que la cédula vacía se rechace.
TEST_CASE("Validador_validarCedula_vacia")
{
   CHECK(Validador::validarCedula("") == false);
}

// ---------------------------------------------------------------------
// validarNombre
// ---------------------------------------------------------------------

// Verifica nombres válidos: una palabra, dos palabras con tilde, y con ñ.
TEST_CASE("Validador_validarNombre_validos")
{
   CHECK(Validador::validarNombre("Juan") == true);
   CHECK(Validador::validarNombre("María José") == true);
   CHECK(Validador::validarNombre("Ñañez") == true);
   CHECK(Validador::validarNombre("Güiracocha") == true);
}

// Verifica que se rechacen nombres con dígitos o símbolos.
TEST_CASE("Validador_validarNombre_invalidos")
{
   CHECK(Validador::validarNombre("Juan3") == false);
   CHECK(Validador::validarNombre("Juan!") == false);
   CHECK(Validador::validarNombre("Juan_Pérez") == false);
}

// Verifica que los espacios mal ubicados se rechacen: al inicio, al final o dobles.
TEST_CASE("Validador_validarNombre_espaciosMalUbicados")
{
   CHECK(Validador::validarNombre(" Juan") == false);
   CHECK(Validador::validarNombre("Juan ") == false);
   CHECK(Validador::validarNombre("María  José") == false);
}

// Verifica que el texto vacío y el texto de solo espacios se rechacen.
TEST_CASE("Validador_validarNombre_vacio")
{
   CHECK(Validador::validarNombre("") == false);
   CHECK(Validador::validarNombre("  ") == false);
}

// ---------------------------------------------------------------------
// validarHora
// ---------------------------------------------------------------------

// Verifica horas válidas en formato de 24 horas.
TEST_CASE("Validador_validarHora_validas")
{
   CHECK(Validador::validarHora("07:10") == true);
   CHECK(Validador::validarHora("13:45") == true);
}

// Verifica los límites del día: 00:00 y 23:59 son válidas; 24:00 y 07:60 no.
TEST_CASE("Validador_validarHora_limites")
{
   CHECK(Validador::validarHora("00:00") == true);
   CHECK(Validador::validarHora("23:59") == true);
   CHECK(Validador::validarHora("24:00") == false);
   CHECK(Validador::validarHora("07:60") == false);
}

// Verifica que se rechacen formatos incorrectos: sin cero a la izquierda, letras, sin separador.
TEST_CASE("Validador_validarHora_invalidas")
{
   CHECK(Validador::validarHora("7:10") == false);
   CHECK(Validador::validarHora("ab:cd") == false);
   CHECK(Validador::validarHora("0710") == false);
   CHECK(Validador::validarHora("07:10 ") == false);
}

// Verifica que la hora vacía se rechace.
TEST_CASE("Validador_validarHora_vacia")
{
   CHECK(Validador::validarHora("") == false);
}

// ---------------------------------------------------------------------
// validarFecha
// ---------------------------------------------------------------------

// Verifica fechas válidas en formato AAAA-MM-DD.
TEST_CASE("Validador_validarFecha_valida")
{
   CHECK(Validador::validarFecha("2026-11-03") == true);
   CHECK(Validador::validarFecha("2026-01-01") == true);
}

// Verifica el último día de cada tipo de mes: 31 en enero y diciembre, 30 en abril
// y 28 en febrero de un año no bisiesto.
TEST_CASE("Validador_validarFecha_limitesDelMes")
{
   CHECK(Validador::validarFecha("2026-01-31") == true);
   CHECK(Validador::validarFecha("2026-12-31") == true);
   CHECK(Validador::validarFecha("2026-04-30") == true);
   CHECK(Validador::validarFecha("2026-02-28") == true);
}

// Verifica que se rechacen días que no existen en ese mes y meses o días fuera de rango.
TEST_CASE("Validador_validarFecha_fueraDeRango")
{
   CHECK(Validador::validarFecha("2026-13-01") == false);   // mes 13
   CHECK(Validador::validarFecha("2026-00-10") == false);   // mes 00
   CHECK(Validador::validarFecha("2026-05-00") == false);   // día 00
   CHECK(Validador::validarFecha("2026-04-31") == false);   // abril tiene 30 días
   CHECK(Validador::validarFecha("2026-02-30") == false);   // febrero nunca llega a 30
   CHECK(Validador::validarFecha("2026-01-32") == false);   // ningún mes tiene 32
}

// Verifica que el 29 de febrero se acepte en años bisiestos:
// 2024 es múltiplo de 4 (y no de 100); 2000 es múltiplo de 400.
TEST_CASE("Validador_validarFecha_bisiestoValido")
{
   CHECK(Validador::validarFecha("2024-02-29") == true);
   CHECK(Validador::validarFecha("2000-02-29") == true);
}

// Verifica que el 29 de febrero se rechace en años no bisiestos:
// 2026 no es múltiplo de 4; 2100 es múltiplo de 100 pero no de 400.
TEST_CASE("Validador_validarFecha_bisiestoInvalido")
{
   CHECK(Validador::validarFecha("2026-02-29") == false);
   CHECK(Validador::validarFecha("2100-02-29") == false);
}

// Verifica que se rechacen formatos distintos de AAAA-MM-DD.
TEST_CASE("Validador_validarFecha_formatoInvalido")
{
   CHECK(Validador::validarFecha("03-11-2026") == false);
   CHECK(Validador::validarFecha("2026/11/03") == false);
   CHECK(Validador::validarFecha("2026-1-3") == false);
}

// Verifica que la fecha vacía se rechace.
TEST_CASE("Validador_validarFecha_vacia")
{
   CHECK(Validador::validarFecha("") == false);
}

// ---------------------------------------------------------------------
// validarAula
// ---------------------------------------------------------------------

// Verifica aulas válidas: con guion, sin guion y con varios guiones.
TEST_CASE("Validador_validarAula_validas")
{
   CHECK(Validador::validarAula("A-101") == true);
   CHECK(Validador::validarAula("Lab3") == true);
   CHECK(Validador::validarAula("B2-LAB-1") == true);
}

// Verifica que se rechacen aulas con espacios, símbolos o guiones mal ubicados.
TEST_CASE("Validador_validarAula_invalidas")
{
   CHECK(Validador::validarAula("A 101!") == false);
   CHECK(Validador::validarAula("A 101") == false);
   CHECK(Validador::validarAula("-A101") == false);
   CHECK(Validador::validarAula("A101-") == false);
   CHECK(Validador::validarAula("A--101") == false);
}

// Verifica que el aula vacía se rechace.
TEST_CASE("Validador_validarAula_vacia")
{
   CHECK(Validador::validarAula("") == false);
}

// ---------------------------------------------------------------------
// validarOpcionMenu
// ---------------------------------------------------------------------

// Verifica opciones válidas de uno y dos dígitos.
TEST_CASE("Validador_validarOpcionMenu_validas")
{
   CHECK(Validador::validarOpcionMenu("1") == true);
   CHECK(Validador::validarOpcionMenu("0") == true);
   CHECK(Validador::validarOpcionMenu("12") == true);
}

// Verifica que se rechacen letras, signos, espacios y números de más de dos dígitos.
TEST_CASE("Validador_validarOpcionMenu_invalidas")
{
   CHECK(Validador::validarOpcionMenu("a") == false);
   CHECK(Validador::validarOpcionMenu("-1") == false);
   CHECK(Validador::validarOpcionMenu(" 1") == false);
   CHECK(Validador::validarOpcionMenu("123") == false);
}

// Verifica que la opción vacía (solo Enter) se rechace.
TEST_CASE("Validador_validarOpcionMenu_vacia")
{
   CHECK(Validador::validarOpcionMenu("") == false);
}

// ---------------------------------------------------------------------
// esFinDeSemana (decisión 26)
// ---------------------------------------------------------------------

// Verifica que se reconozcan sábados y domingos, incluidos casos especiales del calendario:
// 2026-11-07 es sábado y 2026-11-08 domingo; 2000-01-01 fue sábado (año múltiplo de 400);
// 2100-02-28 es domingo (año de siglo no bisiesto); 1969-07-20 fue domingo (fecha anterior a 1970,
// donde las funciones de <ctime> fallan en Windows).
TEST_CASE("Validador_esFinDeSemana_sabadosYDomingos")
{
   CHECK(Validador::esFinDeSemana("2026-11-07") == true);   // sábado
   CHECK(Validador::esFinDeSemana("2026-11-08") == true);   // domingo
   CHECK(Validador::esFinDeSemana("2000-01-01") == true);   // sábado
   CHECK(Validador::esFinDeSemana("2100-02-28") == true);   // domingo
   CHECK(Validador::esFinDeSemana("1969-07-20") == true);   // domingo
}

// Verifica que los días de lunes a viernes no se consideren fin de semana, incluidos los límites
// alrededor del fin de semana (viernes 2026-11-06 y lunes 2026-11-09) y fechas cercanas al
// 29 de febrero, donde el ajuste de enero y febrero del algoritmo es delicado.
TEST_CASE("Validador_esFinDeSemana_diasHabiles")
{
   CHECK(Validador::esFinDeSemana("2026-11-06") == false);   // viernes
   CHECK(Validador::esFinDeSemana("2026-11-09") == false);   // lunes
   CHECK(Validador::esFinDeSemana("2026-11-05") == false);   // jueves
   CHECK(Validador::esFinDeSemana("2024-02-29") == false);   // jueves, día bisiesto
   CHECK(Validador::esFinDeSemana("2028-02-29") == false);   // martes, día bisiesto
   CHECK(Validador::esFinDeSemana("2100-03-01") == false);   // lunes, tras un febrero de 28 días
}

// Verifica que una fecha inválida devuelva false: un mes inexistente, el orden invertido
// y el texto vacío no pasan validarFecha, así que no pueden ser fin de semana.
TEST_CASE("Validador_esFinDeSemana_fechasInvalidas")
{
   CHECK(Validador::esFinDeSemana("2026-13-01") == false);
   CHECK(Validador::esFinDeSemana("03-11-2026") == false);
   CHECK(Validador::esFinDeSemana("") == false);
}
