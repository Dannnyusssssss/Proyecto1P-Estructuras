/***********************************************************************
 * Module:  FranjaHoraria.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:31:16
 * Purpose: Declaration of the class FranjaHoraria
 ***********************************************************************/

#if !defined(__Proyecto_FranjaHoraria_h)
#define __Proyecto_FranjaHoraria_h

class Hora;

#include <IMostrable.h>
#include <IIdentificable.h>

class FranjaHoraria : public IMostrable, public IIdentificable
{
public:
   FranjaHoraria();
   FranjaHoraria(Hora * horaInicio, Hora* horaFinal, std::string aula, std::string cedulaProfesor);
   ~FranjaHoraria();
   Hora* getHoraInicio(void);
   void setHoraInicio(Hora* newHoraInicio);
   Hora* getHoraFinal(void);
   void setHoraFinal(Hora* newHoraFinal);
   std::string getAula(void);
   void setAula(std::string newAula);
   std::string getCedulaProfesor(void);
   void setCedulaProfesor(std::string newCedulaProfesor);
   bool puedeRegistrarEntrada(Hora* hora);
   bool puedeRegistrarSalida(Hora* hora);
   std::string getClave(void);
   void mostrar(void);

   Hora* hora[2];

protected:
private:
   Hora* horaInicio;
   Hora* horaFinal;
   std::string aula;
   std::string cedulaProfesor;


};

#endif