// Archivo: test_Hora.cpp
// Pruebas de unidad de la clase Hora: constructores, getters, comparaciones,
// suma de minutos, conversión a texto, límites del día y valores inválidos.

#include "doctest.h"
#include "Hora.h"

// ---------------------------------------------------------------------
// Constructor Hora(int, int)
// ---------------------------------------------------------------------

// Verifica que 07:10 se guarde como 7*60 + 10 = 430 minutos desde la medianoche.
TEST_CASE("Hora_constructorEnteros_valido")
{
   Hora* h = new Hora(7, 10);
   CHECK(h->getTotalMinutos() == 430);
   CHECK(h->getHoras() == 7);
   CHECK(h->getMinutos() == 10);
   delete h;
}

// Verifica los límites del día: 00:00 es el minuto 0 y 23:59 es el minuto 1439.
TEST_CASE("Hora_constructorEnteros_limites")
{
   Hora* inicio = new Hora(0, 0);
   Hora* fin = new Hora(23, 59);
   CHECK(inicio->getTotalMinutos() == 0);
   CHECK(fin->getTotalMinutos() == 1439);
   CHECK(fin->getHoras() == 23);
   CHECK(fin->getMinutos() == 59);
   delete inicio;
   delete fin;
}

// Verifica que valores fuera de rango dejen la hora en 00:00 (decisión 6):
// 24 horas, 60 minutos y negativos no son horas reales del día.
TEST_CASE("Hora_constructorEnteros_fueraDeRango")
{
   Hora* h1 = new Hora(24, 0);
   Hora* h2 = new Hora(7, 60);
   Hora* h3 = new Hora(-1, 10);
   Hora* h4 = new Hora(7, -5);
   CHECK(h1->getTotalMinutos() == 0);
   CHECK(h2->getTotalMinutos() == 0);
   CHECK(h3->getTotalMinutos() == 0);
   CHECK(h4->getTotalMinutos() == 0);
   delete h1;
   delete h2;
   delete h3;
   delete h4;
}

// ---------------------------------------------------------------------
// Constructor Hora(std::string)
// ---------------------------------------------------------------------

// Verifica que el texto "07:10" se separe en 7 horas y 10 minutos.
TEST_CASE("Hora_constructorTexto_valido")
{
   Hora* h = new Hora("07:10");
   CHECK(h->getHoras() == 7);
   CHECK(h->getMinutos() == 10);
   CHECK(h->getTotalMinutos() == 430);
   delete h;
}

// Verifica los límites del día escritos como texto.
TEST_CASE("Hora_constructorTexto_limites")
{
   Hora* inicio = new Hora("00:00");
   Hora* fin = new Hora("23:59");
   CHECK(inicio->getTotalMinutos() == 0);
   CHECK(fin->getTotalMinutos() == 1439);
   delete inicio;
   delete fin;
}

// Verifica que un texto que no se puede interpretar (o fuera de rango) quede en 00:00.
// Normalmente no llega aquí porque el Validador lo rechaza antes, pero la clase no debe romperse.
TEST_CASE("Hora_constructorTexto_invalido")
{
   Hora* letras = new Hora("ab:cd");
   Hora* vacia = new Hora("");
   Hora* fueraRango = new Hora("24:00");
   Hora* sinSeparador = new Hora("0710");
   CHECK(letras->getTotalMinutos() == 0);
   CHECK(vacia->getTotalMinutos() == 0);
   CHECK(fueraRango->getTotalMinutos() == 0);
   CHECK(sinSeparador->getTotalMinutos() == 0);
   delete letras;
   delete vacia;
   delete fueraRango;
   delete sinSeparador;
}

// ---------------------------------------------------------------------
// Getters
// ---------------------------------------------------------------------

// Verifica que getHoras y getMinutos separen correctamente 13:05 (785 minutos):
// getMinutos devuelve la parte de minutos (5), no el total.
TEST_CASE("Hora_getHorasGetMinutos_separacion")
{
   Hora* h = new Hora(13, 5);
   CHECK(h->getHoras() == 13);
   CHECK(h->getMinutos() == 5);
   CHECK(h->getTotalMinutos() == 785);
   delete h;
}

// ---------------------------------------------------------------------
// esMayorIgual
// ---------------------------------------------------------------------

// Verifica que dos horas iguales sean "mayor o igual" (el extremo se incluye).
TEST_CASE("Hora_esMayorIgual_horasIguales")
{
   Hora* a = new Hora(7, 0);
   Hora* b = new Hora(7, 0);
   CHECK(a->esMayorIgual(b) == true);
   delete a;
   delete b;
}

// Verifica que 07:11 sea mayor o igual que 07:10.
TEST_CASE("Hora_esMayorIgual_horaMayor")
{
   Hora* a = new Hora(7, 11);
   Hora* b = new Hora(7, 10);
   CHECK(a->esMayorIgual(b) == true);
   delete a;
   delete b;
}

