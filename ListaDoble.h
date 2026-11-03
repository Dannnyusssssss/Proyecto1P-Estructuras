/***********************************************************************
 * Module:  ListaDoble.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:26:41
 * Purpose: Declaration of the class ListaDoble
 ***********************************************************************/

#if !defined(__Proyecto_ListaDoble_h)
#define __Proyecto_ListaDoble_h

class Nodo;

#include <IListaDoble.h>

class ListaDoble : public IListaDoble
{
public:
   ListaDoble();
   ~ListaDoble();
   void insertarInicio(T* dato);
   void insertarFinal(T* dato);
   bool eliminar(std::string clave);
   T* buscar(std::string clave);
   T* obtener(int indice);
   bool estaVacia(void);
   int getTamanio(void);
   void mostrarTodos(void);

   Nodo** nodo;

protected:
private:
   Nodo<T>* cabeza;
   Nodo<T>* cola;
   int tamanio;


};

#endif