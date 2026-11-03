/***********************************************************************
 * Module:  Menu.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 11:22:21
 * Purpose: Declaration of the class Menu
 ***********************************************************************/

// Archivo: Menu.h
// Contiene la declaración de la clase Menu, la interfaz de consola del sistema.

#if !defined(__Proyecto_Menu_h)
#define __Proyecto_Menu_h

class SistemaAsistencia;

#include "SistemaAsistencia.h"
#include "Hora.h"
#include <string>

// Clase Menu
// Interfaz de consola: muestra los menús, lee y valida lo que escribe el usuario
// (con el Validador) y llama al SistemaAsistencia.
// Es la ÚNICA clase que consulta el reloj del sistema (<ctime>), cuando el usuario presiona
// Enter al pedir la fecha o la hora. Así el resto del sistema se puede probar con cualquier hora.
// Es dueña del SistemaAsistencia y lo libera al destruirse.
class Menu
{
public:
   // Crea el menú y su SistemaAsistencia vacío.
   Menu();

   // Libera el SistemaAsistencia (y con él todos los profesores, franjas y asistencias).
   ~Menu();

   // Copia bloqueada: una copia compartiría el mismo sistema y al destruirse
   // ambos menús harían delete dos veces de la misma memoria.
   Menu(const Menu&) = delete;
   Menu& operator=(const Menu&) = delete;

   // Muestra el menú principal y atiende las opciones hasta que el usuario elige salir
   // (o se termina la entrada estándar).
   void iniciar(void);

protected:
private:
   // Menú del administrador: registrar profesores, asignar franjas y consultar.
   void menuAdministrador(void);

   // Menú del profesor: registrar entrada, registrar salida y ver sus asistencias.
   void menuProfesor(void);

   // Pide una cédula hasta que sea válida (Validador::validarCedula) y la devuelve.
   // Devuelve "" si se termina la entrada estándar.
   std::string leerCedula(void);

   // Muestra "Ingrese la hora (HH:MM) o presione Enter para usar la hora actual".
   // Con Enter usa la hora del sistema; si se escribe una hora, la valida y repite hasta que sea válida.
   // Devuelve un Hora* NUEVO que quien llama debe liberar, o nullptr si se termina la entrada.
   Hora* leerHora(void);

   // Muestra "Ingrese la fecha (AAAA-MM-DD) o presione Enter para usar la fecha actual".
   // Con Enter usa la fecha del sistema; si se escribe una fecha, la valida y repite hasta que sea válida.
   // Devuelve "" si se termina la entrada estándar.
   std::string leerFecha(void);

   SistemaAsistencia* sistema;   // sistema con los profesores y asistencias (el menú es su dueño)


};

#endif
