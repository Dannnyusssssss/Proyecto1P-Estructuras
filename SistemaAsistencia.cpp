/***********************************************************************
 * Module:  SistemaAsistencia.cpp
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:28:38
 * Purpose: Implementation of the class SistemaAsistencia
 ***********************************************************************/

#include "Profesor.h"
#include "Asistencia.h"
#include "SistemaAsistencia.h"

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::SistemaAsistencia()
// Purpose:    Implementation of SistemaAsistencia::SistemaAsistencia()
// Return:     
////////////////////////////////////////////////////////////////////////

SistemaAsistencia::SistemaAsistencia()
{
   profesor = NULL;
   asistencia = NULL;
}

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::~SistemaAsistencia()
// Purpose:    Implementation of SistemaAsistencia::~SistemaAsistencia()
// Return:     
////////////////////////////////////////////////////////////////////////

SistemaAsistencia::~SistemaAsistencia()
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::agregarProfesor(Profesor* pro)
// Purpose:    Implementation of SistemaAsistencia::agregarProfesor()
// Parameters:
// - pro
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool SistemaAsistencia::agregarProfesor(Profesor* pro)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::agregarFranja(std::string cedula, FranjaHoraria* fra)
// Purpose:    Implementation of SistemaAsistencia::agregarFranja()
// Parameters:
// - cedula
// - fra
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool SistemaAsistencia::agregarFranja(std::string cedula, FranjaHoraria* fra)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::registrarEntrada(std::string cedula, std::string fecha, Hora* hora)
// Purpose:    Implementation of SistemaAsistencia::registrarEntrada()
// Parameters:
// - cedula
// - fecha
// - hora
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool SistemaAsistencia::registrarEntrada(std::string cedula, std::string fecha, Hora* hora)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::registrarSalida(std::string cedula, std::string fecha, Hora* hora)
// Purpose:    Implementation of SistemaAsistencia::registrarSalida()
// Parameters:
// - cedula
// - fecha
// - hora
// Return:     bool
////////////////////////////////////////////////////////////////////////

bool SistemaAsistencia::registrarSalida(std::string cedula, std::string fecha, Hora* hora)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::mostrarAsistencias()
// Purpose:    Implementation of SistemaAsistencia::mostrarAsistencias()
// Return:     void
////////////////////////////////////////////////////////////////////////

void SistemaAsistencia::mostrarAsistencias(void)
{
   // TODO : implement
}

////////////////////////////////////////////////////////////////////////
// Name:       SistemaAsistencia::mostrarAsistenciaProfesor(std::string cedula)
// Purpose:    Implementation of SistemaAsistencia::mostrarAsistenciaProfesor()
// Parameters:
// - cedula
// Return:     void
////////////////////////////////////////////////////////////////////////

void SistemaAsistencia::mostrarAsistenciaProfesor(std::string cedula)
{
   // TODO : implement
}