// Verifica que 06:59 no sea mayor o igual que 07:00.
TEST_CASE("Hora_esMayorIgual_horaMenor")
{
   Hora* a = new Hora(6, 59);
   Hora* b = new Hora(7, 0);
   CHECK(a->esMayorIgual(b) == false);
   delete a;
   delete b;
}

// Verifica que comparar con nullptr devuelva false en lugar de fallar.
TEST_CASE("Hora_esMayorIgual_nulo")
{
   Hora* a = new Hora(7, 0);
   CHECK(a->esMayorIgual(nullptr) == false);
   delete a;
}

// ---------------------------------------------------------------------
// esMenorIgual
// ---------------------------------------------------------------------

// Verifica que dos horas iguales sean "menor o igual" (el extremo se incluye).
TEST_CASE("Hora_esMenorIgual_horasIguales")
{
   Hora* a = new Hora(7, 10);
   Hora* b = new Hora(7, 10);
   CHECK(a->esMenorIgual(b) == true);
   delete a;
   delete b;
}

// Verifica que 07:11 no sea menor o igual que 07:10 (caso de llegada tarde).
TEST_CASE("Hora_esMenorIgual_horaMayor")
{
   Hora* a = new Hora(7, 11);
   Hora* b = new Hora(7, 10);
   CHECK(a->esMenorIgual(b) == false);
   delete a;
   delete b;
}

// Verifica que 07:05 sea menor o igual que 07:10.
TEST_CASE("Hora_esMenorIgual_horaMenor")
{
   Hora* a = new Hora(7, 5);
   Hora* b = new Hora(7, 10);
   CHECK(a->esMenorIgual(b) == true);
   delete a;
   delete b;
}

// Verifica que comparar con nullptr devuelva false en lugar de fallar.
TEST_CASE("Hora_esMenorIgual_nulo")
{
   Hora* a = new Hora(7, 0);
   CHECK(a->esMenorIgual(nullptr) == false);
   delete a;
}

// ---------------------------------------------------------------------
// sumarMinutos
// ---------------------------------------------------------------------

// Verifica que 07:00 + 10 = 07:10 y que el resultado sea un objeto nuevo,
// dejando la hora original sin cambios.
TEST_CASE("Hora_sumarMinutos_valido")
{
   Hora* original = new Hora(7, 0);
   Hora* resultado = original->sumarMinutos(10);
   CHECK(resultado != original);                    // es otro objeto
   CHECK(resultado->toString() == "07:10");
   CHECK(original->toString() == "07:00");          // la original no cambió
   delete resultado;
   delete original;
}

// Verifica que sumar 0 minutos dé la misma hora pero en un objeto distinto.
TEST_CASE("Hora_sumarMinutos_cero")
{
   Hora* original = new Hora(9, 0);
   Hora* resultado = original->sumarMinutos(0);
   CHECK(resultado != original);
   CHECK(resultado->getTotalMinutos() == original->getTotalMinutos());
   delete resultado;
   delete original;
}

// Verifica que la suma cambie de hora correctamente: 08:55 + 10 = 09:05.
TEST_CASE("Hora_sumarMinutos_cambioDeHora")
{
   Hora* original = new Hora(8, 55);
   Hora* resultado = original->sumarMinutos(10);
   CHECK(resultado->toString() == "09:05");
   delete resultado;
   delete original;
}

// Verifica la vuelta a la medianoche (decisión 6): 23:55 + 10 = 00:05.
TEST_CASE("Hora_sumarMinutos_vueltaMedianoche")
{
   Hora* original = new Hora(23, 55);
   Hora* resultado = original->sumarMinutos(10);
   CHECK(resultado->toString() == "00:05");
   delete resultado;
   delete original;
}

// Verifica que restar (minutos negativos) también dé la vuelta: 00:05 - 10 = 23:55.
TEST_CASE("Hora_sumarMinutos_negativo")
{
   Hora* original = new Hora(0, 5);
   Hora* resultado = original->sumarMinutos(-10);
   CHECK(resultado->toString() == "23:55");
   delete resultado;
   delete original;
}

// ---------------------------------------------------------------------
// toString
// ---------------------------------------------------------------------

// Verifica que se agregue el cero a la izquierda en horas y minutos de un dígito.
TEST_CASE("Hora_toString_ceroIzquierda")
{
   Hora* h = new Hora(7, 5);
   CHECK(h->toString() == "07:05");
   delete h;
}

// Verifica el formato en los límites del día.
TEST_CASE("Hora_toString_limites")
{
   Hora* inicio = new Hora(0, 0);
   Hora* fin = new Hora(23, 59);
   CHECK(inicio->toString() == "00:00");
   CHECK(fin->toString() == "23:59");
   delete inicio;
   delete fin;
}

// Verifica que convertir el texto a Hora y de vuelta a texto dé el mismo valor.
TEST_CASE("Hora_toString_idaYVuelta")
{
   Hora* h = new Hora("11:10");
   CHECK(h->toString() == "11:10");
   delete h;
}
