/***********************************************************************
 * Module:  Validador.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 9:47:10
 * Purpose: Declaration of the class Validador
 ***********************************************************************/

// Archivo: Validador.h
// Contiene la declaración de la clase Validador, que centraliza todas las
// validaciones de datos de entrada del sistema mediante expresiones regulares.

#if !defined(__Proyecto_Validador_h)
#define __Proyecto_Validador_h

#include <string>

// Clase Validador
// Responsable única de validar los textos que ingresa el usuario (cédula, nombre,
// hora, fecha, aula y opción de menú). Ninguna otra clase arma expresiones regulares.
// Solo tiene métodos static y ningún atributo: no guarda estado, se usa sin crear objetos.
class Validador
{
public:
   // Devuelve true si la cédula es ecuatoriana y real: 10 dígitos, provincia 01-24 o 30,
   // tercer dígito menor que 6 y dígito verificador correcto (módulo 10).
   // Devuelve false con cualquier otro texto, incluido el vacío.
   static bool validarCedula(std::string cedula);

   // Devuelve true si el nombre (o apellido) tiene solo letras, incluidas tildes, ñ y ü,
   // en una o más palabras separadas por un solo espacio.
   // Devuelve false si está vacío, solo tiene espacios, o contiene números o símbolos.
   static bool validarNombre(std::string nombre);

   // Devuelve true si el texto es una hora "HH:MM" de 24 horas (00:00 a 23:59).
   // Devuelve false si falta el cero a la izquierda o la hora o los minutos están fuera de rango.
   static bool validarHora(std::string horaIngresada);

   // Devuelve true si el texto es una fecha "AAAA-MM-DD" con mes 01-12 y día válido para ese mes.
   // El 29 de febrero solo se acepta en años bisiestos.
   // Devuelve false con cualquier otro formato, fecha inexistente o texto vacío.
   static bool validarFecha(std::string fecha);

   // Devuelve true si el aula tiene letras o dígitos, con guiones solo entre ellos (ej. "A-101", "Lab3").
   // Devuelve false si está vacía o tiene espacios u otros símbolos.
   static bool validarAula(std::string aula);

   // Devuelve true si la opción es un número de uno o dos dígitos, sin signo.
   // Que el número exista en el menú lo comprueba el Menu, no esta clase.
   static bool validarOpcionMenu(std::string opcion);

   // Devuelve true si la fecha "AAAA-MM-DD" es válida y cae en sábado o domingo.
   // Si la fecha no pasa validarFecha, devuelve false: quien llama debe validarla antes.
   // Se usa para impedir el registro de asistencia en fines de semana.
   static bool esFinDeSemana(std::string fecha);

protected:
private:

};

#endif
