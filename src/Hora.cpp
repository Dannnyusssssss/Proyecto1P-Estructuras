/***********************************************************************
 * Module:  Hora.cpp
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:32:20
 * Purpose: Implementation of the class Hora
 ***********************************************************************/

// Archivo: Hora.cpp
// Contiene la implementación de la clase Hora.

#include "Hora.h"
#include <sstream>
#include <iomanip>

////////////////////////////////////////////////////////////////////////
// Name:       Hora::Hora(int horas, int minutos)
// Purpose:    Implementation of Hora::Hora()
// Parameters:
// - horas
// - minutos
// Return:     
////////////////////////////////////////////////////////////////////////

// Construye la hora a partir de horas y minutos sueltos.
// Si las horas no están entre 0 y 23 o los minutos entre 0 y 59, queda en 00:00.
Hora::Hora(int horas, int minutos)
{
   // Rango inválido: se usa 00:00 como valor seguro en lugar de guardar una hora imposible
   if (horas < 0 || horas > 23 || minutos < 0 || minutos > 59)
   {
      this->minutos = 0;
   }
   else
   {
      // this->minutos es el atributo; minutos (sin this) es el parámetro
      this->minutos = horas * 60 + minutos;
   }
}

////////////////////////////////////////////////////////////////////////
// Name:       Hora::Hora(std::string hhmm)
// Purpose:    Implementation of Hora::Hora()
// Parameters:
// - hhmm
// Return:     
////////////////////////////////////////////////////////////////////////

// Construye la hora a partir de un texto "HH:MM" ya validado por el Validador.
// Aquí no se arma ninguna regex: solo se separan los números con un stringstream.
Hora::Hora(std::string hhmm)
{
   std::istringstream entrada(hhmm);
   int horas = -1;        // valor inválido por defecto, por si la lectura falla
   int mins = -1;         // valor inválido por defecto, por si la lectura falla
   char separador = ' ';  // debe terminar siendo ':'

   // Se leen: número, separador y número. Si algo falla, el stream queda en estado de error
   entrada >> horas >> separador >> mins;

   // Texto ilegible, separador distinto de ':' o valores fuera de rango: queda en 00:00
   if (entrada.fail() || separador != ':' || horas < 0 || horas > 23 || mins < 0 || mins > 59)
   {
      minutos = 0;
   }
   else
   {
      minutos = horas * 60 + mins;
   }
}

////////////////////////////////////////////////////////////////////////
// Name:       Hora::getHoras()
// Purpose:    Implementation of Hora::getHoras()
// Return:     int
////////////////////////////////////////////////////////////////////////

// Devuelve la parte de las horas: la división entera descarta los minutos sobrantes.
int Hora::getHoras(void) const
{
   return minutos / 60;
}

////////////////////////////////////////////////////////////////////////
// Name:       Hora::getMinutos()
// Purpose:    Implementation of Hora::getMinutos()
// Return:     int
////////////////////////////////////////////////////////////////////////

// Devuelve la parte de los minutos: el resto de dividir para 60.
int Hora::getMinutos(void) const
{
   return minutos % 60;
}

////////////////////////////////////////////////////////////////////////
// Name:       Hora::getTotalMinutos()
// Purpose:    Implementation of Hora::getTotalMinutos()
// Return:     int
////////////////////////////////////////////////////////////////////////

// Devuelve los minutos desde la medianoche, tal como se guardan.
int Hora::getTotalMinutos(void) const
{
   return minutos;
}

////////////////////////////////////////////////////////////////////////
// Name:       Hora::esMayorIgual(Hora* otra)
// Purpose:    Implementation of Hora::esMayorIgual()
// Parameters:
// - otra
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Compara esta hora con otra. Solo consulta otra: no la libera ni la guarda.
bool Hora::esMayorIgual(Hora* otra) const
{
   // Sin otra hora no hay con qué comparar
   if (otra == nullptr)
   {
      return false;
   }
   return minutos >= otra->getTotalMinutos();
}

////////////////////////////////////////////////////////////////////////
// Name:       Hora::esMenorIgual(Hora* otra)
// Purpose:    Implementation of Hora::esMenorIgual()
// Parameters:
// - otra
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Compara esta hora con otra. Solo consulta otra: no la libera ni la guarda.
bool Hora::esMenorIgual(Hora* otra) const
{
   // Sin otra hora no hay con qué comparar
   if (otra == nullptr)
   {
      return false;
   }
   return minutos <= otra->getTotalMinutos();
}

////////////////////////////////////////////////////////////////////////
// Name:       Hora::sumarMinutos(int min)
// Purpose:    Implementation of Hora::sumarMinutos()
// Parameters:
// - min
// Return:     Hora*
////////////////////////////////////////////////////////////////////////

// Crea una hora nueva desplazada min minutos. La original no cambia.
// Se usa para calcular el fin de las ventanas: horaInicio + 10 y horaFinal + 10.
Hora* Hora::sumarMinutos(int min) const
{
   const int minutosPorDia = 24 * 60;   // 1440 minutos tiene un día

   // El primer % deja el valor entre -1439 y 1439; sumar 1440 y aplicar % otra vez
   // lo lleva siempre a 0..1439, así funciona también con min negativo.
   // Ejemplo: 23:55 (1435) + 10 = 1445 -> 1445 % 1440 = 5 -> 00:05
   int total = ((minutos + min) % minutosPorDia + minutosPorDia) % minutosPorDia;

   // Se devuelve un objeto nuevo en el heap; quien lo recibe es su dueño y debe hacer delete
   return new Hora(total / 60, total % 60);
}

////////////////////////////////////////////////////////////////////////
// Name:       Hora::toString()
// Purpose:    Implementation of Hora::toString()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

// Arma el texto "HH:MM". setw(2) con setfill('0') agrega el cero a la izquierda (7 -> "07").
std::string Hora::toString(void) const
{
   std::ostringstream salida;
   salida << std::setw(2) << std::setfill('0') << getHoras()
          << ":"
          << std::setw(2) << std::setfill('0') << getMinutos();
   return salida.str();
}
