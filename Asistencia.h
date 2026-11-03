/***********************************************************************
 * Module:  Asistencia.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:35:39
 * Purpose: Declaration of the class Asistencia
 ***********************************************************************/

#if !defined(__Proyecto_Asistencia_h)
#define __Proyecto_Asistencia_h

class Hora;
class Profesor;

#include <IMostrable.h>
#include <IIdentificable.h>

class Asistencia : public IMostrable, public IIdentificable
{
public:
   Asistencia();
   Asistencia(std::string cedulaProfesor, std::string fecha, Hora* horaInicioFranja);
   ~Asistencia();
   std::string getCedulaProfesor(void);
   std::string getFecha(void);
   Hora* getHoraEntrada(void);
   void setHoraEntrada(Hora* newHoraEntrada);
   Hora* getHoraSalida(void);
   void setHoraSalida(Hora* newHoraSalida);
   int getEstado(void);
   bool estaSinRegistro(void);
   bool tieneEntrada(void);
   Hora* getHoraInicioFranja(void);
   void mostrar(void);
   bool estaCompleta(void);
   void registrarEntrada(Hora* hora);
   void registrarSalida(Hora* hora);
   std::string getClave(void);

   Hora** hora;
   Profesor* profesor;

protected:
private:
   std::string cedulaProfesor;
   std::string fecha;
   Hora* horaInicioFranja;
   Hora* horaEntrada;
   Hora* horaSalida;
   int estado;


};

#endif