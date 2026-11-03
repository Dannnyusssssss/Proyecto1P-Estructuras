/***********************************************************************
 * Module:  SistemaAsistencia.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:28:38
 * Purpose: Declaration of the class SistemaAsistencia
 ***********************************************************************/

#if !defined(__Proyecto_SistemaAsistencia_h)
#define __Proyecto_SistemaAsistencia_h

class Profesor;
class Asistencia;

#include <Profesor.h>
#include <FranjaHoraria.h>
#include <Hora.h>

class SistemaAsistencia
{
public:
   SistemaAsistencia();
   ~SistemaAsistencia();
   bool agregarProfesor(Profesor* pro);
   bool agregarFranja(std::string cedula, FranjaHoraria* fra);
   bool registrarEntrada(std::string cedula, std::string fecha, Hora* hora);
   bool registrarSalida(std::string cedula, std::string fecha, Hora* hora);
   void mostrarAsistencias(void);
   void mostrarAsistenciaProfesor(std::string cedula);

   Profesor** profesor;
   Asistencia** asistencia;

protected:
private:
   ListaDoble<Profesor>* profesores;
   ListaDoble<Asistencia>* asistencias;


};

#endif