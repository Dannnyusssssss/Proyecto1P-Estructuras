/***********************************************************************
 * Module:  ListaDoble.h
 * Author:  gdann
 * Modified: martes, 3 de noviembre de 2026 10:26:41
 * Purpose: Declaration of the class ListaDoble
 ***********************************************************************/

// Archivo: ListaDoble.h
// Contiene la clase genérica ListaDoble<T>, la lista doblemente enlazada que usa
// todo el sistema en lugar de los contenedores de la STL.
// Al ser un template, su implementación está en este mismo archivo y no en un .cpp:
// el compilador necesita ver el código completo del template en el momento de usarlo
// con un tipo concreto (por ejemplo, ListaDoble<Profesor>), y desde un .cpp aparte no lo vería.

#if !defined(__Proyecto_ListaDoble_h)
#define __Proyecto_ListaDoble_h

#include "Nodo.h"
#include "IListaDoble.h"
#include <string>

// Clase ListaDoble<T>
// Implementa IListaDoble<T> con nodos enlazados en ambos sentidos.
// Es DUEÑA de sus nodos y de los datos que contienen (composición): al eliminar un
// elemento o al destruirse la lista, libera ambos con delete.
// Requisito para T: debe tener getClave() const (para buscar y eliminar)
// y mostrar() const (para mostrarTodos), es decir, implementar IIdentificable e IMostrable.
// La lista no revisa claves duplicadas: esa regla de negocio la controla quien inserta.
template<typename T>
class ListaDoble : public IListaDoble<T>
{
public:
   // Crea una lista vacía.
   ListaDoble();

   // Libera todos los nodos y todos los datos que contienen.
   ~ListaDoble() override;

   // Copia bloqueada: una copia compartiría los mismos nodos y datos, y al destruirse
   // ambas listas se haría delete dos veces de la misma memoria.
   ListaDoble(const ListaDoble<T>&) = delete;
   ListaDoble<T>& operator=(const ListaDoble<T>&) = delete;

   // Inserta dato al inicio. La lista pasa a ser su dueña. Si dato es nullptr no inserta nada.
   void insertarInicio(T* dato) override;

   // Inserta dato al final. La lista pasa a ser su dueña. Si dato es nullptr no inserta nada.
   void insertarFinal(T* dato) override;

   // Elimina el primer elemento con esa clave, liberando su nodo y su dato.
   // Devuelve true si lo eliminó; false si no existe o la lista está vacía.
   bool eliminar(std::string clave) override;

   // Devuelve el primer elemento con esa clave, o nullptr si no existe o la lista está vacía.
   // La lista sigue siendo dueña del elemento devuelto: quien lo recibe no debe liberarlo.
   T* buscar(std::string clave) const override;

   // Devuelve el elemento de la posición indice (desde 0), o nullptr si el índice
   // es negativo o mayor o igual que el tamaño. La lista sigue siendo su dueña.
   T* obtener(int indice) const override;

   // Devuelve true si la lista no tiene elementos.
   bool estaVacia(void) const override;

   // Devuelve la cantidad de elementos.
   int getTamanio(void) const override;

   // Llama a mostrar() de cada elemento, desde la cabeza hasta la cola.
   // Con la lista vacía no imprime nada; el mensaje lo decide quien llama.
   void mostrarTodos(void) const override;

   // getCabeza() y getCola() se agregaron al modelo (decisión 16) SOLO para que las pruebas
   // puedan recorrer los enlaces hacia adelante y hacia atrás. El código del sistema no los usa:
   // recorre la lista con getTamanio() y obtener(), que son parte de la interfaz.
   // No tienen setters: desde afuera no se puede cambiar la estructura de la lista.

   // Devuelve el primer nodo (nullptr si la lista está vacía).
   // Permite recorrer la lista hacia adelante con getSiguiente(). No se deben liberar los nodos.
   Nodo<T>* getCabeza(void) const;

   // Devuelve el último nodo (nullptr si la lista está vacía).
   // Permite recorrer la lista hacia atrás con getAnterior(). No se deben liberar los nodos.
   Nodo<T>* getCola(void) const;

protected:
private:
   Nodo<T>* cabeza;   // primer nodo de la lista, o nullptr si está vacía
   Nodo<T>* cola;     // último nodo de la lista, o nullptr si está vacía
   int tamanio;       // cantidad de nodos que tiene la lista


};

// ---------------------------------------------------------------------
// Implementación de los métodos (en el .h por ser un template)
// ---------------------------------------------------------------------

// La lista empieza sin nodos: cabeza y cola no apuntan a nada.
template<typename T>
ListaDoble<T>::ListaDoble() : cabeza(nullptr), cola(nullptr), tamanio(0)
{
}

// Recorre la lista desde la cabeza liberando cada dato y cada nodo.
template<typename T>
ListaDoble<T>::~ListaDoble()
{
   Nodo<T>* actual = cabeza;
   while (actual != nullptr)
   {
      // Se guarda el siguiente ANTES de borrar, porque después del delete
      // ya no se puede leer actual->getSiguiente()
      Nodo<T>* siguiente = actual->getSiguiente();
      delete actual->getDato();   // la lista es dueña del dato (composición)
      delete actual;              // y también del nodo
      actual = siguiente;
   }
   // Se dejan los punteros en un estado coherente aunque el objeto vaya a desaparecer
   cabeza = nullptr;
   cola = nullptr;
   tamanio = 0;
}

