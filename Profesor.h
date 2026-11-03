/***********************************************************************
 * Module:  Profesor.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:35:39
 * Purpose: Declaration of the class Profesor
 ***********************************************************************/

// Archivo: Profesor.h
// Contiene la declaración de la clase Profesor, una Persona con franjas horarias asignadas.

#if !defined(__Proyecto_Profesor_h)
#define __Proyecto_Profesor_h

class FranjaHoraria;

#include "Persona.h"
#include "FranjaHoraria.h"
#include "ListaDoble.h"
#include "Hora.h"
#include <string>

// Clase Profesor
// Representa a un profesor: una Persona con su lista de franjas horarias.
// Responsabilidades:
//  - Aceptar solo franjas válidas: una de las 7 predefinidas, de su propia cédula,
//    sin duplicados y sin cruces con sus otras franjas.
//  - Encontrar la franja que corresponde a una hora de entrada o de salida.
// Es dueño de su lista de franjas (y, a través de ella, de cada franja y sus horas).
class Profesor : public Persona
{
public:
   // Crea un profesor sin datos y con la lista de franjas vacía.
   Profesor();

   // Crea un profesor con sus datos (sin validar) y la lista de franjas vacía.
   Profesor(std::string cedula, std::string nombre, std::string apellido);

   // Libera la lista de franjas, que a su vez libera cada franja y sus horas.
   ~Profesor() override;

   // Copia bloqueada: una copia compartiría la misma lista de franjas y al destruirse
   // ambos profesores se haría delete dos veces de la misma memoria.
   Profesor(const Profesor&) = delete;
   Profesor& operator=(const Profesor&) = delete;

   // Devuelve la lista de franjas. El profesor sigue siendo su dueño: no liberarla.
   ListaDoble<FranjaHoraria>* getFranjas(void) const;

   // Agrega la franja si es válida. Rechaza (devuelve false) si:
   //  - franja, su hora de inicio o su hora de fin son nullptr,
   //  - la cédula de la franja no es la de este profesor,
   //  - no es una de las 7 franjas predefinidas (07, 09, 11, 13, 15, 17 o 19 con 2 h de duración),
   //  - el profesor ya tiene esa franja (duplicada),
   //  - se cruza en horario con otra franja del profesor.
   // Propiedad de la memoria: si devuelve true, la lista del profesor pasa a ser dueña de la franja;
   // si devuelve false, quien llamó conserva la franja y debe liberarla.
   bool agregarFranja(FranjaHoraria* franja);

   // Devuelve la franja cuya ventana de entrada contiene la hora, o nullptr si ninguna
   // (o si hora es nullptr). La franja devuelta sigue siendo del profesor. No libera hora.
   FranjaHoraria* buscarFranjaParaEntrada(Hora* hora) const;

   // Devuelve la franja cuya ventana de salida contiene la hora, o nullptr si ninguna
   // (o si hora es nullptr). La franja devuelta sigue siendo del profesor. No libera hora.
   FranjaHoraria* buscarFranjaParaSalida(Hora* hora) const;

   // Muestra los datos de la persona y, debajo, cada franja con sangría.
   // Si no tiene franjas imprime "  Sin franjas asignadas".
   void mostrar(void) const override;

protected:
private:
   ListaDoble<FranjaHoraria>* franjas;   // franjas horarias del profesor (el profesor es dueño de la lista)


};

#endif
