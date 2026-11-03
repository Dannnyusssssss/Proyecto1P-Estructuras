/***********************************************************************
 * Module:  Nodo.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:34:05
 * Purpose: Declaration of the class Nodo
 ***********************************************************************/

#if !defined(__Proyecto_Nodo_h)
#define __Proyecto_Nodo_h

class Nodo
{
public:
   Nodo(T _dato);
   T* getDato(void);
   void setDato(T* newDato);
   Nodo<T>* getSiguiente(void);
   void setSiguiente(Nodo<T>* newSiguiente);
   Nodo<T>* getAnterior(void);
   void setAnterior(Nodo<T>* newAnterior);

   Nodo* nodoB;

protected:
private:
   T* dato;
   Nodo<T>* siguiente;
   Nodo<T>* anterior;


};

#endif