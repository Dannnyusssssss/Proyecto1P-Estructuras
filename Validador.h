/***********************************************************************
 * Module:  Validador.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 9:47:10
 * Purpose: Declaration of the class Validador
 ***********************************************************************/

#if !defined(__Proyecto_Validador_h)
#define __Proyecto_Validador_h

class Validador
{
public:
   static bool validarCedula(std::string cedula);
   static bool validarNombre(std::string nombre);
   static bool validarHora(std::string horaIngresada);
   static bool validarFecha(std::string fecha);
   static bool validarAula(std::string aula);
   static bool validarOpcionMenu(std::string opcion);

protected:
private:

};

#endif