// Inserta un nuevo nodo antes de la cabeza actual.
template<typename T>
void ListaDoble<T>::insertarInicio(T* dato)
{
   // Un dato nulo no se guarda: haría fallar a buscar() y mostrarTodos()
   if (dato == nullptr)
   {
      return;
   }

   Nodo<T>* nuevo = new Nodo<T>(dato);   // nodo nuevo, sin vecinos todavía

   // Si la lista está vacía, el nuevo nodo es a la vez cabeza y cola
   if (cabeza == nullptr)
   {
      cabeza = nuevo;   // el primer nodo de la lista
      cola = nuevo;     // y también el último, porque es el único
   }
   else
   {
      // Con elementos: el nuevo nodo se coloca delante de la cabeza actual
      nuevo->setSiguiente(cabeza);   // nuevo -> antigua cabeza
      cabeza->setAnterior(nuevo);    // antigua cabeza <- nuevo
      cabeza = nuevo;                // el nuevo pasa a ser el primero
   }
   tamanio++;
}

// Inserta un nuevo nodo después de la cola actual.
template<typename T>
void ListaDoble<T>::insertarFinal(T* dato)
{
   // Un dato nulo no se guarda: haría fallar a buscar() y mostrarTodos()
   if (dato == nullptr)
   {
      return;
   }

   Nodo<T>* nuevo = new Nodo<T>(dato);   // nodo nuevo, sin vecinos todavía

   // Si la lista está vacía, el nuevo nodo es a la vez cabeza y cola
   if (cola == nullptr)
   {
      cabeza = nuevo;   // el primer nodo de la lista
      cola = nuevo;     // y también el último, porque es el único
   }
   else
   {
      // Con elementos: el nuevo nodo se coloca detrás de la cola actual
      cola->setSiguiente(nuevo);   // antigua cola -> nuevo
      nuevo->setAnterior(cola);    // antigua cola <- nuevo
      cola = nuevo;                // el nuevo pasa a ser el último
   }
   tamanio++;
}

// Busca el nodo con la clave y lo desenlaza según su posición.
template<typename T>
bool ListaDoble<T>::eliminar(std::string clave)
{
   // Se localiza el nodo que tiene la clave buscada
   Nodo<T>* actual = cabeza;
   while (actual != nullptr && actual->getDato()->getClave() != clave)
   {
      actual = actual->getSiguiente();
   }

   // Lista vacía o clave inexistente: no hay nada que eliminar
   if (actual == nullptr)
   {
      return false;
   }

   if (actual == cabeza && actual == cola)
   {
      // Caso 1 - Único elemento: la lista queda vacía
      cabeza = nullptr;
      cola = nullptr;
   }
   else if (actual == cabeza)
   {
      // Caso 2 - Es la cabeza: el segundo nodo pasa a ser el primero
      cabeza = actual->getSiguiente();
      cabeza->setAnterior(nullptr);   // el nuevo primero ya no tiene a nadie antes
   }
   else if (actual == cola)
   {
      // Caso 3 - Es la cola: el penúltimo nodo pasa a ser el último
      cola = actual->getAnterior();
      cola->setSiguiente(nullptr);    // el nuevo último ya no tiene a nadie después
   }
   else
   {
      // Caso 4 - Está en el medio: sus vecinos se enlazan entre sí, saltándolo
      Nodo<T>* previo = actual->getAnterior();
      Nodo<T>* posterior = actual->getSiguiente();
      previo->setSiguiente(posterior);   // previo -> posterior
      posterior->setAnterior(previo);    // previo <- posterior
   }

   // Ya desenlazado, se liberan el dato y el nodo (la lista es dueña de ambos)
   delete actual->getDato();
   delete actual;
   tamanio--;
   return true;
}

// Recorre desde la cabeza comparando la clave de cada elemento.
template<typename T>
T* ListaDoble<T>::buscar(std::string clave) const
{
   Nodo<T>* actual = cabeza;
   while (actual != nullptr)
   {
      if (actual->getDato()->getClave() == clave)
      {
         return actual->getDato();   // se devuelve el dato, la lista sigue siendo su dueña
      }
      actual = actual->getSiguiente();
   }
   // Se recorrió toda la lista (o estaba vacía) sin encontrar la clave
   return nullptr;
}

// Avanza indice posiciones desde la cabeza.
template<typename T>
T* ListaDoble<T>::obtener(int indice) const
{
   // Índices válidos: de 0 a tamanio - 1. Fuera de ese rango no hay elemento
   if (indice < 0 || indice >= tamanio)
   {
      return nullptr;
   }

   Nodo<T>* actual = cabeza;
   for (int i = 0; i < indice; i++)
   {
      actual = actual->getSiguiente();
   }
   return actual->getDato();
}

// La lista está vacía cuando no tiene nodos.
template<typename T>
bool ListaDoble<T>::estaVacia(void) const
{
   return tamanio == 0;
}

// Devuelve el contador de nodos, que se actualiza en cada inserción y eliminación.
template<typename T>
int ListaDoble<T>::getTamanio(void) const
{
   return tamanio;
}

// Delega en mostrar() de cada elemento: la lista no sabe cómo se imprime cada tipo.
template<typename T>
void ListaDoble<T>::mostrarTodos(void) const
{
   Nodo<T>* actual = cabeza;
   while (actual != nullptr)
   {
      actual->getDato()->mostrar();
      actual = actual->getSiguiente();
   }
}

// Devuelve el primer nodo para poder recorrer la lista hacia adelante.
template<typename T>
Nodo<T>* ListaDoble<T>::getCabeza(void) const
{
   return cabeza;
}

// Devuelve el último nodo para poder recorrer la lista hacia atrás.
template<typename T>
Nodo<T>* ListaDoble<T>::getCola(void) const
{
   return cola;
}

#endif
