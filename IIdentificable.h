/***********************************************************************
 * Module:  IIdentificable.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 9:15:20
 * Purpose: Declaration of the class IIdentificable
 ***********************************************************************/

// Archivo: IIdentificable.h
// Contiene la interfaz IIdentificable, que obliga a las clases que la implementan
// a entregar una clave única con la que se las puede buscar.

#if !defined(__Proyecto_IIdentificable_h)
#define __Proyecto_IIdentificable_h

#include <string>

// Interfaz IIdentificable
// Representa a cualquier objeto del sistema que tiene una clave única.
// La lista doble usa esta clave para buscar y eliminar elementos sin conocer su tipo.
class IIdentificable
{
public:
   // Destructor virtual: garantiza que al hacer delete a través de un puntero
   // IIdentificable* se ejecute el destructor de la clase concreta.
   virtual ~IIdentificable() {}

   // Devuelve la clave única del objeto (por ejemplo, la cédula de una persona).
   // No recibe parámetros. Es const porque solo consulta el objeto.
   virtual std::string getClave(void) const = 0;

protected:
private:

};

#endif
