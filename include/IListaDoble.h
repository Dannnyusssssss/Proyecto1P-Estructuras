/***********************************************************************
 * Module:  IListaDoble.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:02:55
 * Purpose: Declaration of the class IListaDoble
 ***********************************************************************/

// Archivo: IListaDoble.h
// Contiene la interfaz genérica IListaDoble<T>, que define las operaciones
// que debe ofrecer cualquier lista doblemente enlazada del sistema.
// Al ser un template, no tiene archivo .cpp: el compilador necesita ver la
// declaración completa al usarla con un tipo concreto (por ejemplo, IListaDoble<Profesor>).

#if !defined(__Proyecto_IListaDoble_h)
#define __Proyecto_IListaDoble_h

#include <string>

// Interfaz IListaDoble<T>
// Representa el contrato de una colección de punteros a T.
// Separa "qué hace la lista" de "cómo lo hace", para que las clases del sistema
// dependan de este contrato y no de la implementación concreta.
template<typename T>
class IListaDoble
{
public:
   // Destructor virtual: garantiza que al hacer delete a través de un puntero
   // IListaDoble<T>* se ejecute el destructor de la lista concreta y se liberen sus nodos.
   virtual ~IListaDoble() {}

   // Inserta el puntero dato al inicio de la lista. No devuelve nada.
   virtual void insertarInicio(T* dato)=0;

   // Inserta el puntero dato al final de la lista. No devuelve nada.
   virtual void insertarFinal(T* dato)=0;

   // Elimina el elemento cuya clave coincide con la recibida.
   // Devuelve true si lo encontró y eliminó; false si no existe o la lista está vacía.
   virtual bool eliminar(std::string clave)=0;

   // Busca el elemento cuya clave coincide con la recibida.
   // Devuelve su puntero, o nullptr si no existe. Es const porque no modifica la lista.
   virtual T* buscar(std::string clave) const=0;

   // Devuelve el elemento en la posición indice (empezando en 0),
   // o nullptr si el índice es negativo o está fuera de rango.
   virtual T* obtener(int indice) const=0;

   // Devuelve true si la lista no tiene elementos.
   virtual bool estaVacia(void) const=0;

   // Devuelve la cantidad de elementos de la lista.
   virtual int getTamanio(void) const=0;

   // Muestra por consola todos los elementos, en orden desde el primero.
   virtual void mostrarTodos(void) const=0;

protected:
private:

};

#endif
