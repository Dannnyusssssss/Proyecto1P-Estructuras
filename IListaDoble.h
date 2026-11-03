/***********************************************************************
 * Module:  IListaDoble.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:02:55
 * Purpose: Declaration of the class IListaDoble
 ***********************************************************************/

#if !defined(__Proyecto_IListaDoble_h)
#define __Proyecto_IListaDoble_h

class IListaDoble
{
public:
   virtual void insertarInicio(T* dato)=0;
   virtual void insertarFinal(T* dato)=0;
   virtual bool eliminar(std::string clave)=0;
   virtual T* buscar(std::string clave)=0;
   virtual T* obtener(int indice)=0;
   virtual bool estaVacia(void)=0;
   virtual int getTamanio(void)=0;
   virtual void mostrarTodos(void)=0;

protected:
private:

};

#endif