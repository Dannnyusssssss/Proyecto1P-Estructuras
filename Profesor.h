/***********************************************************************
 * Module:  Profesor.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:35:39
 * Purpose: Declaration of the class Profesor
 ***********************************************************************/

#if !defined(__Proyecto_Profesor_h)
#define __Proyecto_Profesor_h

class FranjaHoraria;

#include <Persona.h>
#include <FranjaHoraria.h>

class Profesor : public Persona
{
public:
   Profesor();
   Profesor(std::string cedula, std::string nombre, std::string apellido);
   ~Profesor();
   ListaDoble<FranjaHoraria>* getFranjas(void);
   bool agregarFranja(FranjaHoraria* franja);
   FranjaHoraria* buscarFranjaParaEntrada(Hora* hora);
   FranjaHoraria* buscarFranjaParaSalida(Hora* hora);
   void mostrar(void);

   FranjaHoraria** franjaHoraria;

protected:
private:
   ListaDoble<FranjaHoraria>* franjas;


};

#endif