// Archivo: ElementoPrueba.h
// Contiene la clase ElementoPrueba, usada SOLO en las pruebas de ListaDoble<T>
// para probar la lista sin depender de las clases del dominio.

#if !defined(__Proyecto_ElementoPrueba_h)
#define __Proyecto_ElementoPrueba_h

#include "IMostrable.h"
#include "IIdentificable.h"
#include <iostream>
#include <string>

// Clase ElementoPrueba
// Elemento mínimo que cumple lo que la lista necesita: tiene una clave y sabe mostrarse.
// Lleva la cuenta de cuántos objetos existen en cada momento, para que las pruebas
// comprueben que la lista libera los datos al eliminar y al destruirse.
class ElementoPrueba : public IMostrable, public IIdentificable
{
public:
   // Crea el elemento con su clave y suma uno al contador de objetos vivos.
   explicit ElementoPrueba(std::string clave) : clave(clave)
   {
      vivos++;
   }

   // Resta uno al contador de objetos vivos.
   ~ElementoPrueba() override
   {
      vivos--;
   }

   // Copia bloqueada: no se necesita y alteraría el conteo de objetos vivos.
   ElementoPrueba(const ElementoPrueba&) = delete;
   ElementoPrueba& operator=(const ElementoPrueba&) = delete;

   // Devuelve la clave del elemento.
   std::string getClave(void) const override
   {
      return clave;
   }

   // Imprime la clave seguida de un espacio, para poder verificar el orden de mostrarTodos.
   void mostrar(void) const override
   {
      std::cout << clave << " ";
   }

   // Devuelve cuántos ElementoPrueba existen en este momento.
   static int getVivos(void)
   {
      return vivos;
   }

private:
   std::string clave;        // clave con la que la lista busca y elimina el elemento
   inline static int vivos = 0;  // cantidad de objetos ElementoPrueba creados y aún no liberados
};

#endif
