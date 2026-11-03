/***********************************************************************
 * Module:  IMostrable.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 9:15:48
 * Purpose: Declaration of the class IMostrable
 ***********************************************************************/

// Archivo: IMostrable.h
// Contiene la interfaz IMostrable, que obliga a las clases que la implementan
// a saber mostrarse por consola.

#if !defined(__Proyecto_IMostrable_h)
#define __Proyecto_IMostrable_h

// Interfaz IMostrable
// Representa a cualquier objeto del sistema que puede imprimir su información.
// Permite que la lista doble muestre sus elementos sin conocer su tipo concreto.
class IMostrable
{
public:
   // Destructor virtual: garantiza que al hacer delete a través de un puntero
   // IMostrable* se ejecute el destructor de la clase concreta y no quede memoria sin liberar.
   virtual ~IMostrable() {}

   // Muestra por consola la información del objeto.
   // No recibe parámetros ni devuelve nada. Es const porque solo consulta el objeto.
   virtual void mostrar(void) const = 0;

protected:
private:

};

#endif
