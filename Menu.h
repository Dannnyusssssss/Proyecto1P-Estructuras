/***********************************************************************
 * Module:  Menu.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 11:22:21
 * Purpose: Declaration of the class Menu
 ***********************************************************************/

#if !defined(__Proyecto_Menu_h)
#define __Proyecto_Menu_h

class SistemaAsistencia;

#include <SistemaAsistencia.h>
#include <Hora.h>

class Menu
{
public:
   Menu();
   ~Menu();
   void iniciar(void);

   SistemaAsistencia* sistemaAsistencia;

protected:
private:
   void menuAdministrador(void);
   void menuProfesor(void);
   std::string leerCedula(void);
   Hora* leerHora(void);
   std::string leerFecha(void);

   SistemaAsistencia* sistema;


};

#endif