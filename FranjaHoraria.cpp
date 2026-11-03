/***********************************************************************
 * Module:  FranjaHoraria.cpp
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:31:16
 * Purpose: Implementation of the class FranjaHoraria
 ***********************************************************************/

#include "Hora.h"
#include "FranjaHoraria.h"

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::FranjaHoraria()
// Purpose:    Implementation of FranjaHoraria::FranjaHoraria()
// Return:     
////////////////////////////////////////////////////////////////////////

FranjaHoraria::FranjaHoraria()
{
   hora = NULL;
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

FranjaHoraria::FranjaHoraria(Hora * horaInicio, Hora* horaFinal, std::string aula, std::string cedulaProfesor)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::~FranjaHoraria()
// Purpose:    Implementation of FranjaHoraria::~FranjaHoraria()
// Return:     
////////////////////////////////////////////////////////////////////////

FranjaHoraria::~FranjaHoraria()
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::getHoraInicio()
// Purpose:    Implementation of FranjaHoraria::getHoraInicio()
// Return:     Hora*
////////////////////////////////////////////////////////////////////////

Hora* FranjaHoraria::getHoraInicio(void)
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

void FranjaHoraria::setHoraInicio(Hora* newHoraInicio)
{
   horaInicio = newHoraInicio;
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::getHoraFinal()
// Purpose:    Implementation of FranjaHoraria::getHoraFinal()
// Return:     Hora*
////////////////////////////////////////////////////////////////////////

Hora* FranjaHoraria::getHoraFinal(void)
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

void FranjaHoraria::setHoraFinal(Hora* newHoraFinal)
{
   horaFinal = newHoraFinal;
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::getAula()
// Purpose:    Implementation of FranjaHoraria::getAula()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

std::string FranjaHoraria::getAula(void)
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

void FranjaHoraria::setAula(std::string newAula)
{
   aula = newAula;
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::getCedulaProfesor()
// Purpose:    Implementation of FranjaHoraria::getCedulaProfesor()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

std::string FranjaHoraria::getCedulaProfesor(void)
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

bool FranjaHoraria::puedeRegistrarEntrada(Hora* hora)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::puedeRegistrarSalida(Hora* hora)
// Purpose:    Implementation of FranjaHoraria::puedeRegistrarSalida()
// Parameters:
// - hora
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool FranjaHoraria::puedeRegistrarSalida(Hora* hora)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::getClave()
// Purpose:    Implementation of FranjaHoraria::getClave()
// Return:     std::string
////////////////////////////////////////////////////////////////////////

std::string FranjaHoraria::getClave(void)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       FranjaHoraria::mostrar()
// Purpose:    Implementation of FranjaHoraria::mostrar()
// Return:     void
////////////////////////////////////////////////////////////////////////

void FranjaHoraria::mostrar(void)
{
   // TODO : implement
}