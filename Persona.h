/***********************************************************************
 * Module:  Persona.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 9:04:15
 * Purpose: Declaration of the class Persona
 ***********************************************************************/

#if !defined(__Proyecto_Persona_h)
#define __Proyecto_Persona_h

#include <IMostrable.h>
#include <IIdentificable.h>

class Persona : public IMostrable, public IIdentificable
{
public:
   Persona();
   Persona(std::string cedula, std::string nombre, std::string apellido);
   std::string getCedula(void);
   void setCedula(std::string newCedula);
   std::string getNombre(void);
   void setNombre(std::string newNombre);
   std::string getApellido(void);
   void setApellido(std::string newApellido);
   std::string getClave(void);
   void mostrar(void);

protected:
private:
   std::string cedula;
   std::string nombre;
   std::string apellido;


};

#endif