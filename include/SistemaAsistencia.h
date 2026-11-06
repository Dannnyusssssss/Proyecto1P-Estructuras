/***********************************************************************
 * Module:  SistemaAsistencia.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:28:38
 * Purpose: Declaration of the class SistemaAsistencia
 ***********************************************************************/

// Archivo: SistemaAsistencia.h
// Contiene la declaración de la clase SistemaAsistencia, que aplica las reglas de negocio
// del registro de asistencia docente.

#if !defined(__Proyecto_SistemaAsistencia_h)
#define __Proyecto_SistemaAsistencia_h

class Profesor;
class Asistencia;

#include "Profesor.h"
#include "FranjaHoraria.h"
#include "Hora.h"
#include "Asistencia.h"
#include "ListaDoble.h"
#include <string>

// Clase SistemaAsistencia
// Controlador del sistema: guarda los profesores y las asistencias, valida los datos con el
// Validador y aplica todas las reglas de negocio (cédulas únicas, ventanas de 10 minutos,
// una asistencia por cédula + fecha + franja, no hay salida sin entrada).
// NUNCA consulta el reloj: la fecha y la hora llegan siempre como parámetros, para que las
// pruebas puedan usar cualquier hora.
// Es dueña de sus dos listas (y, a través de ellas, de todos los profesores y asistencias).
class SistemaAsistencia
{
public:
   // Crea el sistema con las listas de profesores y asistencias vacías.
   SistemaAsistencia();

   // Libera las dos listas, que a su vez liberan cada profesor (con sus franjas) y cada asistencia.
   ~SistemaAsistencia();

   // Copia bloqueada: una copia compartiría las mismas listas y al destruirse
   // ambos sistemas se haría delete dos veces de la misma memoria.
   SistemaAsistencia(const SistemaAsistencia&) = delete;
   SistemaAsistencia& operator=(const SistemaAsistencia&) = delete;

   // Agrega el profesor. Rechaza (devuelve false) si pro es nullptr, si la cédula no es válida,
   // si el nombre o el apellido no son válidos, o si ya existe un profesor con esa cédula.
   // Propiedad de la memoria: si devuelve true, el sistema pasa a ser dueño del profesor;
   // si devuelve false, quien llamó conserva el profesor y debe liberarlo.
   bool agregarProfesor(Profesor* pro);

   // Agrega la franja al profesor con esa cédula. Rechaza (devuelve false) si fra es nullptr,
   // si no existe el profesor, si el aula no es válida, si esa aula ya está ocupada en ese
   // horario por otro profesor (sin distinguir mayúsculas: A-101 y a-101 son la misma aula)
   // o si el profesor rechaza la franja (no predefinida, de otra cédula, duplicada o cruzada).
   // Propiedad de la memoria: si devuelve true, el profesor pasa a ser dueño de la franja;
   // si devuelve false, quien llamó conserva la franja y debe liberarla.
   bool agregarFranja(std::string cedula, FranjaHoraria* fra);

   // Registra la entrada del profesor en la fecha y hora indicadas. Rechaza (devuelve false) si
   // la fecha no es válida, hora es nullptr, no existe el profesor, ninguna franja suya tiene la
   // ventana de entrada abierta a esa hora, o ya existe la asistencia de esa franja y fecha.
   // Solo consulta hora: no la libera (la asistencia guarda su propia copia).
   bool registrarEntrada(std::string cedula, std::string fecha, Hora* hora);

   // Registra la salida del profesor en la fecha y hora indicadas. Rechaza (devuelve false) si
   // la fecha no es válida, hora es nullptr, no existe el profesor, ninguna franja suya tiene la
   // ventana de salida abierta a esa hora, no hubo entrada en esa franja y fecha, o la asistencia
   // ya estaba completa. Solo consulta hora: no la libera.
   bool registrarSalida(std::string cedula, std::string fecha, Hora* hora);

   // Muestra todas las asistencias, o "No hay asistencias registradas." si no hay ninguna.
   void mostrarAsistencias(void) const;

   // Muestra las asistencias de esa cédula, o un mensaje si no tiene ninguna.
   void mostrarAsistenciaProfesor(std::string cedula) const;

   // Agregado al modelo (decisión 10, aprobada en la Fase 11): muestra cada profesor con
   // sus franjas, o "No hay profesores registrados." si no hay ninguno.
   void mostrarProfesores(void) const;

   // Agregado al modelo (decisión 28): devuelve true solo si hay un profesor registrado con esa
   // cédula. Con la cédula vacía devuelve false. No modifica nada: el menú lo usa para comprobar
   // al profesor apenas escribe su cédula, antes de mostrarle el submenú.
   bool existeProfesor(std::string cedula) const;

protected:
private:
   ListaDoble<Profesor>* profesores;      // profesores registrados (el sistema es dueño de la lista)
   ListaDoble<Asistencia>* asistencias;   // asistencias registradas (el sistema es dueño de la lista)

   // Método privado auxiliar (decisión 25): compara dos nombres de aula sin distinguir
   // mayúsculas de minúsculas. Recibe las dos aulas y devuelve true si son la misma
   // ("A-101" y "a-101" -> true). Solo convierte letras de la 'a' a la 'z', lo cual basta
   // porque Validador::validarAula solo acepta letras sin tildes, números y guion.
   bool mismaAula(std::string a, std::string b) const;


};

#endif
