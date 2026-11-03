/***********************************************************************
 * Module:  FranjaHoraria.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:31:16
 * Purpose: Declaration of the class FranjaHoraria
 ***********************************************************************/

// Archivo: FranjaHoraria.h
// Contiene la declaración de la clase FranjaHoraria, un bloque de clase asignado a un profesor.

#if !defined(__Proyecto_FranjaHoraria_h)
#define __Proyecto_FranjaHoraria_h

class Hora;

#include "IMostrable.h"
#include "IIdentificable.h"
#include <string>

// Clase FranjaHoraria
// Representa un bloque de clase de un profesor: hora de inicio, hora de fin y aula.
// Su responsabilidad es decidir si una hora cae dentro de la ventana de 10 minutos
// para registrar la entrada (desde horaInicio) o la salida (desde horaFinal).
// Es dueña de sus dos objetos Hora y los libera al destruirse.
// No comprueba que sea una de las 7 franjas predefinidas: eso lo hace Profesor::agregarFranja.
class FranjaHoraria : public IMostrable, public IIdentificable
{
public:
   // Crea una franja vacía: sin horas (nullptr), sin aula y sin cédula.
   FranjaHoraria();

   // Crea la franja con los datos recibidos. La franja SE QUEDA con los punteros
   // horaInicio y horaFinal (no los copia) y los liberará al destruirse:
   // quien llama no debe hacer delete de ellos.
   FranjaHoraria(Hora * horaInicio, Hora* horaFinal, std::string aula, std::string cedulaProfesor);

   // Libera horaInicio y horaFinal.
   ~FranjaHoraria() override;

   // Copia bloqueada: una copia compartiría los mismos objetos Hora y al destruirse
   // ambas franjas se haría delete dos veces de la misma memoria.
   FranjaHoraria(const FranjaHoraria&) = delete;
   FranjaHoraria& operator=(const FranjaHoraria&) = delete;

   // Devuelve la hora de inicio (la franja sigue siendo su dueña; no liberarla).
   Hora* getHoraInicio(void) const;

   // Reemplaza la hora de inicio: libera la anterior y se queda con la nueva.
   // Si recibe el mismo puntero que ya tiene, no hace nada.
   void setHoraInicio(Hora* newHoraInicio);

   // Devuelve la hora de fin (la franja sigue siendo su dueña; no liberarla).
   Hora* getHoraFinal(void) const;

   // Reemplaza la hora de fin: libera la anterior y se queda con la nueva.
   // Si recibe el mismo puntero que ya tiene, no hace nada.
   void setHoraFinal(Hora* newHoraFinal);

   // Devuelve el aula.
   std::string getAula(void) const;

   // Reemplaza el aula.
   void setAula(std::string newAula);

   // Devuelve la cédula del profesor al que pertenece la franja.
   std::string getCedulaProfesor(void) const;

   // Reemplaza la cédula del profesor. Cambia también la clave de la franja.
   void setCedulaProfesor(std::string newCedulaProfesor);

   // Devuelve true si horaInicio <= hora <= horaInicio + 10 minutos (extremos incluidos).
   // Devuelve false si hora o horaInicio son nullptr. Solo consulta hora: no la libera.
   bool puedeRegistrarEntrada(Hora* hora) const;

   // Devuelve true si horaFinal <= hora <= horaFinal + 10 minutos (extremos incluidos).
   // Devuelve false si hora o horaFinal son nullptr. Solo consulta hora: no la libera.
   bool puedeRegistrarSalida(Hora* hora) const;

   // Devuelve la clave "cedulaProfesor-HH:MM" con la hora de inicio (ej. "1710034065-07:00").
   // Si no hay hora de inicio devuelve "cedulaProfesor-".
   std::string getClave(void) const override;

   // Imprime "HH:MM - HH:MM | Aula: <aula>" y un salto de línea.
   // Si falta alguna hora, en su lugar imprime "--:--".
   void mostrar(void) const override;

protected:
private:
   Hora* horaInicio;            // hora en que empieza la clase (la franja es su dueña)
   Hora* horaFinal;             // hora en que termina la clase (la franja es su dueña)
   std::string aula;            // aula donde se dicta la clase (ej. "A-101")
   std::string cedulaProfesor;  // cédula del profesor al que pertenece la franja


};

#endif
