/***********************************************************************
 * Module:  Asistencia.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:35:39
 * Purpose: Declaration of the class Asistencia
 ***********************************************************************/

// Archivo: Asistencia.h
// Contiene la declaración de la clase Asistencia, el registro de entrada y salida
// de un profesor en una franja y una fecha.

#if !defined(__Proyecto_Asistencia_h)
#define __Proyecto_Asistencia_h

class Hora;

#include "IMostrable.h"
#include "IIdentificable.h"
#include <string>

// Clase Asistencia
// Representa la asistencia de un profesor a UNA franja en UNA fecha.
// Su responsabilidad es llevar el estado del registro en el orden correcto:
//   0 (sin registro) -> 1 (entrada registrada) -> 2 (completa).
// Solo registrarEntrada (0 -> 1) y registrarSalida (1 -> 2) cambian el estado.
// No revisa la ventana de 10 minutos: eso lo hace SistemaAsistencia con la franja antes de llamar.
// Es dueña de sus tres objetos Hora y los libera al destruirse.
class Asistencia : public IMostrable, public IIdentificable
{
public:
   // Crea una asistencia vacía: sin cédula, sin fecha, sin horas y en estado 0.
   Asistencia();

   // Crea la asistencia de un profesor para una fecha y la franja que empieza en horaInicioFranja.
   // Guarda una COPIA de horaInicioFranja (esa hora pertenece a la franja), así que quien llama
   // conserva la original. Si horaInicioFranja es nullptr, la asistencia queda sin hora de franja.
   Asistencia(std::string cedulaProfesor, std::string fecha, Hora* horaInicioFranja);

   // Libera horaInicioFranja, horaEntrada y horaSalida.
   ~Asistencia() override;

   // Copia bloqueada: una copia compartiría los mismos objetos Hora y al destruirse
   // ambas asistencias se haría delete dos veces de la misma memoria.
   Asistencia(const Asistencia&) = delete;
   Asistencia& operator=(const Asistencia&) = delete;

   // Devuelve la cédula del profesor.
   std::string getCedulaProfesor(void) const;

   // Devuelve la fecha "AAAA-MM-DD" de la asistencia.
   std::string getFecha(void) const;

   // Devuelve la hora de entrada registrada, o nullptr si aún no hay (la asistencia es su dueña).
   Hora* getHoraEntrada(void) const;

   // Reemplaza la hora de entrada: libera la anterior y se queda con la nueva.
   // Si recibe el mismo puntero, no hace nada. NO cambia el estado.
   void setHoraEntrada(Hora* newHoraEntrada);

   // Devuelve la hora de salida registrada, o nullptr si aún no hay (la asistencia es su dueña).
   Hora* getHoraSalida(void) const;

   // Reemplaza la hora de salida: libera la anterior y se queda con la nueva.
   // Si recibe el mismo puntero, no hace nada. NO cambia el estado.
   void setHoraSalida(Hora* newHoraSalida);

   // Devuelve el estado: 0 sin registro, 1 entrada registrada, 2 completa.
   int getEstado(void) const;

   // Devuelve true si el estado es 0 (todavía no se registró la entrada).
   bool estaSinRegistro(void) const;

   // Devuelve true si el estado es 1 (se registró la entrada pero no la salida).
   bool tieneEntrada(void) const;

   // Devuelve la hora de inicio de la franja a la que corresponde (la asistencia es dueña de esta copia).
   Hora* getHoraInicioFranja(void) const;

   // Imprime "fecha | Cédula: ... | Franja: HH:MM | Entrada: HH:MM | Salida: HH:MM | Estado: ..."
   // y un salto de línea. Las horas que falten se muestran como "--:--".
   void mostrar(void) const override;

   // Devuelve true si el estado es 2 (entrada y salida registradas).
   bool estaCompleta(void) const;

   // Registra la entrada guardando una COPIA de hora y pasa el estado de 0 a 1.
   // Si el estado no es 0 (ya hay entrada) o hora es nullptr, no cambia nada.
   void registrarEntrada(Hora* hora);

   // Registra la salida guardando una COPIA de hora y pasa el estado de 1 a 2.
   // Si el estado no es 1 (no hay entrada o ya está completa) o hora es nullptr, no cambia nada.
   void registrarSalida(Hora* hora);

   // Devuelve la clave "cedula-fecha-HH:MM" (ej. "1710034065-2026-11-03-07:00").
   // Si no hay hora de franja devuelve "cedula-fecha-".
   std::string getClave(void) const override;

protected:
private:
   static const int SIN_REGISTRO = 0;         // estado inicial: sin entrada ni salida
   static const int ENTRADA_REGISTRADA = 1;   // estado tras registrar la entrada
   static const int COMPLETA = 2;             // estado tras registrar la salida

   std::string cedulaProfesor;   // cédula del profesor que registra la asistencia
   std::string fecha;            // fecha de la asistencia en formato "AAAA-MM-DD"
   Hora* horaInicioFranja;       // copia de la hora de inicio de la franja (identifica la franja)
   Hora* horaEntrada;            // hora en que se registró la entrada, o nullptr
   Hora* horaSalida;             // hora en que se registró la salida, o nullptr
   int estado;                   // 0 sin registro, 1 entrada registrada, 2 completa


};

#endif
