/***********************************************************************
 * Module:  Persona.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 9:04:15
 * Purpose: Declaration of the class Persona
 ***********************************************************************/

// Archivo: Persona.h
// Contiene la declaración de la clase Persona, base de Profesor.

#if !defined(__Proyecto_Persona_h)
#define __Proyecto_Persona_h

#include "IMostrable.h"
#include "IIdentificable.h"
#include <string>

// Clase Persona
// Representa a una persona identificada por su cédula, con nombre y apellido.
// Implementa IIdentificable (su clave es la cédula) e IMostrable, para que una
// ListaDoble pueda buscarla y mostrarla. No valida sus datos: eso lo hace
// SistemaAsistencia con el Validador antes de crearla.
// No tiene punteros, por eso no declara destructor; el que hereda de las interfaces
// ya es virtual, así que delete funciona bien aunque el objeto real sea un Profesor.
class Persona : public IMostrable, public IIdentificable
{
public:
   // Crea una persona con cédula, nombre y apellido vacíos.
   Persona();

   // Crea una persona con los datos recibidos, guardados tal cual (sin validar).
   Persona(std::string cedula, std::string nombre, std::string apellido);

   // Devuelve la cédula.
   std::string getCedula(void) const;

   // Reemplaza la cédula. Cambia también la clave de la persona.
   void setCedula(std::string newCedula);

   // Devuelve el nombre.
   std::string getNombre(void) const;

   // Reemplaza el nombre.
   void setNombre(std::string newNombre);

   // Devuelve el apellido.
   std::string getApellido(void) const;

   // Reemplaza el apellido.
   void setApellido(std::string newApellido);

   // Devuelve la clave única de la persona: su cédula.
   std::string getClave(void) const override;

   // Imprime "Cédula: <cedula> | Nombre: <nombre> <apellido>" y un salto de línea.
   void mostrar(void) const override;

protected:
private:
   std::string cedula;     // cédula ecuatoriana de 10 dígitos; es la clave de la persona
   std::string nombre;     // nombre o nombres
   std::string apellido;   // apellido o apellidos


};

#endif
