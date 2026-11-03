/***********************************************************************
 * Module:  FranjaHoraria.cpp
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:31:16
 * Purpose: Implementation of the class FranjaHoraria
 ***********************************************************************/

// Archivo: FranjaHoraria.cpp
// Contiene la implementación de la clase FranjaHoraria.

#include "Hora.h"
#include "FranjaHoraria.h"
#include <iostream>

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::FranjaHoraria()
// Purpose:    Implementation of FranjaHoraria::FranjaHoraria()
// Return:
////////////////////////////////////////////////////////////////////////

// Franja vacía: los punteros se dejan en nullptr para que el destructor pueda hacer
// delete sin riesgo (delete de nullptr no hace nada).
FranjaHoraria::FranjaHoraria()
   : horaInicio(nullptr), horaFinal(nullptr), aula(""), cedulaProfesor("")
{
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::FranjaHoraria(Hora * horaInicio, Hora* horaFinal, std::string aula, std::string cedulaProfesor)
// Purpose:    Implementation of FranjaHoraria::FranjaHoraria()
// Parameters:
// - horaInicio
// - horaFinal
// - aula
// - cedulaProfesor
// Return:
////////////////////////////////////////////////////////////////////////

// Se guardan los MISMOS punteros recibidos (sin new): desde aquí la franja es su dueña.
FranjaHoraria::FranjaHoraria(Hora * horaInicio, Hora* horaFinal, std::string aula, std::string cedulaProfesor)
   : horaInicio(horaInicio), horaFinal(horaFinal), aula(aula), cedulaProfesor(cedulaProfesor)
{
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::~FranjaHoraria()
// Purpose:    Implementation of FranjaHoraria::~FranjaHoraria()
// Return:
////////////////////////////////////////////////////////////////////////

// Libera las dos horas, de las que la franja es dueña.
FranjaHoraria::~FranjaHoraria()
{
   delete horaInicio;
   delete horaFinal;
   horaInicio = nullptr;
   horaFinal = nullptr;
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::getHoraInicio()
// Purpose:    Implementation of FranjaHoraria::getHoraInicio()
// Return:     Hora*
////////////////////////////////////////////////////////////////////////

// Devuelve la hora de inicio sin transferir su propiedad.
Hora* FranjaHoraria::getHoraInicio(void) const
{
   return horaInicio;
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::setHoraInicio(Hora* newHoraInicio)
// Purpose:    Implementation of FranjaHoraria::setHoraInicio()
// Parameters:
// - newHoraInicio
// Return:     void
////////////////////////////////////////////////////////////////////////

// Reemplaza la hora de inicio liberando la anterior.
void FranjaHoraria::setHoraInicio(Hora* newHoraInicio)
{
   // Si es el mismo puntero, borrarlo dejaría a la franja apuntando a memoria liberada
   if (newHoraInicio == horaInicio)
   {
      return;
   }
   delete horaInicio;            // se libera la hora anterior (la franja era su dueña)
   horaInicio = newHoraInicio;   // y se toma posesión de la nueva
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::getHoraFinal()
// Purpose:    Implementation of FranjaHoraria::getHoraFinal()
// Return:     Hora*
////////////////////////////////////////////////////////////////////////

// Devuelve la hora de fin sin transferir su propiedad.
Hora* FranjaHoraria::getHoraFinal(void) const
{
   return horaFinal;
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::setHoraFinal(Hora* newHoraFinal)
// Purpose:    Implementation of FranjaHoraria::setHoraFinal()
// Parameters:
// - newHoraFinal
// Return:     void
////////////////////////////////////////////////////////////////////////

// Reemplaza la hora de fin liberando la anterior.
void FranjaHoraria::setHoraFinal(Hora* newHoraFinal)
{
   // Si es el mismo puntero, borrarlo dejaría a la franja apuntando a memoria liberada
   if (newHoraFinal == horaFinal)
   {
      return;
   }
   delete horaFinal;           // se libera la hora anterior (la franja era su dueña)
   horaFinal = newHoraFinal;   // y se toma posesión de la nueva
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::getAula()
// Purpose:    Implementation of FranjaHoraria::getAula()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

// Devuelve el aula guardada.
std::string FranjaHoraria::getAula(void) const
{
   return aula;
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::setAula(std::string newAula)
// Purpose:    Implementation of FranjaHoraria::setAula()
// Parameters:
// - newAula
// Return:     void
////////////////////////////////////////////////////////////////////////

// Reemplaza el aula.
void FranjaHoraria::setAula(std::string newAula)
{
   aula = newAula;
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::getCedulaProfesor()
// Purpose:    Implementation of FranjaHoraria::getCedulaProfesor()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

// Devuelve la cédula del profesor dueño de la franja.
std::string FranjaHoraria::getCedulaProfesor(void) const
{
   return cedulaProfesor;
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::setCedulaProfesor(std::string newCedulaProfesor)
// Purpose:    Implementation of FranjaHoraria::setCedulaProfesor()
// Parameters:
// - newCedulaProfesor
// Return:     void
////////////////////////////////////////////////////////////////////////

// Reemplaza la cédula del profesor.
void FranjaHoraria::setCedulaProfesor(std::string newCedulaProfesor)
{
   cedulaProfesor = newCedulaProfesor;
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::puedeRegistrarEntrada(Hora* hora)
// Purpose:    Implementation of FranjaHoraria::puedeRegistrarEntrada()
// Parameters:
// - hora
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Comprueba si hora está en la ventana de entrada [horaInicio, horaInicio + 10].
bool FranjaHoraria::puedeRegistrarEntrada(Hora* hora) const
{
   // Sin hora a consultar o sin hora de inicio no hay ventana que comparar
   if (hora == nullptr || horaInicio == nullptr)
   {
      return false;
   }

   // Fin de la ventana: horaInicio + 10 minutos. sumarMinutos crea un objeto nuevo
   // del que este método es dueño, por eso se libera antes de salir
   Hora* limite = horaInicio->sumarMinutos(10);

   // Ambos extremos se incluyen: 07:00 y 07:10 son válidas, 06:59 y 07:11 no
   bool dentro = hora->esMayorIgual(horaInicio)   // no antes del inicio
              && hora->esMenorIgual(limite);      // ni después de los 10 minutos

   delete limite;   // hora NO se libera: solo se consultó
   return dentro;
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::puedeRegistrarSalida(Hora* hora)
// Purpose:    Implementation of FranjaHoraria::puedeRegistrarSalida()
// Parameters:
// - hora
// Return:     bool
////////////////////////////////////////////////////////////////////////

// Comprueba si hora está en la ventana de salida [horaFinal, horaFinal + 10].
bool FranjaHoraria::puedeRegistrarSalida(Hora* hora) const
{
   // Sin hora a consultar o sin hora de fin no hay ventana que comparar
   if (hora == nullptr || horaFinal == nullptr)
   {
      return false;
   }

   // Fin de la ventana: horaFinal + 10 minutos (objeto nuevo, se libera antes de salir)
   Hora* limite = horaFinal->sumarMinutos(10);

   // Ambos extremos se incluyen: 09:00 y 09:10 son válidas, 08:59 y 09:11 no
   bool dentro = hora->esMayorIgual(horaFinal)    // no antes de que termine la clase
              && hora->esMenorIgual(limite);      // ni después de los 10 minutos

   delete limite;   // hora NO se libera: solo se consultó
   return dentro;
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::getClave()
// Purpose:    Implementation of FranjaHoraria::getClave()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

// La cédula y la hora de inicio identifican la franja: un profesor no puede tener
// dos franjas que empiecen a la misma hora.
std::string FranjaHoraria::getClave(void) const
{
   // Sin hora de inicio solo queda la parte de la cédula
   if (horaInicio == nullptr)
   {
      return cedulaProfesor + "-";
   }
   return cedulaProfesor + "-" + horaInicio->toString();
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::mostrar()
// Purpose:    Implementation of FranjaHoraria::mostrar()
// Return:     void
////////////////////////////////////////////////////////////////////////

// Imprime el horario y el aula en una línea.
void FranjaHoraria::mostrar(void) const
{
   // Si falta una hora se muestra "--:--" en lugar de acceder a un puntero nulo
   std::string inicio = (horaInicio != nullptr) ? horaInicio->toString() : "--:--";
   std::string fin = (horaFinal != nullptr) ? horaFinal->toString() : "--:--";
   std::cout << inicio << " - " << fin << " | Aula: " << aula << std::endl;
}
