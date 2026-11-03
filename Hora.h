/***********************************************************************
 * Module:  Hora.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:32:20
 * Purpose: Declaration of the class Hora
 ***********************************************************************/

#if !defined(__Proyecto_Hora_h)
#define __Proyecto_Hora_h

class Hora
{
public:
   Hora(int horas, int minutos);
   Hora(std::string hhmm);
   int getHoras(void);
   int getMinutos(void);
   int getTotalMinutos(void);
   bool esMayorIgual(Hora* otra);
   bool esMenorIgual(Hora* otra);
   Hora* sumarMinutos(int min);
   std::string toString(void);

protected:
private:
   int minutos;


};

#endif