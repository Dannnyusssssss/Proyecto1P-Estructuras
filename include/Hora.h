/***********************************************************************
 * Module:  Hora.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:32:20
 * Purpose: Declaration of the class Hora
 ***********************************************************************/

// Archivo: Hora.h
// Contiene la declaración de la clase Hora, que representa una hora del día
// en formato de 24 horas y permite compararla y desplazarla en minutos.

#if !defined(__Proyecto_Hora_h)
#define __Proyecto_Hora_h

#include <string>

// Clase Hora
// Representa una hora del día (00:00 a 23:59) guardada como minutos desde la medianoche.
// Guardarla así simplifica las comparaciones de las ventanas de 10 minutos:
// comparar dos horas es comparar dos enteros.
class Hora
{
public:
   // Crea la hora a partir de horas (0-23) y minutos (0-59).
   // Si algún valor está fuera de rango, la hora queda en 00:00.
   Hora(int horas, int minutos);

   // Crea la hora a partir de un texto "HH:MM".
   // Supone que el texto ya fue validado con Validador::validarHora.
   // Si aun así no se puede interpretar o está fuera de rango, la hora queda en 00:00.
   Hora(std::string hhmm);

   // Devuelve la parte de las horas (0-23).
   int getHoras(void) const;

   // Devuelve la parte de los minutos (0-59), no el total desde la medianoche.
   int getMinutos(void) const;

   // Devuelve el total de minutos desde la medianoche (0-1439).
   int getTotalMinutos(void) const;

   // Devuelve true si esta hora es mayor o igual que otra.
   // Si otra es nullptr devuelve false. No libera otra.
   bool esMayorIgual(Hora* otra) const;

   // Devuelve true si esta hora es menor o igual que otra.
   // Si otra es nullptr devuelve false. No libera otra.
   bool esMenorIgual(Hora* otra) const;

   // Devuelve una hora NUEVA (creada con new) que es esta hora más min minutos.
   // No modifica la hora original. Da la vuelta a la medianoche (23:55 + 10 = 00:05)
   // y admite valores negativos. Quien llama debe liberar la hora devuelta con delete.
   Hora* sumarMinutos(int min) const;

   // Devuelve la hora como texto "HH:MM", con cero a la izquierda (por ejemplo, "07:05").
   std::string toString(void) const;

protected:
private:
   int minutos;   // minutos transcurridos desde la medianoche (0 a 1439)


};

#endif
