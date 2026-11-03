/***********************************************************************
 * Module:  Profesor.cpp
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:35:39
 * Purpose: Implementation of the class Profesor
 ***********************************************************************/

#include "FranjaHoraria.h"
#include "Profesor.h"

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::Profesor()
// Purpose:    Implementation of Profesor::Profesor()
// Return:     
////////////////////////////////////////////////////////////////////////

Profesor::Profesor()
{
   franjaHoraria = NULL;
}

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::Profesor(std::string cedula, std::string nombre, std::string apellido)
// Purpose:    Implementation of Profesor::Profesor()
// Parameters:
// - cedula
// - nombre
// - apellido
// Return:     
////////////////////////////////////////////////////////////////////////

Profesor::Profesor(std::string cedula, std::string nombre, std::string apellido)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::~Profesor()
// Purpose:    Implementation of Profesor::~Profesor()
// Return:     
////////////////////////////////////////////////////////////////////////

Profesor::~Profesor()
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::getFranjas()
// Purpose:    Implementation of Profesor::getFranjas()
// Return:     ListaDoble<FranjaHoraria>*
////////////////////////////////////////////////////////////////////////

ListaDoble<FranjaHoraria>* Profesor::getFranjas(void)
{
   return franjas;
}

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::agregarFranja(FranjaHoraria* franja)
// Purpose:    Implementation of Profesor::agregarFranja()
// Parameters:
// - franja
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool Profesor::agregarFranja(FranjaHoraria* franja)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::buscarFranjaParaEntrada(Hora* hora)
// Purpose:    Implementation of Profesor::buscarFranjaParaEntrada()
// Parameters:
// - hora
// Return:     FranjaHoraria*
////////////////////////////////////////////////////////////////////////

FranjaHoraria* Profesor::buscarFranjaParaEntrada(Hora* hora)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::buscarFranjaParaSalida(Hora* hora)
// Purpose:    Implementation of Profesor::buscarFranjaParaSalida()
// Parameters:
// - hora
// Return:     FranjaHoraria*
////////////////////////////////////////////////////////////////////////

FranjaHoraria* Profesor::buscarFranjaParaSalida(Hora* hora)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       Profesor::mostrar()
// Purpose:    Implementation of Profesor::mostrar()
// Return:     void
////////////////////////////////////////////////////////////////////////

void Profesor::mostrar(void)
{
   // TODO : implement
}